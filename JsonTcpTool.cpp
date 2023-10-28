#include "JsonTcpTool.h"
#include "Tool.h"
#include <QDateTime>
#include <QCryptographicHash>
#include <QElapsedTimer>

JsonTcpTool jsonTcpTool;
bool JsonTcpTool::interruptAll = false;

JsonTcpTool::JsonTcpTool()
{
    interrupt = false;
    wms = 15*1000;
    wms1 = 1*1000;
    json_maxsize = 10240;
    array_maxsize = 10240;
}

QString JsonTcpTool::getRandom()
{
    qint64 ms = Tool::currentDateTime().toMSecsSinceEpoch();
    return QString::number(ms);
}

QByteArray JsonTcpTool::md5(const QByteArray &data)
{
    return QCryptographicHash::hash(data, QCryptographicHash::Md5).toHex();
}

QByteArray JsonTcpTool::md5(const QByteArray &data, const QByteArray &key)
{
    return QCryptographicHash::hash(QByteArray(data).append(key),
                                    QCryptographicHash::Md5).toHex();
}

bool JsonTcpTool::waitForReadJson(QTcpSocket *socket, QByteArray &data)
{
    QElapsedTimer etimer; etimer.start();
    qint64 elapsed = 0;
    qint64 wms0 = 0;
    int ds = data.size();

    while (elapsed-wms0 < wms && !interruptAll && !interrupt) {
        if (socket->waitForReadyRead(wms1)) {
            data.append(socket->readAll());
            if (data.size() > json_maxsize)
                return false;
            if (data.size() >= 2 && data.at(data.size()-1) == 0 && data.at(data.size()-2) == 0) {
                //qDebug("size: %d %s", data.size(), data.constData());
                return true;
            }
            elapsed = etimer.elapsed();
            wms0 = elapsed;
        }
        else if (socket->state() == QAbstractSocket::UnconnectedState ||
                 socket->error() != QAbstractSocket::SocketTimeoutError)
            return false;
        else
            elapsed = etimer.elapsed();

        if (elapsed >= wms && (data.size() - ds) / elapsed < 1) // 速度<1KB/s
            return false;
    }
    return false;
}

// 读到的数据追加到data，若读完一个Json，则放到data，剩余的放到data2
bool JsonTcpTool::waitForReadJson(QTcpSocket *socket, QByteArray &data, QByteArray &data2)
{
    QElapsedTimer etimer; etimer.start();
    qint64 elapsed = 0;
    qint64 wms0 = 0;
    int ds = data.size();
    int index = 1;

    while (elapsed-wms0 < wms && !interruptAll && !interrupt) {
        if (socket->waitForReadyRead(wms1)) {
            data.append(socket->readAll());
            for (; index < data.size(); index++)
                if (data.at(index) == 0 && data.at(index-1) == 0) {
                    index++;
                    data2 = data.mid(index, -1);
                    data.remove(index, data.size() - index);
                    //qDebug("size: %d %s", data.size(), data.constData());
                    return true;
                }
            if (index > json_maxsize)
                return false;
            elapsed = etimer.elapsed();
            wms0 = elapsed;
        }
        else if (socket->state() == QAbstractSocket::UnconnectedState ||
                 socket->error() != QAbstractSocket::SocketTimeoutError)
            return false;
        else
            elapsed = etimer.elapsed();
        if (elapsed >= wms && (data.size() - ds) / elapsed < 1) // 速度<1KB/s
            return false;
    }
    return false;
}

// 读到的数据追加到data，若读完一个QByteArray，则放到data2，剩余的放到data。
bool JsonTcpTool::waitForReadArray(QTcpSocket *socket, QByteArray &data, QByteArray &data2)
{
    QElapsedTimer etimer; etimer.start();
    qint64 elapsed = 0;
    qint64 wms0 = 0;
    int ds = data.size();
    int size = 0;

    data2.clear();

    do {
        if (size == 0) {
            if (data.size() >= 4) {
                int i0 = data.at(0);
                int i1 = data.at(1);
                int i2 = data.at(2);
                int i3 = data.at(3);
                size |= (i0 << 24) & 0xFF000000;
                size |= (i1 << 16) & 0x00FF0000;
                size |= (i2 <<  8) & 0x0000FF00;
                size |= (i3 <<  0) & 0x000000FF;
                if (size > array_maxsize)
                    return false;
                if (data.size() >= 4 + size) {
                    data2 = data.mid(4, size);
                    data.remove(0, 4 + size);
                    return true;
                }
            }
        }
        else if (data.size() >= 4 + size) {
            data2 = data.mid(4, size);
            data.remove(0, 4 + size);
            return true;
        }
        if (socket->waitForReadyRead(wms1)) {
            data.append(socket->readAll());
            elapsed = etimer.elapsed();
            wms0 = elapsed;
        }
        else if (socket->state() == QAbstractSocket::UnconnectedState ||
                 socket->error() != QAbstractSocket::SocketTimeoutError)
            return false;
        else
            elapsed = etimer.elapsed();

        if (elapsed >= wms && (data.size() - ds) / elapsed < 1) // 速度<1KB/s
            return false;
    } while (elapsed-wms0 < wms && !interruptAll && !interrupt);

    return false;
}

