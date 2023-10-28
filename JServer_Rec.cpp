#include "JServer_Rec.h"

JServer_Rec::JServer_Rec(JsonTcpServer_FuncMap *map, RecDB *db, QObject *parent) : QObject(parent)
{
    recdb = db;
    map->insert("rec_queryCnt", std::bind(&JServer_Rec::rec_queryCnt,
                                          this, std::placeholders::_1));
    map->insert("rec_queryDate", std::bind(&JServer_Rec::rec_queryDate,
                                          this, std::placeholders::_1));
    map->insert("rec_queryDRec", std::bind(&JServer_Rec::rec_queryDRec,
                                          this, std::placeholders::_1));
    map->insert("rec_queryHandy", std::bind(&JServer_Rec::rec_queryHandy,
                                          this, std::placeholders::_1));
    map->insert("rec_clear", std::bind(&JServer_Rec::rec_clear,
                                          this, std::placeholders::_1));
}

bool JServer_Rec::rec_queryCnt(JsonTcpServer_Socket &s)
{
    RecQR_Cnt rqr = recdb->queryCnt();

    QJsonObject jo;
    if (!rqr.err) {
        jo.insert("rcnt", rqr.rcnt);
        jo.insert("dcnt", rqr.dcnt);
    }
    s.writeJsonResp(rqr.err, "", jo);
    return true;
}

bool JServer_Rec::rec_queryDate(JsonTcpServer_Socket &s)
{
    int did = s.content.value("did").toInt();
    int n = s.content.value("n").toInt();
    RecQR_Date rqr;
    if (n > 1000)
        rqr.err = -1;
    else
        rqr = recdb->queryDate(did, n);

    QJsonObject jo;
    if (!rqr.err) {
        jo.insert("did", rqr.did);
        jo.insert("n", rqr.l.size());
        QJsonObject jo_list;
        QJsonArray ja_prm_id;
        QJsonArray ja_prm_name;
        QJsonArray ja_start_time;
        QJsonArray ja_end_time;
        QJsonArray ja_weight1;
        QJsonArray ja_weight2;
        QJsonArray ja_weight3;
        QJsonArray ja_speed;
        QJsonArray ja_hegeCnt;
        QJsonArray ja_totalWeight;
        QJsonArray ja_totalPakCnt;
        QJsonArray ja_prec;
        QJsonArray ja_unit;
        QJsonArray ja_drcnt;
        foreach (const Rec_Date &r, rqr.l) {
            ja_prm_id.append(r.prm_id);
            ja_prm_name.append(r.prm_name);
            ja_start_time.append(r.start_time.toString("yyyy-MM-dd hh:mm:ss"));
            ja_end_time.append(r.end_time.toString("yyyy-MM-dd hh:mm:ss"));
            ja_weight1.append(r.weight1);
            ja_weight2.append(r.weight2);
            ja_weight3.append(r.weight3);
            ja_speed.append(r.speed);
            ja_hegeCnt.append(r.hegeCnt);
            ja_totalWeight.append(QString::number(r.totalWeight));
            ja_totalPakCnt.append(QString::number(r.totalPakCnt));
            ja_prec.append(r.prec);
            ja_unit.append(r.unit);
            ja_drcnt.append(r.drcnt);
        }
        jo_list.insert("prm_id", ja_prm_id);
        jo_list.insert("prm_name", ja_prm_name);
        jo_list.insert("start_time", ja_start_time);
        jo_list.insert("end_time", ja_end_time);
        jo_list.insert("weight1", ja_weight1);
        jo_list.insert("weight2", ja_weight2);
        jo_list.insert("weight3", ja_weight3);
        jo_list.insert("speed", ja_speed);
        jo_list.insert("hegeCnt", ja_hegeCnt);
        jo_list.insert("totalWeight", ja_totalWeight);
        jo_list.insert("totalPakCnt", ja_totalPakCnt);
        jo_list.insert("prec", ja_prec);
        jo_list.insert("unit", ja_unit);
        jo_list.insert("drcnt", ja_drcnt);
        jo.insert("z_list", jo_list);
    }
    s.writeJsonResp(rqr.err, "", jo);
    return true;
}

bool JServer_Rec::rec_queryDRec(JsonTcpServer_Socket &s)
{
    int did = s.content.value("did").toInt();
    int rid = s.content.value("rid").toInt();
    int n = s.content.value("n").toInt();
    RecQR_DRec rqr;
    if (n > 1000)
        rqr.err = -1;
    else
        rqr = recdb->queryDRec(did, rid, n);

    QJsonObject jo;
    if (!rqr.err) {
        jo.insert("did", rqr.did);
        jo.insert("rid", rqr.rid);
        jo.insert("n", rqr.l.size());
        QJsonObject jo_list;
        QJsonArray ja_weight;
        QJsonArray ja_time;
        QJsonArray ja_hege;
        QJsonArray ja_prec;
        QJsonArray ja_unit;
        foreach (const Rec_Rec &r, rqr.l) {
            ja_weight.append(r.weight);
            ja_time.append(r.time.toString("yyyy-MM-dd hh:mm:ss"));
            ja_hege.append(r.hege);
            ja_prec.append(r.prec);
            ja_unit.append(r.unit);
        }
        jo_list.insert("weight", ja_weight);
        jo_list.insert("time", ja_time);
        jo_list.insert("hege", ja_hege);
        jo_list.insert("prec", ja_prec);
        jo_list.insert("unit", ja_unit);
        jo.insert("z_list", jo_list);
    }
    s.writeJsonResp(rqr.err, "", jo);
    return true;
}

