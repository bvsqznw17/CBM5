#include "ModbusTcpServer.h"
#ifdef QT_NETWORK_LIB

static int socket_set_keepalive (int fd, int idle = 10, int wsec = 10);

ModbusTcpServer::ModbusTcpServer(quint16 _port, MachineComm *_mc, QObject *parent) :
    QTcpServer(parent)
{
    isOnline = false;
    maxConnections = 1;
    maxConnections0 = 0;
    port = _port;
    mc = _mc;
    mctoms = 1;
}

ModbusTcpServer::~ModbusTcpServer()
{
    setIsOnline_inv(false);
}

void ModbusTcpServer::setIsOnline_inv(bool b)
{
    if (b == isOnline)
        return;
    isOnline = b;

    if (isOnline) {
        QTcpServer::listen(QHostAddress::Any, port);
    }
    else {
        QTcpServer::close();
        QList<QThread*> list = findChildren<QThread*>();
        foreach (QThread *t, list) {
            t->quit();
            t->wait();
            t->deleteLater();
        }
    }
}

void ModbusTcpServer::incomingConnection(qintptr handle)
{
    qDebug("incomingConnection, %d", maxConnections);
    if (maxConnections == 0) {
        QTcpSocket socket(this);
        socket.setSocketDescriptor(handle);
        return;
    }

    maxConnections--;
    maxConnections0++;
    emit connChanged(maxConnections0);

    QTcpSocket *socket = new QTcpSocket();
    socket->setSocketDescriptor(handle);
    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    ModbusTcpServer_Socket *ss = new ModbusTcpServer_Socket(socket, mc, mctoms);
    connect(ss->thd, SIGNAL(finished()), this, SLOT(onThreadFinished()));
    ss->uuid = uuid;
    ss->thd->setParent(this);
    ss->thd->start();
}

void ModbusTcpServer::onThreadFinished()
{
    qDebug("onThreadFinished");
    maxConnections++;
    maxConnections0--;
    emit connChanged(maxConnections0);
}

ModbusTcpServerC::ModbusTcpServerC(QString _ip, quint16 _port, MachineComm *_mc, QObject *parent) :
    QObject(parent)
{
    isOnline = false;
    connCnt = 0;
    ip = _ip;
    port = _port;
    mc = _mc;
    mctoms = 1;
    cthd = 0;
    sthd = 0;
}

ModbusTcpServerC::~ModbusTcpServerC()
{
    setIsOnline_inv(false);
}

void ModbusTcpServerC::setIsOnline_inv(bool b)
{
    if (b == isOnline)
        return;
    isOnline = b;

    if (isOnline) {
        cthd = new ModbusTcpServerC_Thread(this);
        cthd->start();
    }
    else {
        if (cthd) {
            cthd->stop();
            cthd->wait();
            cthd->deleteLater();
            cthd = 0;
        }
        if (sthd) {
            sthd->quit();
            sthd->wait();
            sthd->deleteLater();
            sthd = 0;
        }
    }
}

void ModbusTcpServerC::setIPPort(QString _ip, quint16 _port)
{
    ip = _ip;
    port = _port;
    if (isOnline) {
        setIsOnline_inv(false);
        setIsOnline_inv(true);
    }
}

int ModbusTcpServerC::getConnCnt()
{
    return connCnt;
}

void ModbusTcpServerC::onThreadBegin()
{
    qDebug("onThreadBegin");
    connCnt++;
    emit connChanged(connCnt);
}

void ModbusTcpServerC::onThreadFinished()
{
    qDebug("onThreadFinished");
    sthd = 0;
    cthd = 0;
    connCnt--;
    emit connChanged(connCnt);
    if (isOnline) {
        setIsOnline_inv(false);
        setIsOnline_inv(true);
    }
}

ModbusTcpServerC_Thread::ModbusTcpServerC_Thread(ModbusTcpServerC *parent) :
    QThread(parent)
{
    p = parent;
    m_run = true;
}

ModbusTcpServerC_Thread::~ModbusTcpServerC_Thread()
{
    stop();
    this->wait();
}

void ModbusTcpServerC_Thread::stop()
{
    m_run = false;
}

void ModbusTcpServerC_Thread::run()
{
    QTcpSocket *s = new QTcpSocket;
    s->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    while (1) {
        s->connectToHost(p->ip, p->port);
        if (s->waitForConnected(5000))
            break;
        if (!m_run) return;
        msleep(1000);
        if (!m_run) return;
    }
    ModbusTcpServer_Socket *ss = new ModbusTcpServer_Socket(s, p->mc, p->mctoms);
    connect(ss->thd, SIGNAL(finished()), p, SLOT(onThreadFinished()));
    connect(ss->thd, SIGNAL(finished()), this, SLOT(deleteLater()));
    ss->uuid = p->uuid;
    p->sthd = ss->thd;
    QTimer::singleShot(0, p, SLOT(onThreadBegin()));
    ss->thd->start();
}