void JsonTcpTool::writeArray(QTcpSocket *socket, const QByteArray &data)
{
    int size = data.size();
    char i[4];
    i[0] = size >> 24;
    i[1] = size >> 16;
    i[2] = size >> 8;
    i[3] = size >> 0;
    socket->write(i, 4);
    socket->write(data);
}

bool JsonTcpTool::waitForBytesWritten(QTcpSocket *socket)
{
    qint64 ds = socket->bytesToWrite();
    if (ds == 0)
        return true;

    QElapsedTimer etimer; etimer.start();
    qint64 elapsed = 0;
    qint64 wms0 = 0;

    while (elapsed-wms0 < wms && !interruptAll && !interrupt) {
        bool b = socket->waitForBytesWritten(wms1);
        qint64 n = socket->bytesToWrite();
        if (n == 0)
            return true;

        elapsed = etimer.restart();
        if (elapsed >= wms && (ds - n) / elapsed < 1) // 速度<1KB/s
            return false;

        if (b)
            wms0 = elapsed;
        else if (socket->state() == QAbstractSocket::UnconnectedState ||
                 socket->error() != QAbstractSocket::SocketTimeoutError)
            return false;
    }
    return false;
}

bool JsonTcpTool::waitForDisconnected(QTcpSocket *socket)
{
    if (!waitForBytesWritten(socket))
        return false;

    QElapsedTimer etimer; etimer.start();

    socket->disconnectFromHost();
    do {
        if (socket->state() == QAbstractSocket::UnconnectedState ||
                socket->waitForDisconnected(wms1))
            return true;
        else if (socket->error() != QAbstractSocket::SocketTimeoutError)
            return false;
    } while (etimer.elapsed() < wms && !interruptAll && !interrupt);
    return false;
}

bool JsonTcpTool::waitForConnected(QTcpSocket *socket)
{
    return socket->waitForConnected(5000);
}

void JsonTcpTool::writeJsonResp(QTcpSocket *socket, const QByteArray &key,
                                const QString &random, const QString &myRandom,
                                int error, const QString &message,
                                const QString &fun, const QJsonObject &content)
{
    QJsonObject jo;
    jo.insert("z_content", content);
    jo.insert("fun", fun);
    jo.insert("error", error);
    jo.insert("message", message);
    jo.insert("random", random);
    jo.insert("myRandom", myRandom);

    QByteArray json = QJsonDocument(jo).toJson();
    QByteArray data;
    data.reserve(json.size() + 1 + 32 + 2);
    data.append(json).append(key);
    QByteArray md5 = JsonTcpTool::md5(data);
    data.resize(json.size() + 1 + 32 + 2);
    char *d = data.data() + json.size();
    *d++ = '\0';
    memcpy(d, md5.constData(), 32); d += 32;
    *d++ = '\0'; *d++ = '\0';
    socket->write(data);
}

void JsonTcpTool::writeJsonReq(QTcpSocket *socket, const QByteArray &key,
                               const QString &random, const QString &myRandom,
                               const QString &fun, const QJsonObject &content)
{
    QJsonObject jo;
    jo.insert("z_content", content);
    jo.insert("fun", fun);
    jo.insert("random", random);
    jo.insert("myRandom", myRandom);

    QByteArray json = QJsonDocument(jo).toJson();
    QByteArray data;
    data.reserve(json.size() + 1 + 32 + 2);
    data.append(json).append(key);
    QByteArray md5 = JsonTcpTool::md5(data);
    data.resize(json.size() + 1 + 32 + 2);
    char *d = data.data() + json.size();
    *d++ = '\0';
    memcpy(d, md5.constData(), 32); d += 32;
    *d++ = '\0'; *d++ = '\0';
    socket->write(data);
}

JsonTcpSocket_Delete::JsonTcpSocket_Delete(QTcpSocket *socket) :
    s(socket)
{
}

JsonTcpSocket_Delete::~JsonTcpSocket_Delete()
{
    delete s;
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

int socket_set_keepalive (int fd, int idle, int wsec)
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
int socket_set_keepalive (int fd, int idle, int wsec)
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
