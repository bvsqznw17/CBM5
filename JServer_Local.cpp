#include "JServer_Local.h"

JServer_Local::JServer_Local(JsonTcpServer_FuncMap *map, QObject *parent) : QObject(parent)
{
    map->insert("mc_in_tweight", std::bind(&JServer_Local::mc_in_tweight, this, std::placeholders::_1));
}

bool JServer_Local::mc_in_tweight(JsonTcpServer_Socket &s)
{
    int w = s.content.value("tweight").toInt();
    emit sig_mc_in_tweight(w);
    s.writeJsonResp(0, "");
    return true;
}