ModbusTcpServer_Socket::ModbusTcpServer_Socket(QTcpSocket *s, MachineComm *_mc, int toms)
{
    socket = s;
    mc = _mc;
    mctoms = toms;
    socket->setParent(this);
    socket_set_keepalive(socket->socketDescriptor());
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(onDisconnected()));
    mc->echom.create(mcecho, mcecIds, this, "onResponse");
    isNeedRequest = false;
    isRequest = false;

    hearttimer = new QTimer(this);
    hearttimer->setInterval(10000);
    connect(hearttimer, SIGNAL(timeout()), this, SLOT(heartattack()));
    heartbeat1();

    thd = new QThread;
    connect(thd, SIGNAL(finished()), thd, SLOT(deleteLater()));
    connect(thd, SIGNAL(finished()), this, SLOT(deleteLater()));
    this->moveToThread(thd);
}

ModbusTcpServer_Socket::~ModbusTcpServer_Socket()
{
    mc->clearRequest(mcecho, mcecIds);
    mc->echom.destroy(mcecIds, this);
}

void ModbusTcpServer_Socket::clearRequest()
{
    reqs.clear();
    head.clear();
    invalidHead.clear();
    isNeedRequest = false;
    if (isRequest) {
        isRequest = false;
        mc->clearRequest(mcecho, mcecIds);
    }
}

void ModbusTcpServer_Socket::sendRequest()
{
    if (!reqs.isEmpty()) {
        isRequest = true;
        mc->request(reqs, mcecho);
        reqs.clear();
    }
    else {
        sendInvalidHead();
        invalidHead.clear();
    }
}

void ModbusTcpServer_Socket::sendInvalidHead()
{
#ifndef MYOPT_ModbusTcpServer_V1
    char *bh = (char *)invalidHead.data();
    char ba[1];
    bh[2] = 0x00;
    bh[3] = 0x02;
    bh[4] = 0;
    bh[5] = 1;
    ba[0] = 0;
    socket->write(bh, 6);
    socket->write(ba, 1);
#endif
}

void ModbusTcpServer_Socket::onDisconnected()
{
    qDebug("onDisconnected");
    thd->quit();
}

void ModbusTcpServer_Socket::onReadyRead()
{
    qDebug("onReadyRead");
    heartbeat2();
    recv.append(socket->readAll());
    qDebug() << recv.toHex();

    while (1) {

    // 若长度不够，则继续等待
    if (recv.size() < 6)
        break;

    // 若数据长度不够，则继续等待
    const uchar *d = (const uchar *)recv.constData();
    int len = ((uint)d[4] << 8) | ((uint)d[5]);
    if (recv.size() < len+6)
        break;

    // 若协议不支持，则丢弃该帧
    uint proto = d[3] & 0x0F;
    if (proto > 3) {
        qDebug("##r proto err");
        recv.remove(0, len+6);
        continue;
    }

    // 若协议为2，则停止并清除之前的请求，并丢弃该帧
    if (proto == 2) {
        qDebug("##r proto == 2");
        if (len == 1 && d[0+6] == 0)
            clearRequest();
        recv.remove(0, len+6);
        continue;
    }

    // 若协议为3
    if (proto == 3) {
        qDebug("##r proto == 3");
        if (len == 1 && d[0+6] == 0) {
            uchar *d = (uchar *)recv.data();
            d[2] = 0;
            d[3] = 3;
            d[4] = 0;
            d[5] = uuid.size() + 1;
            socket->write((char *)d, 7);
            socket->write(uuid);
        }
        recv.remove(0, len+6);
        continue;
    }

    // 开始处理请求帧
    MCRequest req;
    if (len >= 2) {
        int toms = d[2];
        if (toms == 0 || toms > 50)
            toms = mctoms;
        req.timeoutms = 200 * toms;
        req.resendcnt = d[3] >> 4;
        req.rsplen = 0;

        if (d[1+6] == 0x03) {
            if (len == 6) {
                int n = ((uint)d[4+6] << 8) | ((uint)d[5+6]);
                req.rsplen = 3 + n*2;
                req.data = recv.mid(6, len);
            }
        }
        else if (d[1+6] == 0x10) {
            if (len >= 7 && len <= 256) {
                req.rsplen = 6;
                req.data = recv.mid(6, len);
            }
        }
    }

    // 若正在请求，则先停止并清除之前的请求
    if (isRequest || isNeedRequest) {
        qDebug("##r isRequest");
        clearRequest();
    }

    // 若data为空，则无法处理，将忽略此请求和后面的“连续请求”
    if (invalidHead.isEmpty() && req.data.isEmpty()) {
        qDebug("##r req.data.isEmpty()");
        invalidHead = recv.left(6);
    }

    // 先将请求缓存
    if (invalidHead.isEmpty() && reqs.size() < 1000) {
        reqs.append(req);
        head.enqueue(recv.left(6));
    }

    recv.remove(0, len+6);

    // 若协议为0，则发送请求。否则协议为1，等待“连续请求”
    if (proto == 0)
        isNeedRequest = true;

    } // while

    if (isNeedRequest) {
        sendRequest();
        isNeedRequest = false;
    }
    else
        heartbeat1();
}

