#include "MachineComm.h"

//#define MC_Print
#ifdef MC_Print
void MachineComm_print(const char *prefix, const char *data, int size, bool warning = false);
#define MC_Debug(A, B) MachineComm_print(A, B.constData(), B.size())
#define MC_Warning(A, B) MachineComm_print(A, B.constData(), B.size(), true)
#else
#define MC_Debug(A, B)
#define MC_Warning(A, B)
#endif

#ifndef MYOPT_MCTCP
#include "CRC.h"

MachineCommRTU::MachineCommRTU(QIODevice *io, QObject *parent) :
    QObject(parent),
    timerReadyRead(this),
    timerTimeout(this)
{
    qRegisterMetaType<MCRequest>("MCRequest_zy1");
    qRegisterMetaType<MCResponse>("MCResponse_zy1");
    qRegisterMetaType<QList<MCRequest> >("QList<MCRequest_zy1>");
    m_io = io;
    m_io->setParent(this);

    timerReadyRead.setInterval(5);
    timerReadyRead.setSingleShot(true);
    timerTimeout.setSingleShot(true);
    connect(&timerReadyRead, SIGNAL(timeout()), this, SLOT(onReadyRead()));
    connect(&timerTimeout, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

void MachineCommRTU::request(const MCRequest &req, const SignalEcho &echo)
{
    request(QList<MCRequest>() << req, echo);
}

// 一个请求可以包含多个请求帧，只要QList里的一个请求帧发送失败了，Qlist后面的就不再发送
void MachineCommRTU::request(const QList<MCRequest> &reqs, const SignalEcho &echo)
{
    if (echo.isValid())
        echom.from(echo, reqs.size());

    QMetaObject::invokeMethod(this, "newRequest", Qt::QueuedConnection,
                              Q_ARG_List_MCRequest(reqs),
                              Q_ARG_SignalEcho(echo));
}

// 清除队列中跟指定echo相关的请求
// 如果请求正在发送，则会停止发送，但不会立即开始发送下一请求，至少会等到响应帧接收完成
// 这个函数会重新获取echo，注意要在回调函数里判断echo是否相等
void MachineCommRTU::clearRequest(SignalEcho &echo, SignalEchoIds &ids)
{
    if (!echo.isValid())
        return;

    // 稍后将请求队列中跟旧echo有关的项移除掉
    QMetaObject::invokeMethod(this, "clearRequest_p", Qt::QueuedConnection,
                              Q_ARG_SignalEcho(echo));
    // 重新获取echo，旧echo的回调将会无效
    echom.reCreate(echo, ids);
}

void MachineCommRTU::newRequest(const QList<MCRequest> &reqs, const SignalEcho &echo)
{
    reqsQueue.enqueue(reqs);
    echoQueue.enqueue(echo);
    // 如果当前没有正在发送的请求，则直接发送
    if (curReq.isEmpty()) {
        nextRequest();
    }
}

void MachineCommRTU::clearRequest_p(const SignalEcho &echo)
{
    for (int i = echoQueue.size() - 1; i >= 0; i--) {
        if (echoQueue.at(i) == echo) {
            echom.echoEmpty(echo, reqsQueue.at(i).size());
            echoQueue.removeAt(i);
            reqsQueue.removeAt(i);
        }
    }
    if (curReqi != -1 && curEcho == echo) {
        curClear = true;
        echom.echoEmpty(curEcho, curReq.size() - curReqi);
    }
}

// 发送队列里的下一个请求
void MachineCommRTU::nextRequest()
{
    send.clear();
    recv.clear();
    curReq.clear();
    curEcho = SignalEcho();
    curReqi = -1;
    curClear = false;
    if (reqsQueue.isEmpty())
        return;

    curReq = reqsQueue.dequeue();
    curEcho = echoQueue.dequeue();

    sendRequest();
}

void MachineCommRTU::sendRequest()
{
    curReqi++;
    if (curReqi >= curReq.size()) {
        nextRequest();
        return;
    }

    const MCRequest &req = curReq.at(curReqi);
    int reqlen = req.data.length() + 2;
    rsplen = req.rsplen + 2;
    QByteArray ba(reqlen, Qt::Uninitialized);
    char *data = ba.data();
    memcpy(&data[0], req.data.constData(), req.data.length());
    crc16_make(data, reqlen);

    send = ba;
    resendccnt = -1;
    resendcnt = req.resendcnt;
    timeoutms = req.timeoutms;
    resendRequest();
}

void MachineCommRTU::resendRequest()
{
    if (curClear) {
        response(-1);
        return;
    }
    recv.clear();
    recv.reserve(rsplen);
    resendccnt++;
#ifdef MC_Print
    QByteArray tmp = m_io->readAll();
    if (!tmp.isEmpty())
        MC_Warning("cr", tmp);
#else
    m_io->readAll();
#endif
    m_io->write(send);
    timerReadyRead.start();
    timerTimeout.start(timeoutms + timerReadyRead.interval());
    MC_Debug(">>", send);
}

// err: 0：无错误，-1：超时，其他：异常码
void MachineCommRTU::response(int err)
{
    if (err == -1)
        recv.clear();
    recv.resize(recv.size() - 2);
    timerReadyRead.stop();
    timerTimeout.stop();

    if (curClear) {
        nextRequest();
        return;
    }

    MCRequest req = curReq.at(curReqi);
    MCResponse rsp;
    rsp.data = recv;

    if (curEcho.isValid())
        echom.echo(curEcho,
                   Q_ARG(int, err),
                   Q_ARG_MCRequest(req),
                   Q_ARG_MCResponse(rsp),
                   Q_ARG_SignalEcho(curEcho));

    if (err == 0)
        sendRequest();
    else {
        if (curEcho.isValid())
            echom.echoEmpty(curEcho, curReq.size() - curReqi - 1);
        nextRequest();
    }
}

void MachineCommRTU::onReadyRead()
{
    QByteArray ba = m_io->readAll();
    if (!ba.isEmpty()) {
        recv.append(ba);
        if (recv.size() < 2)
            return;

        if ((uchar)recv.at(1) > (uchar)0x80) {
            // 收到异常帧(功能码大于0x80)
            const int errflen = 5;
            if (recv.length() == errflen) {
                // 帧长度正确
                if (recv.at(0) == send.at(0) && crc16_check(recv)) {
                    // 校验正确
                    MC_Debug("< ", recv);
                    uchar err = recv.at(2);
#ifdef MYOPT_ModbusTcpServer_V1
                    response((err == 0xFF) ? -1 : err);
#else
                    response(err);
#endif
                    return;
                }
                else {
                    // 校验出错，清空接收缓存，继续等待接收
                    MC_Warning("x ", recv);
                    recv.clear();
                }
            }
            else if (recv.length() > errflen) {
                // 长度出错，清空接收缓存，继续等待接收
                MC_Warning("x ", recv);
                recv.clear();
            }
        }
        else {
            // 收到正常帧
            if (recv.length() == rsplen) {
                // 帧长度正确
                if (recv.at(0) == send.at(0) && crc16_check(recv)) {
                    // 校验正确
                    MC_Debug("< ", recv);
                    response(0);
                    return;
                }
                else {
                    // 校验出错，清空接收缓存，继续等待接收
                    MC_Warning("x ", recv);
                    recv.clear();
                }
            }
            else if (recv.length() > rsplen) {
                // 长度出错，清空接收缓存，继续等待接收
                MC_Warning("x ", recv);
                recv.clear();
            }
        }
    }
    timerReadyRead.start();
}

void MachineCommRTU::onTimeout()
{
    // 接收超时
    MC_Warning("to", recv);
    if (resendccnt < resendcnt)
        resendRequest();
    else
        response(-1);
}
#else

MachineCommTCP::MachineCommTCP(QTcpSocket *s, QObject *parent) :
    QObject(parent),
    timerTimeout(this)
{
    qRegisterMetaType<MCRequest>("MCRequest_zy1");
    qRegisterMetaType<MCResponse>("MCResponse_zy1");
    qRegisterMetaType<QList<MCRequest> >("QList<MCRequest_zy1>");
    socket = s;
    socket->setParent(this);

    curReqi = -1;
    workidSend = 0;
    workidRecv = 0;
    timerTimeout.setSingleShot(true);
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(&timerTimeout, SIGNAL(timeout()), this, SLOT(onTimeout()));

    socketIsReconnecting = false;
    socketIsConnected = true;
#ifndef MYOPT_MCTCP_S
    connect(socket, SIGNAL(connected()),
            this, SLOT(onSocketConnected()));
    connect(socket, SIGNAL(disconnected()),
            this, SLOT(onSocketDisconnected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(onSocketDisconnected()));
    QTimer::singleShot(0, this, SLOT(onSocketReconnect()));
#endif
}

void MachineCommTCP::request(const MCRequest &req, const SignalEcho &echo)
{
    request(QList<MCRequest>() << req, echo);
}

// 一个请求可以包含多个请求帧，只要QList里的一个请求帧发送失败了，Qlist后面的就不再发送
void MachineCommTCP::request(const QList<MCRequest> &reqs, const SignalEcho &echo)
{
    if (echo.isValid())
        echom.from(echo, reqs.size());

    QMetaObject::invokeMethod(this, "newRequest", Qt::QueuedConnection,
                              Q_ARG_List_MCRequest(reqs),
                              Q_ARG_SignalEcho(echo));
}

// 清除队列中跟指定echo相关的请求
// 如果请求正在发送，则会停止发送，但不会立即开始发送下一请求，至少会等到响应帧接收完成
// 这个函数会修改echo，注意要在回调函数里判断echo是否相等
void MachineCommTCP::clearRequest(SignalEcho &echo, SignalEchoIds &ids)
{
    if (!echo.isValid())
        return;

    // 稍后将请求队列中跟旧echo有关的项移除掉
    QMetaObject::invokeMethod(this, "clearRequest_p", Qt::QueuedConnection,
                              Q_ARG_SignalEcho(echo));
    // 重新获取echo，旧echo的回调将会无效
    echom.reCreate(echo, ids);
}

void MachineCommTCP::newRequest(const QList<MCRequest> &reqs, const SignalEcho &echo)
{
    reqsQueue.enqueue(reqs);
    echoQueue.enqueue(echo);
    // 如果当前没有正在发送的请求，则直接发送
    if (curReq.isEmpty()) {
        nextRequest();
    }
}

void MachineCommTCP::clearRequest_p(const SignalEcho &echo)
{
    for (int i = echoQueue.size() - 1; i >= 0; i--) {
        if (echoQueue.at(i) == echo) {
            echom.echoEmpty(echo, reqsQueue.at(i).size());
            echoQueue.removeAt(i);
            reqsQueue.removeAt(i);
        }
    }
    if (curReqi != -1 && curEcho == echo) {
        echom.echoEmpty(curEcho, curReq.size() - curReqi);
        sendClearRequest();
        nextRequest();
    }
}

void MachineCommTCP::sendClearRequest()
{
    char data[7];
    data[0] = workidSend >> 8;
    data[1] = workidSend >> 0;
    workidSend++;
    data[2] = 0;
    data[3] = 2;
    data[4] = 0;
    data[5] = 1;
    data[6] = 0;
    socket->write(data, 7);
}

// 发送队列里的下一个请求
void MachineCommTCP::nextRequest()
{
    curReq.clear();
    curEcho = SignalEcho();
    curReqi = -1;
    if (reqsQueue.isEmpty())
        return;

    curReq = reqsQueue.dequeue();
    curEcho = echoQueue.dequeue();

    sendRequest();
}

void MachineCommTCP::sendRequest()
{
    curReqi++;
    if (curReqi >= curReq.size()) {
        nextRequest();
        return;
    }

    if (curReqi == 0) {
        workidRecv = workidSend;
        QByteArray ba;
        int len = 0;
        for (int i = 0; i < curReq.size(); i++) {
            const MCRequest &req = curReq.at(i);
            len += req.data.length() + 6;
        }

        ba.resize(len);
        char *data = ba.data();
        len = 0;
        for (int i = 0; i < curReq.size(); i++) {
            const MCRequest &req = curReq.at(i);
            data[0+len] = workidSend >> 8;
            data[1+len] = workidSend >> 0;
            workidSend++;
            int toms = (req.timeoutms + 199) / 200;
            data[2+len] = toms;
            data[3+len] = (i != curReq.size() - 1);
            int rscnt = req.resendcnt;
            if (rscnt < 0) rscnt = 0;
            else if (rscnt > 15) rscnt = 15;
            data[3+len] |= rscnt << 4;
            int reqlen = req.data.length();
            data[4+len] = reqlen >> 8;
            data[5+len] = reqlen >> 0;

            memcpy(&data[6+len], req.data.constData(), req.data.length());
            len += req.data.length() + 6;
        }

        if (socketIsConnected) {
            socket->write(ba);
            MC_Debug(">>", ba);
        }
        else {
            MC_Debug(">>", ba);
            response(-1);
            return;
        }
    }
    else
        workidRecv++;

    const MCRequest &req = curReq.at(curReqi);
    rsplen = req.rsplen;
    int toms = (req.timeoutms + 199) / 200;
    int timeoutms = toms * 200 + 10 * 1000;
    timerTimeout.start(timeoutms);
}

// err: 0：无错误，-1：超时，其他：异常码
void MachineCommTCP::response(int err, int len)
{
#ifdef MC_Print
    if (err == -1)
        MC_Warning("timeout", QByteArray());
#endif
    QByteArray data;
    if (err != -1)
        data = recv.mid(6, len);
    timerTimeout.stop();

    MCRequest req = curReq.at(curReqi);
    MCResponse rsp;
    rsp.data = data;

    if (curEcho.isValid())
        echom.echo(curEcho,
                   Q_ARG(int, err),
                   Q_ARG_MCRequest(req),
                   Q_ARG_MCResponse(rsp),
                   Q_ARG_SignalEcho(curEcho));

    if (err == 0)
        sendRequest();
    else {
        if (curEcho.isValid())
            echom.echoEmpty(curEcho, curReq.size() - curReqi - 1);
        nextRequest();
    }
}

void MachineCommTCP::onReadyRead()
{
    recv.append(socket->readAll());
    Next:

    // 若长度不够，则继续等待
    if (recv.size() < 6)
        return;

    // 若数据长度不够，则继续等待
    const uchar *d = (const uchar *)recv.constData();
    int len = ((uint)d[4] << 8) | ((uint)d[5]);
    if (recv.size() < len+6)
        return;

    // 若协议不支持，则丢弃该帧
    int proto = d[3] & 0x0F;
    if (proto != 0 && proto != 1 && proto != 2) {
        MC_Warning("x ", recv.left(len+6));
        recv.remove(0, len+6);
        goto Next;
    }

    // 若当前没有请求，或事务id对不上，则丢弃该帧
    quint16 wid = ((quint16)d[0] << 8) | ((quint16)d[1]);
    if (curReqi == -1 || wid != workidRecv) {
        MC_Warning("x ", recv.left(len+6));
        recv.remove(0, len+6);
        goto Next;
    }

    // 若收到转发异常帧
    if (proto == 2) {
        if (len == 1)
            response(-1);
        recv.remove(0, len+6);
        goto Next;
    }

    // 若收到异常帧
    const int errflen = 3;
    if (len == errflen && d[1+6] > (uchar)0x80) {
        MC_Debug("< ", recv.left(len+6));
        uchar err = d[2+6];
#ifdef MYOPT_ModbusTcpServer_V1
        response((err == 0xFF) ? -1 : err, len);
#else
        response(err);
#endif
        recv.remove(0, len+6);
        goto Next;
    }

    // 若长度不符合预期，则丢弃该帧
    if (len != rsplen) {
        MC_Warning("x ", recv.left(len+6));
        recv.remove(0, len+6);
        goto Next;
    }

    // 收到正常帧
    MC_Debug("< ", recv.left(len+6));
    response(0, len);
    recv.remove(0, len+6);
    goto Next;
}

void MachineCommTCP::onTimeout()
{
#ifndef MYOPT_MCTCP_S
    onSocketDisconnected();
#else
    recv.clear();
    if (curReqi != -1)
        response(-1);
#endif
}

void MachineCommTCP::onSocketConnected()
{
    socketIsConnected = true;
    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    emit socketConnected();
}

void MachineCommTCP::onSocketDisconnected()
{
    if (socketIsReconnecting)
        return;
    socketIsReconnecting = true;

    socket->abort();

    if (socketIsConnected) {
        socketIsConnected = false;
        emit socketDisconnected();
    }

    recv.clear();
    if (curReqi != -1)
        response(-1);

    QTimer::singleShot(2000, this, SLOT(onSocketReconnect()));
}

void MachineCommTCP::onSocketReconnect()
{
    socketIsReconnecting = false;
    QString ip = socket->property("ip").toString();
    uint port = socket->property("port").toUInt();
    socket->connectToHost(ip, port);
}
#endif

MachineCommQueue::MachineCommQueue(MachineComm *mcomm, QObject *parent) :
    QObject(parent)
{
    mc = mcomm;
    mc->echom.create(m_echo, m_ecIds, this, "onResponse");
    curReqi = 0;
}

MachineCommQueue::~MachineCommQueue()
{
    mc->clearRequest(m_echo, m_ecIds);
    mc->echom.destroy(m_ecIds, this);
}

void MachineCommQueue::request(const MCRequest &req, const SignalEcho &echo)
{
    request(QList<MCRequest>() << req, echo);
}

void MachineCommQueue::request(const QList<MCRequest> &reqs, const SignalEcho &echo)
{
    if (reqs.isEmpty())
        return;

    if (echo.isValid())
        echom.from(echo, reqs.size());

    QMetaObject::invokeMethod(this, "newRequest",
                              Q_ARG_List_MCRequest(reqs),
                              Q_ARG_SignalEcho(echo));
}

void MachineCommQueue::clearRequest(SignalEcho &echo, SignalEchoIds &ids)
{
    if (!echo.isValid())
        return;

    // 稍后将请求队列中跟旧echo有关的项移除掉
    QMetaObject::invokeMethod(this, "clearRequest_p",
                              Q_ARG_SignalEcho(echo));
    // 重新获取echo，旧echo的回调将会无效
    echom.reCreate(echo, ids);
}

void MachineCommQueue::newRequest(const QList<MCRequest> &reqs, const SignalEcho &echo)
{
    if (curReq.isEmpty()) {
        curReq = reqs;
        curEcho = echo;
        mc->request(curReq, m_echo);
    }
    else {
        reqsQueue.enqueue(reqs);
        echoQueue.enqueue(echo);
    }
}

void MachineCommQueue::clearRequest_p(const SignalEcho &echo)
{
    for (int i = echoQueue.size() - 1; i >= 0; i--) {
        if (echoQueue.at(i) == echo) {
            echom.echoEmpty(echo, reqsQueue.at(i).size());
            echoQueue.removeAt(i);
            reqsQueue.removeAt(i);
        }
    }
    if (curReqi < curReq.size() && curEcho == echo) {
        echom.echoEmpty(curEcho, curReq.size() - curReqi);
        mc->clearRequest(m_echo, m_ecIds);
        nextRequest();
    }
}

void MachineCommQueue::nextRequest()
{
    curReq.clear();
    curEcho = SignalEcho();
    curReqi = 0;
    if (reqsQueue.isEmpty())
        return;

    curReq = reqsQueue.dequeue();
    curEcho = echoQueue.dequeue();
    mc->request(curReq, m_echo);
}

void MachineCommQueue::onResponse(int err,
                                  const MCRequest_zy1 &req,
                                  const MCResponse_zy1 &rsp,
                                  const SignalEcho_zy1 &echo)
{
    mc->echom.finish(echo);
    if (echo == m_echo) {
        curReqi++;
        if (curEcho.isValid())
            echom.echo(curEcho,
                       Q_ARG(int, err),
                       Q_ARG_MCRequest(req),
                       Q_ARG_MCResponse(rsp),
                       Q_ARG_SignalEcho(curEcho));

        if (err || curReqi >= curReq.size())
            nextRequest();
    }
}

MachineComm::~MachineComm()
{
}

#ifdef MC_Print
#include <QDebug>
void MachineComm_print(const char *prefix, const char *data, int size, bool warning)
{
    if (!warning)
        return;
    QByteArray ba(data, size);
    ba = ba.toHex();
    int len = ba.length() + ba.length() / 2;
    if (len <= 0) len = 1;
    char str[len];
    for (int i = 0, j = 0; i < ba.length(); i += 2, j += 3) {
        str[j] = ba[i];
        str[j + 1] = ba[i + 1];
        str[j + 2] = ' ';
    }
    str[len - 1] = 0;
    if (warning) qWarning() << prefix << str;
    else qDebug() << prefix << str;
}
#endif
