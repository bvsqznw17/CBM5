#include "RecOperate.h"
#include "UDiskOperation.h"
#include <QTextStream>
#include <qmath.h>

RecOperateWorker::RecOperateWorker(RecDB *rdb, QObject *parent) :
    QObject(parent)
{
    recDB = rdb;
    m_cancel = false;
}

void RecOperateWorker::query(QVariant v)
{
    RecQ_Handy rq = v.value<RecQ_Handy>();
    RecQR_Handy rqr = recDB->queryHandy(rq);
    v.setValue<RecQR_Handy>(rqr);
    emit queryFns(v);
}

void RecOperateWorker::queryDRec(int did)
{
    QList<Rec_Rec> list;
    RecQR_Date rd = recDB->queryDate(did, 1);
    int n = !rd.l.isEmpty() ? rd.l.at(0).drcnt : 0;
    list.reserve(n);
    for (int i = 0; i < n; i += 1000) {
        int nn = n - i;
        nn = qMin(1000, nn);
        RecQR_DRec rr = recDB->queryDRec(did, 0, nn);
        if (rr.err)
            break;
        list.append(rr.l);
    }

    RecQR_Handy rqr;
    rqr.err = rd.err;
    rqr.ld = rd.l;
    rqr.did = rd.did;
    rqr.di = rd.did;
    rqr.lr = list;
    rqr.rid = 0;
    QVariant v;
    v.setValue<RecQR_Handy>(rqr);
    emit queryFns(v);
}

void RecOperateWorker::clear()
{
    recDB->clear();
}

static QString vStrU(qint64 v, int p, int u)
{
    int worP = u >> 4;
    if (worP == 1 || worP == 2)
        return QString::number(v).append(" P");
    QString ustr;
    switch (u & 0x0F) {
    case 0: ustr = " g"; break;
    case 1: ustr = " kg"; break;
    default:  ustr = ""; break;
    }
    return QString::number(v * qPow(10, -p), 'f', p).append(ustr);
}

