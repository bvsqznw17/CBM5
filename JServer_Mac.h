#ifndef JSERVER_MAC_H
#define JSERVER_MAC_H

#include <QObject>
#include "JsonTcpServer.h"

class JServer_Mac : public QObject
{
    Q_OBJECT
public:
    explicit JServer_Mac(JsonTcpServer_FuncMap *map, QObject *parent = nullptr);
    QString uuid;

    inline QString getName() { return m_name; }
    inline QString getCPUSN() { return m_cpusn; }
    void setName(const QString &str);

private:
    bool mac_info(JsonTcpServer_Socket &s);
    bool heart_beat(JsonTcpServer_Socket &s);

    QString readCPUSN();

private:
    QMutex m_mutex;
    QString m_name;
    QString m_cpusn;
};

#endif // JSERVER_MAC_H
