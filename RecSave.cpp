#include "RecSave.h"
#include "MTools.h"
#include "Tool.h"
#include "FormA.h"
#include "PrintThread.h"
#include "SPrinter.h"
#include "TcpComm.h"

#undef MAC
#undef MP
#undef MMP
#define MAC         (*mac)
#define MP          (mp.m)
#define MMP         (mp.mm)

RecSaveWorker::RecSaveWorker(RecDB *rdb, QObject *parent) :
    QObject(parent)
{
    recDB = rdb;
}

void RecSaveWorker::addRec(const QVariant &v)
{
    RecSave_Rec rrs = v.value<RecSave_Rec>();
    recDB->addRec(rrs);
}

void RecSaveWorker::addRecStart(const QVariant &v)
{
    RecSave_Rec rrs = v.value<RecSave_Rec>();
    recDB->addRecStart(rrs.d);
}

void RecSaveWorker::addRecEnd(const QVariant &v)
{
    QDateTime d = v.value<QDateTime>();
    recDB->addRecEnd(d);
}

RecSave::RecSave(FormA *f,
                 RecDB *rdb) :
    QObject(0),
    worker(rdb)
{
    worker.moveToThread(&workerThd);
    workerThd.start();

    form = f;
    mp.firstReadMpslist.clear();
    mac = new Machine(f->mac.maddr(), f->mac.mcomm(), &mp, this);
    mac->echom.create(echoff, ecIds, this, "onReadFnsFF");
    mac->echom.create(echof, ecIds, this, "onReadFns");
    mac->echom.create(echor, ecIds, this, "onReadFns");
    mac->echom.create(echop, ecIds, this, "onReadFns");

    rir.span(mp.m.cmd_param, mp.m.runstatus_union);
    rip.span(mp.m.prm_id, mp.m.prm_AutoZeroTime);
    ris.span(mp.m.sys_CPUID, mp.m.sys_AutoZeroWeight);

    error = 0;
    isRun = false;
    isAddStart = false;
    rn = 0;
    rn2 = 0;
    rrs_pfi = -1;

    connect(form->tcpComm, SIGNAL(inNumber(int)),
            this, SLOT(mc_in_tweight(int)));

    this->moveToThread(&thd);
    QTimer::singleShot(0, this, SLOT(onReadFirst()));
}

RecSave::~RecSave()
{
    thd.quit();
    thd.wait();
    workerThd.quit();
    workerThd.wait();
    mac->echom.destroy(ecIds, this);
}

// 第一次需要读取的
void RecSave::onReadFirst()
{
    mac->read(ris);
    mac->read(rip);
    mac->read(rir, echof);
}

// 读取记录需要的(读取失败时会反复调用)
void RecSave::onReadRec()
{
    mac->read(rip);
    mac->read(rir, echor);
}

// 读取配方(系统参数/程序/产品)(也是记录需要的)
void RecSave::onReadPF()
{
    mac->read(ris, echop);
}

