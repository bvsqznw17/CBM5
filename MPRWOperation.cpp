#include "MPRWOperation.h"
#include "UDiskOperation.h"

MPRWOperation::MPRWOperation(int maddr, MachineComm *comm) :
    QObject(0)
{
    mp.firstReadMpslist.clear();
    mac = new Machine(maddr, comm, &mp, this);
    mac->echom.create(echo, ecIds, this, "onMPRWFns");

    moveToThread(&thd);
    thd.start();
}

MPRWOperation::~MPRWOperation()
{
    thd.quit();
    thd.wait();
    mac->echom.destroy(ecIds, this);
}

void MPRWOperation::Import(const QString &name)
{
    QEventLoop _loop;
    loop = &_loop;
    m_cancel = false;
    int curp = -1;
    int totalp = 2+99-5+25-5+1;
    emit progress(++curp, totalp);

    // 读入文件
#ifndef MYOPT_WINDOW
    QString fileName = QString("/mnt/usb/%1").arg(name);
    if (!UDiskOperation::mountAny()) {
        emit progress(-6, 0);
        return;
    }
#else
    QString fileName = QString("%1").arg(name);
#endif
    QFile file(fileName);
    file.open(QFile::ReadOnly);
    QByteArray data = file.readAll();
    QDataStream in(data);
    MRegRWInfo r;
    file.close();
#ifndef MYOPT_WINDOW
    UDiskOperation::umountAny();
#endif

    // 检查文件头
    char c[256];
    c[32] = 0;
    in.readRawData(c, 32);
    if (data.size() != 28006 ||
            QString::fromLatin1(c,12) != "modify_param" ||
            QString::fromLatin1(c+12) != "cbm5_param_v1") {
        emit progress(-5, 0);
        return;
    }
    emit progress(++curp, totalp);

    // 写入系统参数
    in.skipRawData(15*2);
    r.span(mp.m.sys_prm_ids[0], mp.m.sys_BurdeningOpt);
    if (waitMPRW_Import(in, r, curp, totalp))
        return;
    r.span(mp.m.sys_ioin_pin, mp.m._sys_end);
    if (waitMPRW_Import(in, r, curp, totalp))
        return;

    // 写入程序参数
    MRegRWInfo rp[3];
    rp[0].span(mp.m.prm_name, mp.m.prm_ASF);
    rp[1].span(mp.m.prm_ZDD_Dou[0], mp.m._prm_end);
    rp[2].count = 0;
    for (int i = 1; i < 95; i++) {
        writeCMD(CTL_PRM_WRITE, i);
        if (waitMPRW_Import(in, rp, curp, totalp))
            return;
    }
    for (int i = 95; i < 100; i++)
        for (int j = 0; rp[j].count; j++)
            in.skipRawData(rp[j].count*2);

    // 加载当前程序并可写
    writeCMD(0x00,CTL_PRM_WRITE,0x41,0x00);

    // 写入马达参数
    r.span(mp.m.motor_pos[0], mp.m._motor_end);
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 4; j++) {
            int mm = (i << 4) | j;
            writeCMD(CTL_MOTOR_SELECT, mm);
            if (waitMPRW_Import(in, r, curp, totalp))
                return;
        }
        in.skipRawData(r.count*2);
    }

    if (!in.atEnd())
        qWarning("MPRWOperation::Import: !in.atEnd()");
    if (in.status() != QDataStream::Ok)
        qWarning("MPRWOperation::Import: in.status() != QDataStream::Ok");
}

void MPRWOperation::Export(const QString &name)
{
    QEventLoop _loop;
    loop = &_loop;
    m_cancel = false;
    int curp = -1;
    int totalp = 2+99+25+1;
    emit progress(++curp, totalp);

#ifndef MYOPT_WINDOW
    if (UDiskOperation::monitor().isEmpty()) {
        emit progress(-6, 0);
        return;
    }
#endif

    QByteArray data;
    QDataStream out(&data, QFile::WriteOnly| QFile::Truncate);
    MRegRWInfo r;

    // 写文件头
    out.writeRawData("modify_param", 12);
    char version[20] = "cbm5_param_v1";
    out.writeRawData(version, 20);

    // 读取系统参数
    r.span(mp.m.sys_CPUID, mp.m.sys_BurdeningOpt);
    if (waitMPRW_Export(out, r, curp, totalp))
        return;
    r.span(mp.m.sys_ioin_pin, mp.m._sys_end);
    if (waitMPRW_Export(out, r, curp, totalp))
        return;

    // 读取程序参数
    MRegRWInfo rp[3];
    rp[0].span(mp.m.prm_name, mp.m.prm_ASF);
    rp[1].span(mp.m.prm_ZDD_Dou[0], mp.m._prm_end);
    rp[2].count = 0;
    for (int i = 1; i < 100; i++) {
        writeCMD(CTL_PRM_READ, i);
        if (waitMPRW_Export(out, rp, curp, totalp))
            return;
    }

    // 加载当前程序并可写
    writeCMD(0x00,CTL_PRM_WRITE,0x41,0x00);

    // 读取马达参数
    r.span(mp.m.motor_pos[0], mp.m._motor_end);
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            int mm = (i << 4) | j;
            writeCMD(CTL_MOTOR_SELECT, mm);
            if (waitMPRW_Export(out, r, curp, totalp))
                return;
        }
    }

    // 写入文件
