#ifndef TCPCOMM_H
#define TCPCOMM_H

#include <QTcpServer>
#include <QTcpSocket>

class TcpComm : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpComm(quint16 _port, QObject *parent = 0);

    inline void setIsOnline(bool b)
    { QMetaObject::invokeMethod(this, "setIsOnline_inv", Q_ARG(bool, b)); }
    Q_INVOKABLE void setIsOnline_inv(bool b);

    inline void outNumber(int n)
    { QMetaObject::invokeMethod(this, "onOutNumber", Q_ARG(int, n)); }

signals:
    void inNumber(int n);

private:
    void incomingConnection(qintptr handle);

private slots:
    void onDisconnected();
    void onReadyRead();
    void onOutNumber(int n);

private:
    bool isOnline;
    quint16 port;
    QTcpSocket *socket;
};

#endif // TCPCOMM_H
