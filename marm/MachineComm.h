#ifndef MACHINECOMM_H
#define MACHINECOMM_H

#include <QObject>
#include <QIODevice>
#ifdef MYOPT_MCTCP
#include <QTcpSocket>
#endif
#include <QQueue>
#include <QTimer>
#include "SignalEcho.h"

// 请求帧
struct MCRequest {
    int timeoutms; // 超时时间
    int resendcnt; // 超时重发次数
    int rsplen; // 预期的正确响应帧的长度，不包括校验码的长度
    QByteArray data; // 发送的帧数据，不包括校验码
    // resplen和data的长度必须大于等于2，即必须包括机器地址和功能码
};

// 响应帧
struct MCResponse {
    QByteArray data; // 收到的帧数据，不包括校验码
};

typedef MCRequest MCRequest_zy1;
typedef MCResponse MCResponse_zy1;
#define Q_ARG_MCRequest(data) Q_ARG(MCRequest_zy1, data)
#define Q_ARG_MCResponse(data) Q_ARG(MCResponse_zy1, data)
#define Q_ARG_List_MCRequest(data) Q_ARG(QList<MCRequest_zy1>, data)

class MachineComm
{
public:
    virtual ~MachineComm();
    SignalEchoManager echom;
    virtual void request(const MCRequest &req, const SignalEcho &echo = SignalEcho()) = 0;
    virtual void request(const QList<MCRequest> &reqs, const SignalEcho &echo = SignalEcho()) = 0;
    virtual void clearRequest(SignalEcho &echo, SignalEchoIds &ids) = 0;
};

#ifndef MYOPT_MCTCP
class MachineCommRTU : public QObject, public MachineComm
{
    Q_OBJECT
public:
    explicit MachineCommRTU(QIODevice *io, QObject *parent = 0);
    virtual void request(const MCRequest &req, const SignalEcho &echo = SignalEcho());
    virtual void request(const QList<MCRequest> &reqs, const SignalEcho &echo = SignalEcho());
    virtual void clearRequest(SignalEcho &echo, SignalEchoIds &ids);

private:
    Q_SLOT void newRequest(const QList<MCRequest_zy1> &reqs, const SignalEcho_zy1 &echo);
    Q_SLOT void clearRequest_p(const SignalEcho_zy1 &echo);
    void nextRequest();
    void sendRequest();
    void resendRequest();
    void response(int err);
    Q_SLOT void onReadyRead();
    Q_SLOT void onTimeout();

private:
    QIODevice *m_io;
    QTimer timerReadyRead;
    QTimer timerTimeout;

    QQueue<QList<MCRequest> > reqsQueue;
    QQueue<SignalEcho> echoQueue;
    QList<MCRequest> curReq;
    SignalEcho curEcho;
    int curReqi;
    bool curClear;

    QByteArray send; // 发送缓存
    QByteArray recv; // 接收缓存
    int rsplen; // 预期的正确响应帧的长度
    int timeoutms; // 超时时间
    int resendccnt; // 当前已重发次数
    int resendcnt; // 超时重发次数
};
#else
class MachineCommTCP : public QObject, public MachineComm
{
    Q_OBJECT
public:
    explicit MachineCommTCP(QTcpSocket *s, QObject *parent = 0);
    virtual void request(const MCRequest &req, const SignalEcho &echo = SignalEcho());
    virtual void request(const QList<MCRequest> &reqs, const SignalEcho &echo = SignalEcho());
    virtual void clearRequest(SignalEcho &echo, SignalEchoIds &ids);

private:
    Q_SLOT void newRequest(const QList<MCRequest_zy1> &reqs, const SignalEcho_zy1 &echo);
    Q_SLOT void clearRequest_p(const SignalEcho_zy1 &echo);
    void sendClearRequest();
    void nextRequest();
    void sendRequest();
    void response(int err, int len = 0);
    Q_SLOT void onReadyRead();
    Q_SLOT void onTimeout();

private:
    QTcpSocket *socket;
    QTimer timerTimeout;

    QQueue<QList<MCRequest> > reqsQueue;
    QQueue<SignalEcho> echoQueue;
    QList<MCRequest> curReq;
    SignalEcho curEcho;
    int curReqi;

    QByteArray recv; // 接收缓存
    quint16 workidSend; // 要发送的事务id
    quint16 workidRecv; // 要接收的事务id
    int rsplen; // 预期的正确响应帧的长度
    Q_SIGNAL void socketConnected();
    Q_SIGNAL void socketDisconnected();
    Q_SLOT void onSocketConnected();
    Q_SLOT void onSocketDisconnected();
    Q_SLOT void onSocketReconnect();
    bool socketIsReconnecting;
    bool socketIsConnected;
};
#endif

class MachineCommQueue : public QObject, public MachineComm
{
    Q_OBJECT
public:
    MachineCommQueue(MachineComm *mcomm, QObject *parent = 0);
    ~MachineCommQueue();
    virtual void request(const MCRequest &req, const SignalEcho &echo = SignalEcho());
    virtual void request(const QList<MCRequest> &reqs, const SignalEcho &echo = SignalEcho());
    virtual void clearRequest(SignalEcho &echo, SignalEchoIds &ids);

private:
    Q_SLOT void newRequest(const QList<MCRequest_zy1> &reqs, const SignalEcho_zy1 &echo);
    Q_SLOT void clearRequest_p(const SignalEcho_zy1 &echo);
    void nextRequest();
    Q_INVOKABLE void onResponse(int err,
                                const MCRequest_zy1 &req,
                                const MCResponse_zy1 &rsp,
                                const SignalEcho_zy1 &echo);

private:
    MachineComm *mc;
    QQueue<QList<MCRequest> > reqsQueue;
    QQueue<SignalEcho> echoQueue;
    QList<MCRequest> curReq;
    SignalEcho curEcho;
    int curReqi;
    SignalEcho m_echo;
    SignalEchoIds m_ecIds;
};

#endif // MACHINECOMM_H