bool JServer_Rec::rec_queryHandy(JsonTcpServer_Socket &s)
{
    RecQ_Handy rq;
    rq.did = s.content.value("did").toInt();
    rq.dn = s.content.value("dn").toInt();
    rq.di = s.content.value("di").toInt();
    rq.drevs = s.content.value("drevs").toBool();
    rq.rid = s.content.value("rid").toInt();
    rq.rn = s.content.value("rn").toInt();
    rq.rrevs = s.content.value("rrevs").toBool();
    RecQR_Handy rqr;
    if (rq.dn > 1000 || rq.rn > 1000)
        rqr.err = -1;
    else
        rqr = recdb->queryHandy(rq);

    QJsonObject jo;
    if (!rqr.err) {
        jo.insert("rcnt", rqr.rc.rcnt);
        jo.insert("dcnt", rqr.rc.dcnt);
        jo.insert("did", rqr.did);
        jo.insert("di", rqr.di);
        jo.insert("rid", rqr.rid);
        jo.insert("dn", rqr.ld.size());
        jo.insert("rn", rqr.lr.size());
        {
            QJsonObject jo_list;
            QJsonArray ja_weight;
            QJsonArray ja_time;
            QJsonArray ja_hege;
            QJsonArray ja_prec;
            QJsonArray ja_unit;
            foreach (const Rec_Rec &r, rqr.lr) {
                ja_weight.append(r.weight);
                ja_time.append(r.time.toString("yyyy-MM-dd hh:mm:ss"));
                ja_hege.append(r.hege);
                ja_prec.append(r.prec);
                ja_unit.append(r.unit);
            }
            jo_list.insert("weight", ja_weight);
            jo_list.insert("time", ja_time);
            jo_list.insert("hege", ja_hege);
            jo_list.insert("prec", ja_prec);
            jo_list.insert("unit", ja_unit);
            jo.insert("z_list_r", jo_list);
        }
        {
            QJsonObject jo_list;
            QJsonArray ja_prm_id;
            QJsonArray ja_prm_name;
            QJsonArray ja_start_time;
            QJsonArray ja_end_time;
            QJsonArray ja_weight1;
            QJsonArray ja_weight2;
            QJsonArray ja_weight3;
            QJsonArray ja_speed;
            QJsonArray ja_hegeCnt;
            QJsonArray ja_totalWeight;
            QJsonArray ja_totalPakCnt;
            QJsonArray ja_prec;
            QJsonArray ja_unit;
            QJsonArray ja_drcnt;
            foreach (const Rec_Date &r, rqr.ld) {
                ja_prm_id.append(r.prm_id);
                ja_prm_name.append(r.prm_name);
                ja_start_time.append(r.start_time.toString("yyyy-MM-dd hh:mm:ss"));
                ja_end_time.append(r.end_time.toString("yyyy-MM-dd hh:mm:ss"));
                ja_weight1.append(r.weight1);
                ja_weight2.append(r.weight2);
                ja_weight3.append(r.weight3);
                ja_speed.append(r.speed);
                ja_hegeCnt.append(r.hegeCnt);
                ja_totalWeight.append(QString::number(r.totalWeight));
                ja_totalPakCnt.append(QString::number(r.totalPakCnt));
                ja_prec.append(r.prec);
                ja_unit.append(r.unit);
                ja_drcnt.append(r.drcnt);
            }
            jo_list.insert("prm_id", ja_prm_id);
            jo_list.insert("prm_name", ja_prm_name);
            jo_list.insert("start_time", ja_start_time);
            jo_list.insert("end_time", ja_end_time);
            jo_list.insert("weight1", ja_weight1);
            jo_list.insert("weight2", ja_weight2);
            jo_list.insert("weight3", ja_weight3);
            jo_list.insert("speed", ja_speed);
            jo_list.insert("hegeCnt", ja_hegeCnt);
            jo_list.insert("totalWeight", ja_totalWeight);
            jo_list.insert("totalPakCnt", ja_totalPakCnt);
            jo_list.insert("prec", ja_prec);
            jo_list.insert("unit", ja_unit);
            jo_list.insert("drcnt", ja_drcnt);
            jo.insert("z_list_d", jo_list);
        }
    }
    s.writeJsonResp(rqr.err, "", jo);
    return true;
}

bool JServer_Rec::rec_clear(JsonTcpServer_Socket &s)
{
    recdb->clear();
    s.writeJsonResp(0, "");
    return true;
}