void RecSave::onReadFns(int err, const SignalEcho &ec)
{
    mac->echom.finish(ec);

    int olderr = error;
    error = err;
    if (olderr != err)
        emit errChanged(err);

    if (err) {
        QTimer::singleShot(100, this, SLOT(onReadFirst()));
        return;
    }

    if (ec == echof) {
        // 收到第一帧，做一些初始化
        QTimer::singleShot(0, this, SLOT(onReadFF()));
        emit readFns(ris.addr, QByteArray(mp.data() + ris.addr*2, ris.count*2));
        emit readFns(rip.addr, QByteArray(mp.data() + rip.addr*2, rip.count*2));
        emit readFns(rir.addr, QByteArray(mp.data() + rir.addr*2, rir.count*2));
        return;
    }

    savePFInfo();
    if (ec == echor) {
        bool isRun0 = false;
        for (int i = 0; i < form->gn; i++) {
            if (mp.m.cmb_RunStatus[i]->value().toUInt() & 0x01) {
                isRun0 = true;
                break;
            }
        }
        if (isRun0 && !isRun) {
            // 如果是刚开始运行，则先去读取配方信息再去保存记录
            onReadPF();
            isAddStart = true;
        }
        else {
            trySaveRec();
            QTimer::singleShot(100, this, SLOT(onReadRec()));
        }

        if (!isRun0 && isRun) {
            addRecEnd();
        }

        emit readFns(rip.addr, QByteArray(mp.data() + rip.addr*2, rip.count*2));
        emit readFns(rir.addr, QByteArray(mp.data() + rir.addr*2, rir.count*2));

        isRun = isRun0;
    }
    else if (ec == echop) {
        // 在读取配方信息之前，就有可能已经得到新记录了，
        // 所以尽快保存记录，不需要再重新读记录，尽量避免漏存
        trySaveRec();
        QTimer::singleShot(100, this, SLOT(onReadRec()));

        emit readFns(ris.addr, QByteArray(mp.data() + ris.addr*2, ris.count*2));
    }
}

void RecSave::onReadFnsFF(int , const SignalEcho &ec)
{
    mac->echom.finish(ec);
    loop->exit();
}

void RecSave::onReadFF()
{
    rn = mp.m.cmb_combinCount[0]->value().toInt();
    rn2 = mp.m.cmb_combinCount2[0]->value().toInt();
    savePFInfo();
    QEventLoop _loop;
    loop = &_loop;
    for (int i = 0; i < form->gn; i++) {
        int id = MP.sys_prm_ids[i]->value().toInt();
        if (id <= 0 || id >= 100)
            continue;
        writeCMD(CTL_PRM_READ, id);
        mac->read(ris, echoff);
        loop->exec();
        savePFInfo();
    }
    writeCMD(0x00,CTL_PRM_WRITE,0x41,0x00);
    QTimer::singleShot(100, this, SLOT(onReadRec()));
}

void RecSave::savePFInfo()
{
    int i = MP.prm_id->value().toInt() & 0xFF;
    if (i <= 0 || i >= 100)
        return;
    QMutexLocker locker(&pf_mutex);
    pf[i].name = MP.prm_name->vStr();
    pf[i].setw[0] = MP.prm_SetWeight->valuenp().toInt();
    pf[i].setw[1] = MP.prm_SetWeight1->valuenp().toInt();
    pf[i].setw[2] = MP.prm_SetWeight2->valuenp().toInt();
    pf[i].worP = MP.prm_WorP->valuenp().toInt();
    pf[i].speed = MP.prm_speed->valuenp().toInt();
    pf[i].afc = MP.prm_AFCMode->valuenp().toInt();
    pf[i].dww = MP.prm_DWW->valuenp().toInt();
    pf[i].dwwup = MP.prm_DWW_UpLimit->valuenp().toInt();
    pf[i].dwwdn = MP.prm_DWW_DownLimit->valuenp().toInt();
    pf[i].dwwai = MP.prm_P2Cnt->valuenp().toInt();
}