#ifndef MYOPT_WINDOW
    QString fileName = QString("/mnt/usb/%1").arg(name);
    if (!UDiskOperation::mountAny()) {
        emit progress(-6, 0);
        return;
    }
#else
    QString fileName = QString("%1").arg(name);
#endif
    QFile file(fileName);
    file.open(QFile::WriteOnly| QFile::Truncate);
    file.write(data);
    file.close();
#ifndef MYOPT_WINDOW
    system("sync");
    UDiskOperation::umountAny();
#endif
    if (file.error())
        emit progress(-4, 0);
    else
        emit progress(totalp, totalp);
}

QString MPRWOperation::errStr(int err)
{
    switch (err) {
    case -1: return tr("错误");
    case -2: return tr("已被取消");
    case -3: return tr("通信错误");
    case -4: return tr("写入文件失败");
    case -5: return tr("文件格式错误");
    case -6: return tr("未检测到U盘");
    default: return tr("完成");
    }
}

void MPRWOperation::onMPRWFns(int err, const SignalEcho_zy1 &ec)
{
    mac->echom.finish(ec);
    loop->exit(err);
}

int MPRWOperation::waitMPRW_Import(QDataStream &in, MRegRWInfo &r, int &curp, int totalp)
{
    char c[256];
    in.readRawData(c, r.count*2);
    r.data = QByteArray(c, r.count*2);
    mac->write(r, echo);
    int wr = waitMPRW();
    if (wr) {
        emit progress(-3, 0);
        return -3;
    }
    if (m_cancel) {
        emit progress(-2, 0);
        return -2;
    }
    emit progress(++curp, totalp);
#ifdef MYOPT_DEBUG
    qDebug("%d/%d", curp, totalp);
#endif

    return 0;
}

int MPRWOperation::waitMPRW_Export(QDataStream &out, MRegRWInfo &r, int &curp, int totalp)
{
    mac->read(r, echo);
    int wr = waitMPRW();
    if (wr) {
        emit progress(-3, 0);
        return -3;
    }
    if (m_cancel) {
        emit progress(-2, 0);
        return -2;
    }
    out.writeRawData(mp.data() + r.addr*2, r.count*2);
    emit progress(++curp, totalp);
#ifdef MYOPT_DEBUG
    qDebug("%d", r.count*2);
    qDebug("%s", QByteArray(mp.data() + r.addr*2, r.count*2).toHex().constData());
    qDebug("%d/%d", curp, totalp);
#endif

    return 0;
}

int MPRWOperation::waitMPRW_Import(QDataStream &in, MRegRWInfo *rs, int &curp, int totalp)
{
    char c[256];
    while (1) {
        MRegRWInfo &r = *rs;
        in.readRawData(c, r.count*2);
        r.data = QByteArray(c, r.count*2);
        if ((++rs)->count)
            mac->write(r);
        else {
            mac->write(r, echo);
            break;
        }
    }

    int wr = waitMPRW();
    if (wr) {
        emit progress(-3, 0);
        return -3;
    }
    if (m_cancel) {
        emit progress(-2, 0);
        return -2;
    }
    emit progress(++curp, totalp);
#ifdef MYOPT_DEBUG
    qDebug("%d/%d", curp, totalp);
#endif

    return 0;
}

int MPRWOperation::waitMPRW_Export(QDataStream &out, MRegRWInfo *rs, int &curp, int totalp)
{
    MRegRWInfo *rs0 = rs;
    while (1) {
        MRegRWInfo &r = *rs;
        if ((++rs)->count)
            mac->read(r);
        else {
            mac->read(r, echo);
            break;
        }
    }

    int wr = waitMPRW();
    if (wr) {
        emit progress(-3, 0);
        return -3;
    }
    if (m_cancel) {
        emit progress(-2, 0);
        return -2;
    }
    rs = rs0;
    do {
        MRegRWInfo &r = *rs;
        out.writeRawData(mp.data() + r.addr*2, r.count*2);
#ifdef MYOPT_DEBUG
    qDebug("%d", r.count*2);
    qDebug("%s", QByteArray(mp.data() + r.addr*2, r.count*2).toHex().constData());
#endif
    } while ((++rs)->count);
    emit progress(++curp, totalp);
#ifdef MYOPT_DEBUG
    qDebug("%d/%d", curp, totalp);
#endif

    return 0;
}

void MPRWOperation::writeCMD(quint16 cmd, quint16 cmdp)
{
    QList<const MParam *>mps;
    QVariantList vals;
    mps.reserve(2);
    mps.append(mp.m.cmd_param);
    mps.append(mp.m.cmd);
    vals.reserve(2);
    vals.append(cmdp);
    vals.append(cmd);
    mac->write(mps, vals);
}
