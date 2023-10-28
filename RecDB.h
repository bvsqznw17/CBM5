#ifndef RECDB_H
#define RECDB_H

#include <QSqlQuery>
#include <QMutex>
#include <QDateTime>
#include <QVariant>
#include <QList>
#include <QLinkedList>

#define RecQ_Handy RecQ_Handy_cbm5_1
#define RecQR_Handy RecQR_Handy_cbm5_1
#define RecSave_Rec RecSave_Rec_cbm5_1
#define RecDF_PakCntF ((double)1000)

// 记录
struct Rec_Rec {
    int weight;
    QDateTime time;
    uchar hege;
    uchar prec;     // 小数位
    uchar unit;     // 单位
};
// 日期
struct Rec_Date {
    int prm_id;
    QString prm_name;
    QDateTime start_time;
    QDateTime end_time;
    int weight1;
    int weight2;
    int weight3;
    int speed;
    int hegeCnt;
    qint64 totalWeight;
    qint64 totalPakCnt;
    uchar prec;     // 小数位
    uchar unit;     // 单位
    int drcnt;
};
// 查询结果-记录总数
struct RecQR_Cnt {
    int err;
    int rcnt; // 总记录数
    int dcnt; // 总天数
    RecQR_Cnt(int err_ = 0) : err(err_) {}
};
// 查询结果-记录
struct RecQR_Rec {
    int err;
    QList<Rec_Rec> l;
    int rid;
    RecQR_Rec(int err_ = 0) : err(err_) {}
};
// 查询结果-日期
struct RecQR_Date {
    int err;
    QList<Rec_Date> l;
    int did;
    RecQR_Date(int err_ = 0) : err(err_) {}
};
// 查询结果-某日记录
struct RecQR_DRec {
    int err;
    QList<Rec_Rec> l;
    int did;
    int rid;
    RecQR_DRec(int err_ = 0) : err(err_) {}
};
// 查询条件-方便的
struct RecQ_Handy {
    int did;
    int dn;
    int di;
    bool drevs;
    int rid;
    int rn;
    bool rrevs;
};
// 查询结果-方便的
struct RecQR_Handy {
    int err;
    RecQR_Cnt rc;
    QList<Rec_Date> ld;
    QList<Rec_Rec> lr;
    int did;
    int di;
    int rid;
    RecQR_Handy(int err_ = 0) : err(err_) {}
};
// 保存-记录
struct RecSave_Rec {
    Rec_Date d;
    QList<Rec_Rec> l;
};

Q_DECLARE_METATYPE(RecQ_Handy_cbm5_1)
Q_DECLARE_METATYPE(RecQR_Handy_cbm5_1)
Q_DECLARE_METATYPE(RecSave_Rec_cbm5_1)

#ifndef MYOPT_OnlineManager
class RecDB : public QObject
{
    Q_OBJECT
public:
    RecDB(const QString &dbName);
    RecQR_Cnt queryCnt();
    RecQR_Rec queryRec(int rid, int n);
    RecQR_Date queryDate(int did, int n);
    RecQR_DRec queryDRec(int did, int rid, int n);
    RecQR_Handy queryHandy(RecQ_Handy rq);
    void addRec(const RecSave_Rec &rsr);
    void addRecStart(const Rec_Date &r);
    void addRecEnd(const QDateTime &time);
    void clear();
    bool init();

signals:
    void recUpdate();

private:
    void rbuf_init(QSqlQuery &q);
    bool rbuf_queryDate(QList<Rec_Date> &list, int did, int n);
    bool rbuf_queryDRec(QList<Rec_Rec> &list, int did, int rid, int n);
    void rbuf_newQueryDate(const QList<Rec_Date> &list, int did, int n);
    void rbuf_newQueryDRec(const QList<Rec_Rec> &list, int did, int rid, int n);
    void rbuf_addDate(const Rec_Date &r);
    void rbuf_addRec(const RecSave_Rec &rsr, int index, int n);

    void pri_addDate(const Rec_Date &r);
    void pri_addRec(const RecSave_Rec &rsr, int index, int n);

    void pri_quNextD(QSqlQuery &q, QList<Rec_Date> &list);
    void pri_quNextR(QSqlQuery &q, QList<Rec_Rec> &list);

private:
    QSqlDatabase m_db;
    QMutex m_mutex;

    bool isInit;
    RecQR_Cnt lrc;
    Rec_Date lrd;
    QList<Rec_Date> lastd;
    QList<Rec_Date> firstd;
    QList<QList<Rec_Rec> > lastdLr;
    QList<QList<Rec_Rec> > lastdFr;
    QList<QList<Rec_Rec> > firstdLr;
    QList<QList<Rec_Rec> > firstdFr;

    struct RecBuf_Rec {
        int rid;
        int n;
        QList<Rec_Rec> l;
    };
    struct RecBuf_Date {
        int did;
        int n;
        QList<Rec_Date> l;
    };
    struct RecBuf_DRec {
        int did;
        int rid;
        int n;
        QList<Rec_Rec> l;
    };
    QLinkedList<RecBuf_Rec> rbufr;
    QLinkedList<RecBuf_Date> rbufd;
    QLinkedList<RecBuf_DRec> rbufdr;
};
#else

#include <QThread>
#include <QTcpSocket>
#include <QJsonObject>
#include "JsonTcpTool.h"

class RecDB : public QObject
{
    Q_OBJECT
public:
    RecDB(QTcpSocket *s);
    ~RecDB();
    RecQR_Cnt queryCnt();
    RecQR_Date queryDate(int did, int n);
    RecQR_DRec queryDRec(int did, int rid, int n);
    RecQR_Handy queryHandy(RecQ_Handy rq);
    void addRec(const RecSave_Rec &rsr);
    void addRecStart(const Rec_Date &r);
    void addRecEnd(const QDateTime &time);
    void clear();

signals:
    void recUpdate();

private:
    QJsonObject request(const QString &fun, const QJsonObject &content = QJsonObject());
    void timerEvent(QTimerEvent *event);

private:
    QMutex m_mutex;
    QTcpSocket *socket;
    JsonTcpTool jtt;
    QByteArray key;
    QString random;
    QString myRandom;

    QThread thd;
    RecQR_Cnt recCnt;
};
#endif

#endif // RECDB_H