void ModbusTcpServer_Socket::onResponse(int err,
                                 const MCRequest &req,
                                 const MCResponse &rsp,
                                 const SignalEcho &echo)
{
    mc->echom.finish(echo);
    if (echo != mcecho || socket->state() != QAbstractSocket::ConnectedState)
        return;

    Q_UNUSED(req);
    QByteArray bh = head.dequeue();
    QByteArray ba;

    if (err != -1) {
        ba = rsp.data;
        quint16 len = ba.size();
        bh[2] = 0x00;
        bh[3] = bh.at(3) & 0x0F;
        bh[4] = len >> 8;
        bh[5] = len >> 0;
    }
    else {
#ifdef MYOPT_ModbusTcpServer_V1
        bh[2] = 0x00;
        bh[3] = bh.at(3) & 0x0F;
        bh[4] = 0;
        bh[5] = 3;
        ba.resize(3);
        ba[0] = req.data.at(0);
        ba[1] = req.data.at(1) | (uchar)0x80;
        ba[2] = err;
#else
        bh[2] = 0x00;
        bh[3] = 0x02;
        bh[4] = 0;
        bh[5] = 1;
        ba.resize(1);
        ba[0] = 0;
#endif
        head.clear();
        invalidHead.clear();
    }
    socket->write(bh);
    socket->write(ba);
    qDebug() << bh.toHex() << ba.toHex();

    if (head.isEmpty()) {
        isRequest = false;
        if (!invalidHead.isEmpty()) {
            sendInvalidHead();
            invalidHead.clear();
        }
        heartbeat1();
    }
}

void ModbusTcpServer_Socket::heartbeat1()
{
    //hearttimer->start();
}

void ModbusTcpServer_Socket::heartbeat2()
{
    hearttimer->stop();
}

void ModbusTcpServer_Socket::heartattack()
{
    socket->abort();
}

#ifdef Q_OS_WIN
#include <winsock2.h>
struct  tcp_keepalive  {
    u_long    onoff;
    u_long    keepalivetime;
    u_long    keepaliveinterval;
};
#define  SIO_RCVALL                  _WSAIOW(IOC_VENDOR,1)
#define  SIO_RCVALL_MCAST            _WSAIOW(IOC_VENDOR,2)
#define  SIO_RCVALL_IGMPMCAST        _WSAIOW(IOC_VENDOR,3)
#define  SIO_KEEPALIVE_VALS          _WSAIOW(IOC_VENDOR,4)
#define  SIO_ABSORB_RTRALERT          _WSAIOW(IOC_VENDOR,5)
#define  SIO_UCAST_IF                _WSAIOW(IOC_VENDOR,6)
#define  SIO_LIMIT_BROADCASTS        _WSAIOW(IOC_VENDOR,7)
#define  SIO_INDEX_BIND              _WSAIOW(IOC_VENDOR,8)
#define  SIO_INDEX_MCASTIF            _WSAIOW(IOC_VENDOR,9)
#define  SIO_INDEX_ADD_MCAST          _WSAIOW(IOC_VENDOR,10)
#define  SIO_INDEX_DEL_MCAST          _WSAIOW(IOC_VENDOR,11)
//上面的结构体和define是#include <mstcpip.h> 的内容

static int socket_set_keepalive (int fd, int idle, int wsec)
{
    struct tcp_keepalive kavars;
    struct tcp_keepalive alive_out;
    kavars.onoff = TRUE;
    kavars.keepalivetime = idle*1000;
    kavars.keepaliveinterval = wsec*1000/10;

    /* Set: use keepalive on fd */
    int alive = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (const char *) &alive,sizeof alive) != 0)
    {
        qDebug("socket_set_keepalive failed");
        return -1;
    }

    unsigned long ulBytesReturn = 0;
    WSAIoctl(fd, SIO_KEEPALIVE_VALS, &kavars, sizeof(kavars),&alive_out, sizeof(alive_out), &ulBytesReturn, NULL, NULL);
    return 0;
}
#else
#include <netinet/tcp.h>
static int socket_set_keepalive (int fd, int idle, int wsec)
{
    int keepAlive = 1;    // 非0值，开启keepalive属性
    int keepIdle = idle;    // 如该连接在N秒内没有任何数据往来,则进行此TCP层的探测
    int keepInterval = wsec; // 探测发包间隔为N秒
    int keepCount = 1;        // 尝试探测的最多N次数

    setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepAlive, sizeof(keepAlive));
    setsockopt(fd, SOL_TCP, TCP_KEEPIDLE, (void *)&keepIdle, sizeof(keepIdle));
    setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval));
    setsockopt(fd, SOL_TCP, TCP_KEEPCNT, (void *)&keepCount, sizeof(keepCount));
    return 0;
}
#endif

#endif
