#ifndef MODBUSTCPSERVER_H
#define MODBUSTCPSERVER_H

#ifdef QT_NETWORK_LIB
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QUuid>
#include "MachineComm.h"

class ModbusTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit ModbusTcpServer(quint16 _port, MachineComm *_mc, QObject *parent = 0);
    ~ModbusTcpServer();
    int mctoms;
    QByteArray uuid;

    inline void setIsOnline(bool b)
    { QMetaObject::invokeMethod(this, "setIsOnline_inv", Q_ARG(bool, b)); }
    Q_INVOKABLE void setIsOnline_inv(bool b);

    inline int getConnCnt() { return maxConnections0; }

private:
    void incomingConnection(qintptr handle);
    Q_SLOT void onThreadFinished();
    Q_SIGNAL void connChanged(int n);

private:
    bool isOnline;
    int maxConnections;
    int maxConnections0;
    quint16 port;
    MachineComm *mc;
};

class ModbusTcpServerC_Thread;
class ModbusTcpServerC : public QObject
{
    Q_OBJECT
    friend class ModbusTcpServerC_Thread;
public:
    ModbusTcpServerC(QString _ip, quint16 _port, MachineComm *_mc, QObject *parent = 0);
    ~ModbusTcpServerC();
    int mctoms;
    QByteArray uuid;

    inline void setIsOnline(bool b)
    { QMetaObject::invokeMethod(this, "setIsOnline_inv", Q_ARG(bool, b)); }
    Q_INVOKABLE void setIsOnline_inv(bool b);

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
    MachineComm *mc;
    ModbusTcpServerC_Thread *cthd;
    QThread *sthd;
};

class ModbusTcpServerC_Thread : public QThread
{
    Q_OBJECT
public:
    ModbusTcpServerC_Thread(ModbusTcpServerC *parent);
    ~ModbusTcpServerC_Thread();
    void stop();
    bool m_run;
private:
    ModbusTcpServerC *p;
    void run();
};

class ModbusTcpServer_Socket : public QObject
{
    Q_OBJECT
public:
    ModbusTcpServer_Socket(QTcpSocket *s, MachineComm *_mc, int toms);
    ~ModbusTcpServer_Socket();

    QByteArray uuid;
    QThread *thd;

private:
    void clearRequest();
    void sendRequest();
    void sendInvalidHead();

private slots:
    void onDisconnected();
    void onReadyRead();
    void onResponse(int err,
                    const MCRequest_zy1 &req,
                    const MCResponse_zy1 &rsp,
                    const SignalEcho_zy1 &echo);

private:
    QTimer *hearttimer;
    void heartbeat1();
    void heartbeat2();
    Q_SLOT void heartattack();

private:
    QTcpSocket *socket;
    MachineComm *mc;
    int mctoms;

    QByteArray recv;
    QList<MCRequest> reqs;
    QQueue<QByteArray> head;
    QByteArray invalidHead;
    SignalEcho mcecho;
    SignalEchoIds mcecIds;
    bool isNeedRequest;
    bool isRequest;
};
#else
#include <QObject>
class ModbusTcpServer : public QObject
{
    Q_OBJECT
};
class ModbusTcpServerC : public QObject
{
    Q_OBJECT
};
#endif

#endif // MODBUSTCPSERVER_H
