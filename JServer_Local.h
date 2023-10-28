#ifndef JSERVER_LOCAL_H
#define JSERVER_LOCAL_H

#include <QObject>
#include "JsonTcpServer.h"

class JServer_Local : public QObject
{
    Q_OBJECT
public:
    explicit JServer_Local(JsonTcpServer_FuncMap *map, QObject *parent = nullptr);

signals:
    void sig_mc_in_tweight(int w);

private:
    bool mc_in_tweight(JsonTcpServer_Socket &s);

};

#endif // JSERVER_LOCAL_H