void RecOperateWorker::exportRec(QString name)
{
    // 查询记录总数
    RecQR_Cnt rc = recDB->queryCnt();
    RecQR_Date rqd = recDB->queryDate(0, rc.dcnt);
    rc.rcnt = 0;
    foreach (const Rec_Date &d, rqd.l)
        rc.rcnt += d.drcnt;
    if (rc.err || rqd.err || rc.rcnt <= 0) {
        emit progress(-1, 0);
        return;
    }
    emit progress(0, rc.rcnt + 1);

#ifdef MYOPT_WINDOW
    QString fileName = QString("%1.csv").arg(name);
    QString fileName2 = QString("%1-%2.csv").arg(name);
    QString fileNameS = QString("%1-stat.csv").arg(name);
#else
    QString fileName = QString("/mnt/usb/%1.csv").arg(name);
    QString fileName2 = QString("/mnt/usb/%1-%2.csv").arg(name);
    QString fileNameS = QString("/mnt/usb/%1-stat.csv").arg(name);
    // 挂载U盘
    if (!UDiskOperation::mountAny()) {
        emit progress(-6, 0);
        return;
    }
#endif


    int err = 0;
    int n = 0;
    int filenum = 0;
    const int maxline = 50*10000; // 每个文件最多50w条记录
    QFile file, fileS;
    QTextStream out, outS;

    fileS.setFileName(fileNameS);
    fileS.open(QFile::WriteOnly | QFile::Truncate);
    outS.setDevice(&fileS);
    outS << tr("记录号") << ",";
    outS << tr("程序号") << ",";
    outS << tr("产品名称") << ",";
    outS << tr("开始时间") << ",";
    outS << tr("结束时间") << ",";
    outS << tr("目标值") << ",";
    outS << tr("上限值") << ",";
    outS << tr("下限值") << ",";
    outS << tr("设置速度") << ",";
    outS << tr("合格数") << ",";
    outS << tr("不合格数") << ",";
    outS << tr("合格率") << ",";
    outS << tr("平均误差") << ",";
    outS << tr("平均速度") << ",";
    outS << tr("总重量") << ",";
    outS << tr("总个数") << "\n";

    // 每次查询多少条记录
    int nn;
    if (rc.rcnt >= 10000)
        nn = 1000;
    else if (rc.rcnt >= 1000)
        nn = 100;
    else
        nn = 10;

    for (int k=0; k<rqd.l.size(); k++) {
        const Rec_Date &d = rqd.l.at(k);
        outS << k + 1 << ",";
        outS << d.prm_id << ",";
        outS << d.prm_name << ",";
        outS << d.start_time.toString("yyyy-MM-dd hh:mm:ss") << ",";
        outS << d.end_time.toString("yyyy-MM-dd hh:mm:ss") << ",";
        outS << vStrU(d.weight1, d.prec, d.unit) << ",";
        outS << vStrU(d.weight2, d.prec, d.unit) << ",";
        outS << vStrU(d.weight3, d.prec, d.unit) << ",";
        outS << QString("%1 pmm").arg(d.speed) << ",";
        outS << d.hegeCnt << ",";
        outS << d.drcnt - d.hegeCnt << ",";

        double dval;
        dval = (double)d.hegeCnt / d.drcnt * 100;
        if (qIsNaN(dval) || qIsInf(dval))
            dval = 0;
        outS << QString("%1%").arg(dval, 0, 'f', 2) << ",";

        if (d.drcnt > 0) {
            int worP = d.unit >> 4;
            if (worP == 1 || worP == 2)
                dval = (double)d.totalPakCnt / RecDF_PakCntF / d.drcnt;
            else
                dval = (double)d.totalWeight / d.drcnt;
            if (qIsNaN(dval) || qIsInf(dval))
                dval = 0;
            outS << vStrU(qRound(dval) - d.weight1, d.prec, d.unit) << ",";
        }
        else
            outS << vStrU(0, d.prec, d.unit) << ",";

        qint64 secsto = d.start_time.secsTo(d.end_time) + 1;
        dval = (double)d.drcnt * 60 / secsto;
        if (qIsNaN(dval) || qIsInf(dval))
            dval = 0;
        outS << QString::number(dval, 'f', 1).append(" ppm") << ",";
        outS << vStrU(d.totalWeight, d.prec, d.unit&0x0F) << ",";
        outS << QString::number(d.totalPakCnt / RecDF_PakCntF, 'f', 0).append(" P") << "\n";
        if (outS.status() == QTextStream::WriteFailed) {
            err = -4;
            goto End;
        }

        for (int i=0; i<d.drcnt; i+=nn) {
            int nnn;
            if (i+nn < d.drcnt)
                nnn = nn;
            else
                nnn = d.drcnt - i;

            RecQR_DRec rqr = recDB->queryDRec(k, i, nnn);
            if (rqr.err) {
                err = -1;
                goto End;
            }

            for (int j=0; j<rqr.l.size(); j++) {
                if (n++ % maxline == 0) {
                    file.close();
                    if (rc.rcnt <= maxline)
                        file.setFileName(fileName);
                    else
                        file.setFileName(fileName2.arg(++filenum));
                    file.open(QFile::WriteOnly | QFile::Truncate);
                    out.setDevice(&file);
                    out << tr("记录号") << ",";
                    out << tr("具体记录号") << ",";
                    out << tr("重量") << ",";
                    out << tr("单位") << ",";
                    out << tr("合格") << ",";
                    out << tr("时间") << "\n";
                }
                const Rec_Rec r = rqr.l.at(j);
                out << k + 1 << ",";
                out << i + 1 + j << ",";
                int worP = r.unit >> 4;
                if (worP == 1 || worP == 2) {
                    out << QString::number(r.weight) << ",";
                    out << "P" << ",";
                }
                else {
                    out << QString::number(r.weight * qPow(10, -r.prec), 'f', r.prec) << ",";
                    switch (r.unit & 0x0F) {
                    case 0: out << "g"; break;
                    case 1: out << "kg"; break;
                    }
                    out << ",";
                }
                out << (r.hege ? tr("Y") : tr("N")) << ",";
                out << r.time.toString("yyyy-MM-dd hh:mm:ss") << "\n";
                if (out.status() == QTextStream::WriteFailed) {
                    err = -4;
                    goto End;
                }
            }

            if (m_cancel) {
                err = -2;
                goto End;
            }
            emit progress(n, rc.rcnt + 1);
        }
    }

    End:
    qDebug("export:%d", n);
    file.close();
    fileS.close();
#ifndef MYOPT_WINDOW
    system("sync");
    UDiskOperation::umountAny();
#endif
    if (err)
        emit progress(err, 0);
    else
        emit progress(rc.rcnt + 1, rc.rcnt + 1);
}

RecOperate::RecOperate(RecDB *rdb, QObject *parent) :
    QObject(parent),
    worker(rdb)
{
    worker.moveToThread(&thread);
    thread.start();
}

RecOperate::~RecOperate()
{
    thread.quit();
    thread.wait();
}

void RecOperate::query(const RecQ_Handy &rq)
{
    QVariant v;
    v.setValue<RecQ_Handy>(rq);
    QMetaObject::invokeMethod(&worker, "query", Q_ARG(QVariant, v));
}

void RecOperate::queryDRec(int did)
{
    QMetaObject::invokeMethod(&worker, "queryDRec", Q_ARG(int, did));
}

void RecOperate::clear()
{
    QMetaObject::invokeMethod(&worker, "clear");
}

void RecOperate::exportRec(const QString &name)
{
    worker.cancel(false);
    QMetaObject::invokeMethod(&worker, "exportRec", Q_ARG(QString, name));
}
