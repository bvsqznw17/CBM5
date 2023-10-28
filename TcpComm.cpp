#include "TcpComm.h"
#include "JsonTcpTool.h"

TcpComm::TcpComm(quint16 _port, QObject *parent) : QTcpServer(parent)
{
    isOnline = false;
    port = _port;
    socket = 0;
}

void TcpComm::setIsOnline_inv(bool b)
{
    if (b == isOnline)
        return;
    isOnline = b;

    if (isOnline) {
        QTcpServer::listen(QHostAddress::Any, port);
    }
    else {
        QTcpServer::close();
    }
}

void TcpComm::incomingConnection(qintptr handle)
{
    if (socket) {
        QTcpSocket socket(this);
        socket.setSocketDescriptor(handle);
        return;
    }

    socket_set_keepalive(handle);
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(handle);

    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(onDisconnected()));
}

void TcpComm::onDisconnected()
{
    if (!socket)
        return;

    socket->deleteLater();
    socket = 0;
}

void TcpComm::onReadyRead()
{
    if (!socket)
        return;

    QByteArray recv = socket->readAll();
    int n = recv.toInt();
    emit inNumber(n);
}

void TcpComm::onOutNumber(int n)
{
    if (!socket)
        return;

    socket->write(QByteArray::number(n).append('\n'));
}
