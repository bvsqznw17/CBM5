#include "JsonTcpServer.h"

JsonTcpServer::JsonTcpServer(quint16 _port, QObject *parent) :
    QTcpServer(parent)
{
    secretKey = "sk#`~`#CBM5";
    isOnline = false;
    port = _port;
    maxConnections = 1;
}

JsonTcpServer::~JsonTcpServer()
{
    setIsOnline_inv(false);
}

void JsonTcpServer::setIsOnline_inv(bool b)
{
    if (b == isOnline)
        return;
    isOnline = b;

    if (isOnline) {
        QTcpServer::listen(QHostAddress::Any, port);
    }
    else {
        QTcpServer::close();
        QList<JsonTcpServer_SocketThread *> list;
        list = findChildren<JsonTcpServer_SocketThread *>();
        foreach (JsonTcpServer_SocketThread *t, list) {
            t->deleteLater();
        }
    }
}

void JsonTcpServer::setFuncMap(const JsonTcpServer_FuncMap &map)
{
    funcMap = map;
}

void JsonTcpServer::incomingConnection(qintptr handle)
{
    qDebug("incomingConnection, %d", maxConnections);
    if (maxConnections == 0) {
        QTcpSocket socket(this);
        socket.setSocketDescriptor(handle);
        return;
    }

    maxConnections--;
    JsonTcpServer_SocketThread *thd = new JsonTcpServer_SocketThread(this);
    connect(thd, SIGNAL(finished()), thd, SLOT(deleteLater()));
    connect(thd, SIGNAL(finished()), this, SLOT(onThreadFinished()));

    thd->handle = handle;
    thd->jtss.funcMap = funcMap;
    thd->jtss.key = secretKey;
    thd->start();
}

void JsonTcpServer::onThreadFinished()
{
    qDebug("onThreadFinished");
    maxConnections++;
}


JsonTcpServerC::JsonTcpServerC(QString _ip, quint16 _port, QObject *parent) :
    QObject(parent)
{
    secretKey = "sk#`~`#CBM5";
    isOnline = false;
    connCnt = 0;
    ip = _ip;
    port = _port;
    cthd = 0;
}

JsonTcpServerC::~JsonTcpServerC()
{
    setIsOnline_inv(false);
}

void JsonTcpServerC::setIsOnline_inv(bool b)
{
    if (b == isOnline)
        return;
    isOnline = b;

    if (isOnline) {
        cthd = new JsonTcpServerC_Thread(this);
        cthd->start();
    }
    else {
        if (cthd) {
            cthd->stop();
            cthd->wait();
            cthd->deleteLater();
            cthd = 0;
        }
    }
}

void JsonTcpServerC::setFuncMap(const JsonTcpServer_FuncMap &map)
{
    funcMap = map;
}

void JsonTcpServerC::setIPPort(QString _ip, quint16 _port)
{
    ip = _ip;
    port = _port;
    if (isOnline) {
        setIsOnline_inv(false);
        setIsOnline_inv(true);
    }
}

int JsonTcpServerC::getConnCnt()
{
    return connCnt;
}

void JsonTcpServerC::onThreadBegin()
{
    qDebug("onThreadBegin");
    connCnt++;
    emit connChanged(connCnt);
}

void JsonTcpServerC::onThreadFinished()
{
    qDebug("onThreadFinished");
    connCnt--;
    emit connChanged(connCnt);
}


JsonTcpServerC_Thread::JsonTcpServerC_Thread(JsonTcpServerC *parent) :
    QThread(parent)
{
    m_run = true;
    p = parent;
    ss = new JsonTcpServer_Socket();
}

JsonTcpServerC_Thread::~JsonTcpServerC_Thread()
{
    stop();
    this->wait();
    delete ss;
}

void JsonTcpServerC_Thread::stop()
{
    QMutexLocker locker(&mutex);
    m_run = false;
    ss->stop();
}

void JsonTcpServerC_Thread::run()
{
    QTcpSocket *s = new QTcpSocket;
    s->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    while (1) {
        s->connectToHost(p->ip, p->port);
        if (s->waitForConnected(5000)) {
            QMutexLocker locker(&mutex);
            if (ss->jtt.interrupt || !m_run)
                break;
            ss->jtt.interrupt = false;
            locker.unlock();

            socket_set_keepalive(s->socketDescriptor());
            ss->funcMap = p->funcMap;
            ss->key = p->secretKey;
            ss->socket = s;
            QMetaObject::invokeMethod(p, "onThreadBegin");
            ss->process();
            QMetaObject::invokeMethod(p, "onThreadFinished");
        }
        if (!m_run) return;
        msleep(1000);
        if (!m_run) return;
    }
    s->deleteLater();
}


void JsonTcpServer_Socket::stop()
{
    jtt.interrupt = true;
}

void JsonTcpServer_Socket::process()
{
    jtt.wms = LONG_LONG_MAX;

    // 1.发送随机值
    myRandom = jtt.getRandom();
    socket->write(myRandom.toUtf8().append("\0\0", 2));

    while (1) {
        // 2.读取请求
        QByteArray data;
        if (!jtt.waitForReadJson(socket, data))
            return;
        int len = strlen(data.constData());
        int len2 = strlen(data.constData() + len + 1);
        QByteArray json = QByteArray::fromRawData(data.constData(), len);
        QByteArray md5 = QByteArray::fromRawData(data.constData() + len + 1, len2);
        if (md5.size() != 32 || md5.toLower() != jtt.md5(json, key)) {
            writeJsonRespErr(-1, "MD5签名错误");
            return;
        }

        QJsonObject jo = QJsonDocument::fromJson(json).object();
        data.clear();
        if (jo.isEmpty()) {
            writeJsonRespErr(-1, "JSON格式错误");
            return;
        }

        if (myRandom != jo.value("random").toString()) {
            writeJsonRespErr(-1, "random错误");
            return;
        }
        random = jo.value("myRandom").toString();

        // 3.处理请求
        fun = jo.value("fun").toString();
        content = jo.value("z_content").toObject();

        JsonTcpServer_FuncMap::const_iterator it =
                funcMap.constFind(fun);
        if (it == funcMap.constEnd()) {
            writeJsonResp(-1, "fun错误");
        }
        else {
            // 4.交给func处理
            const JsonTcpServer_Func &func = it.value();
            if (!func(*this))
                return;
        }

        fun.clear();
        content = QJsonObject();
    }
}


JsonTcpServer_SocketThread::JsonTcpServer_SocketThread(QObject *parent) :
    QThread(parent)
{
}

JsonTcpServer_SocketThread::~JsonTcpServer_SocketThread()
{
    jtss.stop();
    this->wait();
}

void JsonTcpServer_SocketThread::run()
{
    socket_set_keepalive(handle);
    QTcpSocket *socket = new QTcpSocket();
    socket->setSocketDescriptor(handle);
    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    JsonTcpSocket_Delete deleteSocket(socket);
    jtss.socket = socket;
    jtss.process();
}
