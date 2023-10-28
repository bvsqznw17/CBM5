#ifndef JSONTCPSERVER_H
#define JSONTCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QMutex>
#include <functional>
#include "JsonTcpTool.h"

class JsonTcpServer_Socket;
typedef std::function<bool(JsonTcpServer_Socket&)>
JsonTcpServer_Func;
typedef QMap<QString, JsonTcpServer_Func>
JsonTcpServer_FuncMap;

class JsonTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    QByteArray secretKey;
    explicit JsonTcpServer(quint16 _port, QObject *parent = 0);
    ~JsonTcpServer();

    inline void setIsOnline(bool b)
    { QMetaObject::invokeMethod(this, "setIsOnline_inv", Q_ARG(bool, b)); }
    Q_INVOKABLE void setIsOnline_inv(bool b);

    void setFuncMap(const JsonTcpServer_FuncMap &map);

private:
    void incomingConnection(qintptr handle);
    Q_SLOT void onThreadFinished();

private:
    bool isOnline;
    int maxConnections;
    quint16 port;
    JsonTcpServer_FuncMap funcMap;
};

class JsonTcpServerC_Thread;
class JsonTcpServerC : public QObject
{
    Q_OBJECT
    friend class JsonTcpServerC_Thread;
public:
    QByteArray secretKey;
    JsonTcpServerC(QString _ip, quint16 _port, QObject *parent = 0);
    ~JsonTcpServerC();

    inline void setIsOnline(bool b)
    { QMetaObject::invokeMethod(this, "setIsOnline_inv", Q_ARG(bool, b)); }
    Q_INVOKABLE void setIsOnline_inv(bool b);

    void setFuncMap(const JsonTcpServer_FuncMap &map);

    void setIPPort(QString _ip, quint16 _port);

    int getConnCnt();

private:
    Q_SLOT void onThreadBegin();
    Q_SLOT void onThreadFinished();
    Q_SIGNAL void connChanged(int n);

private:
    bool isOnline;
    int connCnt;
    QString ip;
    quint16 port;
    JsonTcpServerC_Thread *cthd;
    JsonTcpServer_FuncMap funcMap;
};

class JsonTcpServerC_Thread : public QThread
{
    Q_OBJECT
public:
    JsonTcpServerC_Thread(JsonTcpServerC *parent);
    ~JsonTcpServerC_Thread();
    void stop();
    bool m_run;
private:
    JsonTcpServerC *p;
    JsonTcpServer_Socket *ss;
    QMutex mutex;
    void run();
};

class JsonTcpServer_Socket
{
public:
    JsonTcpServer_FuncMap funcMap;
    QTcpSocket *socket;
    QByteArray key;
    QString random;
    QString myRandom;

    QString fun;
    QJsonObject content;
    JsonTcpTool jtt;

    void stop();
    void process();

    inline void writeJsonRespErr(int error, const QString &message)
    { jsonTcpTool.writeJsonResp(socket, key, random, "", error, message, fun);
      jsonTcpTool.waitForDisconnected(socket); }
    inline void writeJsonResp(int error, const QString &message, const QJsonObject &content = QJsonObject())
    { myRandom = jtt.getRandom();
      jtt.writeJsonResp(socket, key, random, myRandom, error, message, fun, content); }
};

class JsonTcpServer_SocketThread : public QThread
{
    Q_OBJECT
public:
    qintptr handle;
    JsonTcpServer_Socket jtss;

    explicit JsonTcpServer_SocketThread(QObject *parent = 0);
    ~JsonTcpServer_SocketThread();
private:
    void run();
};

#endif // JSONTCPSERVER_H
