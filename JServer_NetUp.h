#ifndef JSERVER_NETUP_H
#define JSERVER_NETUP_H

#include <QObject>
#include <QMutex>
#include "JsonTcpServer.h"

class JServer_NetUp : public QObject
{
    Q_OBJECT
public:
    JServer_NetUp(JsonTcpServer_FuncMap *map, QObject *parent = nullptr);

    static void net_up_clear();

signals:
    void net_up_ready(const QString &fn);

private:
    bool net_up(JsonTcpServer_Socket &s);
    QMutex mutex;

public:
    static void makeFileMd5(
            const QString &fn, const QByteArray &data,
            const QString &rd, const QByteArray &key,
            QByteArray &md5);
    static bool checkFileMd5(
            const QString &fn, const QByteArray &data,
            const QString &rd, const QByteArray &key,
            const QByteArray &md5);
    static bool checkDiskSize(int m);
};

#endif // JSERVER_NETUP_H
