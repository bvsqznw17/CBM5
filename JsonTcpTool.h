#ifndef JSONTCPTOOL
#define JSONTCPTOOL

#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

extern class JsonTcpTool jsonTcpTool;

class JsonTcpTool
{
public:
    JsonTcpTool();
    JsonTcpTool(const JsonTcpTool &jtt)
    { memcpy(this, &jtt, sizeof(JsonTcpTool)); }
    static bool interruptAll;
    bool interrupt;
    qint64 wms;
    int wms1;
    int json_maxsize;
    int array_maxsize;

    static QString getRandom();
    static QByteArray md5(const QByteArray &data);
    static QByteArray md5(const QByteArray &data, const QByteArray &key);
    bool waitForReadJson(QTcpSocket *socket, QByteArray &data);
    bool waitForReadJson(QTcpSocket *socket, QByteArray &data, QByteArray &data2);
    bool waitForReadArray(QTcpSocket *socket, QByteArray &data, QByteArray &data2);
    static void writeArray(QTcpSocket *socket, const QByteArray &data);
    bool waitForBytesWritten(QTcpSocket *socket);
    bool waitForDisconnected(QTcpSocket *socket);
    bool waitForConnected(QTcpSocket *socket);
    static void writeJsonResp(QTcpSocket *socket, const QByteArray &key,
                              const QString &random, const QString &myRandom,
                              int error, const QString &message,
                              const QString &fun, const QJsonObject &content = QJsonObject());
    static void writeJsonReq(QTcpSocket *socket, const QByteArray &key,
                             const QString &random, const QString &myRandom,
                             const QString &fun, const QJsonObject &content = QJsonObject());
};
class JsonTcpSocket_Delete
{
public:
    JsonTcpSocket_Delete(QTcpSocket *socket);
    ~JsonTcpSocket_Delete();
    QTcpSocket *s;
};

int socket_set_keepalive (int fd, int idle = 10, int wsec = 10);

#endif // JSONTCPTOOL