// 如果得到了新产生的记录就保存
void RecSave::trySaveRec()
{
#ifdef MYOPT_OnlineManager
    // 联机软件无需保存记录，直接返回
    return;
#endif
    if (isAddStart) {
        isAddStart = false;
        addRecStart();
    }

    // 组合计数在下位机不会归零
    uint recn = mp.m.cmb_combinCount[0]->value().toInt();
    uint recn2 = mp.m.cmb_combinCount2[0]->value().toInt();
    if (recn != rn && rrs_pfi >= 0) {
        //PrintThread::printString(mp.m.cmb_Weight[0]->vStr());
        g_sprinter.printString(mp.m.cmb_Weight[0]->vStr());

        uint n = recn - rn;
        if (n > 1) // 记录缓存溢出，出现漏存
            n = 1;
        rrs.l.clear();
        rrs.l.reserve(n);
        QDateTime dt = form->dt();
        rrs.d.end_time = dt;
        for (int i = n-1; i >= 0; i--) {
            Rec_Rec r;
            r.weight = mp.m.cmb_Weight[0]->valuenp().toInt();
            r.time = dt;
            r.hege = !(mp.m.cmb_RunStatus[0]->valuenp().toInt() & 0x10);
            r.prec = rrs.d.prec;
            r.unit = rrs.d.unit;
            rrs.l.append(r);

            if (r.hege)
                rrs.d.hegeCnt++;

            int worP = pf[rrs_pfi].worP;
            int dww = pf[rrs_pfi].dww;
            int dwwai = pf[rrs_pfi].dwwai;
            if (worP == 2 && dwwai > 0)
                dww = MP.cmb_DWW[0]->valuenp().toInt();

            if (worP == 1 || worP == 2)
                rrs.d.totalWeight += r.weight * dww;
            else
                rrs.d.totalWeight += r.weight;

            if (worP == 1 || worP == 2)
                rrs.d.totalPakCnt += r.weight * RecDF_PakCntF;
            else if (dww > 0)
                rrs.d.totalPakCnt += qRound64((double)r.weight/dww * RecDF_PakCntF);
        }
        QVariant v;
        v.setValue<RecSave_Rec>(rrs);
        QMetaObject::invokeMethod(&worker, "addRec",
                                  Q_ARG(QVariant, v));
    }
    if (recn2 != rn2) {
        form->tcpComm->outNumber(MP.cmb_Weight2[0]->valuenp().toInt());
        emit inTWeightChanged(INT_MAX);
    }
    rn = recn;
    rn2 = recn2;
}

void RecSave::addRecStart()
{
    int i = MP.sys_prm_ids[0]->value().toInt();
    if (i <= 0 || i >= 100)
        return;
    rrs.d.prm_id = i;
    rrs.d.prm_name = pf[i].name;
    rrs.d.start_time = form->dt();
    rrs.d.end_time = rrs.d.start_time;
    rrs.d.weight1 = pf[i].setw[0];
    rrs.d.weight2 = pf[i].setw[1];
    rrs.d.weight3 = pf[i].setw[2];
    rrs.d.speed = pf[i].speed;
    rrs.d.hegeCnt = 0;
    rrs.d.totalPakCnt = 0;
    rrs.d.totalWeight = 0;
    rrs.d.prec = MP.sys_dot_num->value().toInt() % 4;
    rrs.d.unit = MP.sys_Unit->value().toInt() & 0x0F;
    rrs.d.unit |= pf[i].worP << 4;
    rrs_pfi = i;
    QVariant v;
    v.setValue<RecSave_Rec>(rrs);
    QMetaObject::invokeMethod(&worker, "addRecStart",
                              Q_ARG(QVariant, v));
}

void RecSave::addRecEnd()
{
    QVariant v;
    v.setValue<QDateTime>(form->dt());
    QMetaObject::invokeMethod(&worker, "addRecEnd",
                              Q_ARG(QVariant, v));
}

QDateTime RecSave::dt()
{
    return Tool::currentDateTime();
}

void RecSave::writeCMD(quint16 cmd, quint16 cmdp)
{
    QList<const MParam *>mps;
    QVariantList vals;
    mps.reserve(2);
    mps.append(MP.cmd_param);
    mps.append(MP.cmd);
    vals.reserve(2);
    vals.append(cmdp);
    vals.append(cmd);
    mac->write(mps, vals);
}

void RecSave::mc_in_tweight(int w)
{
    if (w < 1 || w > 99999 || !isRun)
        return;
    writeCMD((w>>16)&0xFF, CTL_IN_TWEIGHT, (w>>8)&0xFF, (w>>0)&0xFF);
    emit inTWeightChanged(w);
}
