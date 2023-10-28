#ifndef JSERVER_REC_H
#define JSERVER_REC_H

#include <QObject>
#include "JsonTcpServer.h"
#include "RecDB.h"

class JServer_Rec : public QObject
{
    Q_OBJECT
public:
    explicit JServer_Rec(JsonTcpServer_FuncMap *map, RecDB *db, QObject *parent = nullptr);

signals:

private:
    bool rec_queryCnt(JsonTcpServer_Socket &s);
    bool rec_queryDate(JsonTcpServer_Socket &s);
    bool rec_queryDRec(JsonTcpServer_Socket &s);
    bool rec_queryHandy(JsonTcpServer_Socket &s);
    bool rec_clear(JsonTcpServer_Socket &s);

private:
    RecDB *recdb;
};

#endif // JSERVER_REC_H
