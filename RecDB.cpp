#include "RecDB.h"
#ifndef MYOPT_OnlineManager
#include <QFile>

#ifdef MYOPT_DEBUG
#include <QElapsedTimer>
#include <QSqlError>
#include <QDebug>
#define RecDB_Debug
#endif

// 缓存数(>=1)
#define RBufN_Lastd  201    // 最近日期
#define RBufN_Firstd 101    // 最早日期
#define RBufN_Lastrd  21    // 有记录的最近日期(<=RBufN_Lastd)
#define RBufN_Firstrd 11    // 有记录的最早日期(<=RBufN_Firstd)
#define RBufN_LastdLr 100   // 最近日期中的最近记录
#define RBufN_LastdFr 50    // 最近日期中的最早记录
#define RBufN_FirstdLr 100  // 最早日期中的最近记录
#define RBufN_FirstdFr 50   // 最早日期中的最早记录
#define RBufN_d  50         // 最近查询日期
#define RBufN_dr 50         // 最近查询某日记录

// rec表暂时没有id列，必须同时查询did和rid
static QString quSelR("select * from rec where id>=%1 limit %2");
static QString quSelD("select * from date where id>=%1 limit %2");
static QString quSelDR("select * from rec where did=%1 and rid>=%2 limit %3");
static QString quPreInsD("insert into date values (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
static QString quPreInsR("insert into rec values (?,?,?,?,?,?,?)");
static QString quUpdD("update date set "
                      "end_time='%2', hegeCnt=%3, "
                      "totalWeight=%4, totalPakCnt=%5, "
                      "rcnt=%6 "
                      "where id=%1");

RecDB::RecDB(const QString &dbName) :
    m_mutex(QMutex::Recursive)
{
    if (!QFile::exists(dbName)) {
        QFile::copy("rec_init.db", dbName);
#ifdef __ARM_EABI__
        system("sync");
#endif
    }
    QString dbcName = QString("RecDB-%1").arg(dbName);
    m_db = QSqlDatabase::addDatabase("QSQLITE", dbcName);
    m_db.setDatabaseName(dbName);
    m_db.open();

    isInit = false;
}

RecQR_Cnt RecDB::queryCnt()
{
    if (!init()) return RecQR_Cnt(-1);
    QMutexLocker locker(&m_mutex);

    return lrc;
}

RecQR_Rec RecDB::queryRec(int rid, int n)
{
    return RecQR_Rec(-1);
    if (rid < 0) {
        n += rid;
        rid = 0;
    }
    if (!init()) return RecQR_Rec(-1);
    QMutexLocker locker(&m_mutex);
    RecQR_Rec rqr;
    rqr.rid = rid;
    if (n <= 0)
        return rqr;

#ifdef RecDB_Debug
    QElapsedTimer etimer;
    etimer.start();
#endif
//    if (rbuf_queryDRec(rqr.l, did, rid, n))
//        return rqr;

    QSqlQuery q(m_db);
    if (!q.exec(quSelR.arg(rid).arg(n)))
        return RecQR_Rec(-1);
    rqr.l.reserve(n);
    pri_quNextR(q, rqr.l);
//    rbuf_newQueryDRec(rqr.l, did, rid, n);
#ifdef RecDB_Debug
    qDebug() << "从数据库中查询记录:" << etimer.elapsed();
#endif
    return rqr;
}

RecQR_Date RecDB::queryDate(int did, int n)
{
    if (did < 0) {
        n += did;
        did = 0;
    }
    if (!init()) return RecQR_Date(-1);
    QMutexLocker locker(&m_mutex);
    RecQR_Date rqr;
    rqr.did = did;
    if (n <= 0)
        return rqr;

#ifdef RecDB_Debug
    QElapsedTimer etimer;
    etimer.start();
#endif
    if (rbuf_queryDate(rqr.l, did, n))
        return rqr;

    QSqlQuery q(m_db);
    if (!q.exec(quSelD.arg(did).arg(n)))
        return RecQR_Date(-1);
    rqr.l.reserve(n);
    pri_quNextD(q, rqr.l);
    rbuf_newQueryDate(rqr.l, did, n);
#ifdef RecDB_Debug
    qDebug() << "从数据库中查询日期:" << etimer.elapsed();
#endif
    return rqr;
}

RecQR_DRec RecDB::queryDRec(int did, int rid, int n)
{
    if (rid < 0) {
        n += rid;
        rid = 0;
    }
    if (!init()) return RecQR_DRec(-1);
    QMutexLocker locker(&m_mutex);
    RecQR_DRec rqr;
    rqr.did = did;
    rqr.rid = rid;
    if (n <= 0)
        return rqr;

#ifdef RecDB_Debug
    QElapsedTimer etimer;
    etimer.start();
#endif
    if (rbuf_queryDRec(rqr.l, did, rid, n))
        return rqr;

    QSqlQuery q(m_db);
    if (!q.exec(quSelDR.arg(did).arg(rid).arg(n)))
        return RecQR_DRec(-1);
    rqr.l.reserve(n);
    pri_quNextR(q, rqr.l);
    rbuf_newQueryDRec(rqr.l, did, rid, n);
#ifdef RecDB_Debug
    qDebug() << "从数据库中查询记录:" << etimer.elapsed();
#endif
    return rqr;
}

RecQR_Handy RecDB::queryHandy(RecQ_Handy rq)
{
    if (!init()) return RecQR_Handy(-1);
    QMutexLocker locker(&m_mutex);
    RecQR_Handy rqr;
    rqr.rc = lrc;

    if (rq.drevs) {
        rq.did = rqr.rc.dcnt - rq.did - rq.dn;
        rq.di = rq.dn - rq.di - 1;
    }
    RecQR_Date rqrd = queryDate(rq.did, rq.dn);
    if (rqrd.err)
        return RecQR_Handy(rqrd.err);
    rqr.did = rqrd.did;
    rqr.ld = rqrd.l;

    rqr.di = rq.di + rq.did - rqr.did;
    if (rqr.di < 0 || rqr.di >= rqr.ld.size()) {
        rqr.rid = rq.rid;
        return rqr;
    }
    if (rq.rrevs) {
        rq.rid = rqr.ld.at(rqr.di).drcnt - rq.rid - rq.rn;
    }
    RecQR_DRec rqrdr = queryDRec(rqr.did + rqr.di, rq.rid, rq.rn);
    if (rqrdr.err)
        return RecQR_Handy(rqrdr.err);
    rqr.rid = rqrdr.rid;
    rqr.lr = rqrdr.l;

    return rqr;
}

void RecDB::addRec(const RecSave_Rec &rsr)
{
    if (!init()) return;
    QMutexLocker locker(&m_mutex);
    pri_addRec(rsr, 0, rsr.l.size());
    emit recUpdate();
}

void RecDB::addRecStart(const Rec_Date &r)
{
    if (!init()) return;
    QMutexLocker locker(&m_mutex);
    pri_addDate(r);
    emit recUpdate();
}

void RecDB::addRecEnd(const QDateTime &time)
{
    if (!init()) return;
    QMutexLocker locker(&m_mutex);
    QString str = "update date set end_time='%2' where id=%1";
    str = str.arg(lrc.dcnt - 1).arg(time.toString("yyyy-MM-dd hh:mm:ss"));

    QSqlQuery q(m_db);
    if (q.exec(str)) {
        // 修改最近
        if (!lastd.isEmpty())
            lastd[lastd.size() - 1].end_time = time;
        // 修改缓存
        QMutableLinkedListIterator<RecBuf_Date> itd(rbufd);
        while (itd.hasNext()) {
            RecBuf_Date &rb = itd.next();
            if (rb.did + rb.n >= lrc.dcnt && rb.did < lrc.dcnt) {
                rb.l[rb.l.size() - 1].end_time = time;
            }
        }
    }
    emit recUpdate();
}

void RecDB::clear()
{
    QMutexLocker locker(&m_mutex);
    if (!m_db.databaseName().isEmpty()) {
        m_db.close();
        QFile::remove(m_db.databaseName());
        QFile::copy("rec_init.db", m_db.databaseName());
        m_db.open();
#ifdef __ARM_EABI__
        system("sync");
#endif
    }
    isInit = false;
    lastd.clear();
    firstd.clear();
    lastdLr.clear();
    lastdFr.clear();
    firstdLr.clear();
    firstdFr.clear();
    rbufr.clear();
    rbufd.clear();
    rbufdr.clear();
    emit recUpdate();
}

bool RecDB::init()
{
    if (isInit) return true;
    QMutexLocker locker(&m_mutex);
    if (isInit) return true;

    QSqlQuery q(m_db);

    // 查询总记录数、总天数
    if (!q.exec("select sum(rcnt), count(*) from date"))
        return false;
    bool ok1, ok2;
    q.next();
    lrc.rcnt = q.value(0).toInt(&ok1);
    lrc.dcnt = q.value(1).toInt(&ok2);
#ifdef RecDB_Debug
    qDebug() << "total rcnt" << lrc.rcnt
             << "total dcnt" << lrc.dcnt
             << ok1 << ok2;
#endif
    if (!ok2)
        return false;
    isInit = true;

    rbuf_init(q);
    if (!lastd.isEmpty())
        lrd = lastd.at(lastd.size() - 1);

    return true;
}

void RecDB::rbuf_init(QSqlQuery &q)
{
    if (lrc.dcnt > 0) {
        // 缓存最近日期
        int did;
        if (lrc.dcnt <= RBufN_Lastrd)
            did = 0;
        else if (lrc.dcnt <= RBufN_Lastrd + RBufN_Firstrd)
            did = lrc.dcnt - RBufN_Lastrd;
        else if (lrc.dcnt <= RBufN_Lastd + RBufN_Firstrd)
            did = RBufN_Firstrd;
        else
            did = lrc.dcnt - RBufN_Lastd;
        int cnt = lrc.dcnt - did;
        q.exec(quSelD.arg(did).arg(cnt));
        pri_quNextD(q, lastd);

        int size = qMin(RBufN_Lastrd, lastd.size());
        int sizei = lastd.size() - size;
        for (int i = 0; i < size; i++) {
            lastdLr.append(QList<Rec_Rec>());
            lastdFr.append(QList<Rec_Rec>());
            int drcnt = lastd.at(i+sizei).drcnt;
            if (drcnt > 0) {
                // 缓存最近日期中的最近记录
                int rid = drcnt - RBufN_LastdLr;
                if (rid < 0) rid = 0;
                int rcnt = drcnt - rid;
                q.exec(quSelDR.arg(did+i+sizei).arg(rid).arg(rcnt));
                pri_quNextR(q, lastdLr[i]);

                // 缓存最近日期中的最早记录
                if (rid > 0) {
                    rcnt = rid; rid = 0;
                    if (rcnt > RBufN_LastdFr) rcnt = RBufN_LastdFr;
                    q.exec(quSelDR.arg(did+i+sizei).arg(rid).arg(rcnt));
                    pri_quNextR(q, lastdFr[i]);
                }
            }
        }

        // 缓存最早日期
        if (did > 0) {
            cnt = did; did = 0;
            if (cnt > RBufN_Firstd) cnt = RBufN_Firstd;
            q.exec(quSelD.arg(did).arg(cnt));
            pri_quNextD(q, firstd);

            int size = qMin(RBufN_Firstrd, firstd.size());
            for (int i = 0; i < size; i++) {
                firstdLr.append(QList<Rec_Rec>());
                firstdFr.append(QList<Rec_Rec>());
                int drcnt = firstd.at(i).drcnt;
                if (drcnt > 0) {
                    // 缓存最早日期中的最近记录
                    int rid = drcnt - RBufN_FirstdLr;
                    if (rid < 0) rid = 0;
                    int rcnt = drcnt - rid;
                    q.exec(quSelDR.arg(did+i).arg(rid).arg(rcnt));
                    pri_quNextR(q, firstdLr[i]);

                    // 缓存最早日期中的最早记录
                    if (rid > 0) {
                        rcnt = rid; rid = 0;
                        if (rcnt > RBufN_FirstdFr) rcnt = RBufN_FirstdFr;
                        q.exec(quSelDR.arg(did+i).arg(rid).arg(rcnt));
                        pri_quNextR(q, firstdFr[i]);
                    }
                }
            }
        }
    }
#ifdef RecDB_Debug
    QDebug qde = qDebug();
    qde = qDebug() << "last:" << lastd.size();
    qde = qDebug() << "lastl:";
    foreach (const QList<Rec_Rec> &l, lastdLr)
        qde << l.size();
    qde = qDebug() << "lastf:";
    foreach (const QList<Rec_Rec> &l, lastdFr)
        qde << l.size();
    qde = qDebug() << "first:" << firstd.size();
    qde = qDebug() << "firstl:";
    foreach (const QList<Rec_Rec> &l, firstdLr)
        qde << l.size();
    qde = qDebug() << "firstf:";
    foreach (const QList<Rec_Rec> &l, firstdFr)
        qde << l.size();
#endif
}

bool RecDB::rbuf_queryDate(QList<Rec_Date> &list, int did, int n)
{
    // 从最近日期中查询
    int di = did - (lrc.dcnt - lastd.size());
    if (di >= 0) {
        int nn = lastd.size() - di;
        if (nn > n) nn = n;
        list.reserve(nn);
        for (int i = 0; i < nn; i++)
            list.append(lastd.at(i+di));
#ifdef RecDB_Debug
        qDebug("在最近日期中查询到");
#endif
        return true;
    }
    // 从最早日期中查询
    di = did;
    if (di >= 0 && di + n <= firstd.size()) {
        int nn = n;
        list.reserve(nn);
        for (int i = 0; i < nn; i++)
            list.append(firstd.at(i+di));
#ifdef RecDB_Debug
        qDebug("在最早日期中查询到");
#endif
        return true;
    }
    // 从缓存中查询
    QMutableLinkedListIterator<RecBuf_Date> it(rbufd);
    while (it.hasNext()) {
        RecBuf_Date &rb = it.next();
        // 没有交集，跳过
        if (did >= rb.did + rb.n || rb.did >= did + n)
            continue;
        // 被缓存包含，使用该条缓存
        if (did >= rb.did && did + n <= rb.did + rb.n) {
#ifdef RecDB_Debug
        qDebug("在日期缓存中查询到");
#endif
            di = did - rb.did;
            int nn = rb.l.size() - di;
            if (nn > n) nn = n;
            list.reserve(nn);
            for (int i = 0; i < nn; i++)
                list.append(rb.l.at(i+di));
            RecBuf_Date rbf = rb;
            it.remove();
            rbufd.push_front(rbf);
            return true;
        }
        // 缓存被包含，移除该条缓存
        if (did <= rb.did && did + n >= rb.did + rb.n) {
            it.remove();
            continue;
        }
        // 剩下交叉情况，跳过
    }

    return false;
}

bool RecDB::rbuf_queryDRec(QList<Rec_Rec> &list, int did, int rid, int n)
{
    // 从最近日期中查询
    int di = did - (lrc.dcnt - lastdLr.size());
    if (di >= 0 && di < lastdLr.size()) {
        // 从最近日期中的最近记录中查询
        const QList<Rec_Rec> &llist = lastdLr.at(di);
        int sizei = lastd.size() - lastdLr.size();
        int ri = rid - (lastd.at(di+sizei).drcnt - llist.size());
        if (ri >= 0) {
            int nn = llist.size() - ri;
            if (nn > n) nn = n;
            list.reserve(nn);
            for (int i = 0; i < nn; i++)
                list.append(llist.at(i+ri));
#ifdef RecDB_Debug
        qDebug("在最近日期中的最近记录中查询到");
#endif
            return true;
        }
        // 从最近日期中的最早记录中查询
        const QList<Rec_Rec> &flist = lastdFr.at(di);
        ri = rid;
        if (ri >= 0 && ri + n <= flist.size()) {
            int nn = n;
            list.reserve(nn);
            for (int i = 0; i < nn; i++)
                list.append(flist.at(i+ri));
#ifdef RecDB_Debug
        qDebug("在最近日期中的最早记录中查询到");
#endif
            return true;
        }
    }
    // 从最早日期中查询
    di = did;
    if (di >= 0 && di < firstdLr.size()) {
        // 从最早日期中的最近记录中查询
        const QList<Rec_Rec> &llist = firstdLr.at(di);
        int ri = rid - (firstd.at(di).drcnt - llist.size());
        if (ri >= 0) {
            int nn = llist.size() - ri;
            if (nn > n) nn = n;
            list.reserve(nn);
            for (int i = 0; i < nn; i++)
                list.append(llist.at(i+ri));
#ifdef RecDB_Debug
        qDebug("在最早日期中的最近记录中查询到");
#endif
            return true;
        }
        // 从最早日期中的最早记录中查询
        const QList<Rec_Rec> &flist = firstdFr.at(di);
        ri = rid;
        if (ri >= 0 && ri + n <= flist.size()) {
            int nn = n;
            list.reserve(nn);
            for (int i = 0; i < nn; i++)
                list.append(flist.at(i+ri));
#ifdef RecDB_Debug
        qDebug("在最早日期中的最早记录中查询到");
#endif
            return true;
        }
    }
    // 从缓存中查询
    QMutableLinkedListIterator<RecBuf_DRec> it(rbufdr);
    while (it.hasNext()) {
        RecBuf_DRec &rb = it.next();
        // 没有交集，跳过
        if (did != rb.did || rid >= rb.rid + rb.n || rb.rid >= rid + n)
            continue;
        // 被缓存包含，使用该条缓存
        if (rid >= rb.rid && rid + n <= rb.rid + rb.n) {
#ifdef RecDB_Debug
        qDebug("在记录缓存中查询到");
#endif
            di = rid - rb.rid;
            int nn = rb.l.size() - di;
            if (nn > n) nn = n;
            list.reserve(nn);
            for (int i = 0; i < nn; i++)
                list.append(rb.l.at(i+di));
            RecBuf_DRec rbf = rb;
            it.remove();
            rbufdr.push_front(rbf);
            return true;
        }
        // 缓存被包含，移除该条缓存
        if (rid <= rb.rid && rid + n >= rb.rid + rb.n) {
            it.remove();
            continue;
        }
        // 剩下交叉情况，跳过
    }

    return false;
}

void RecDB::rbuf_newQueryDate(const QList<Rec_Date> &list, int did, int n)
{
    if (rbufd.size() >= RBufN_d)
        rbufd.pop_back();
    RecBuf_Date rb;
    rb.did = did;
    rb.n = n;
    rb.l = list;
    rbufd.push_front(rb);
}

void RecDB::rbuf_newQueryDRec(const QList<Rec_Rec> &list, int did, int rid, int n)
{
    if (rbufdr.size() >= RBufN_dr)
        rbufdr.pop_back();
    RecBuf_DRec rb;
    rb.did = did;
    rb.rid = rid;
    rb.n = n;
    rb.l = list;
    rbufdr.push_front(rb);
}

void RecDB::rbuf_addDate(const Rec_Date &r)
{
    // 添加到最近或最早日期中
    bool b = false;
    if (lastdLr.size() >= RBufN_Lastrd) {
        if (firstdLr.size() < RBufN_Firstrd) {
            firstdLr.push_back(lastdLr.front());
            firstdFr.push_back(lastdFr.front());
            b = true;
        }
        lastdLr.pop_front();
        lastdFr.pop_front();
    }
    lastdLr.push_back(QList<Rec_Rec>());
    lastdFr.push_back(QList<Rec_Rec>());

    if (b)
        firstd.push_back(lastd.takeFirst());
    else if (lastd.size() >= RBufN_Lastd) {
        if (firstd.size() < RBufN_Firstd)
            firstd.push_back(lastd.front());
        lastd.pop_front();
    }
    lastd.push_back(r);

    // 修改缓存
    QMutableLinkedListIterator<RecBuf_Date> i(rbufd);
    while (i.hasNext()) {
        RecBuf_Date &rb = i.next();
        if (rb.did + rb.n > lrc.dcnt && rb.did < lrc.dcnt) {
            rb.l.append(r);
        }
    }
}

void RecDB::rbuf_addRec(const RecSave_Rec &rsr, int index, int n)
{
    {
        Rec_Date &lastd_rd = lastd[lastd.size() - 1];
        int drcnt = lastd_rd.drcnt;
        lastd_rd = rsr.d;
        lastd_rd.drcnt = drcnt + n;
    }
    QList<Rec_Rec> &llist = lastdLr.back();
    QList<Rec_Rec> &flist = lastdFr.back();

    for (int i = 0; i < n; i++) {
        const Rec_Rec &r = rsr.l.at(i+index);

        // 添加到最近或最早记录中
        if (llist.size() >= RBufN_LastdLr) {
            if (flist.size() < RBufN_LastdFr)
                flist.push_back(llist.front());
            llist.pop_front();
        }
        llist.push_back(r);
    }

    // 修改缓存
    QMutableLinkedListIterator<RecBuf_Date> itd(rbufd);
    while (itd.hasNext()) {
        RecBuf_Date &rb = itd.next();
        if (rb.did + rb.n >= lrc.dcnt && rb.did < lrc.dcnt) {
            Rec_Date &lastd_rd = rb.l[rb.l.size() - 1];
            int drcnt = lastd_rd.drcnt;
            lastd_rd = rsr.d;
            lastd_rd.drcnt = drcnt + n;
        }
    }
    int ldid = lrc.dcnt - 1;
    QMutableLinkedListIterator<RecBuf_DRec> itr(rbufdr);
    while (itr.hasNext()) {
        RecBuf_DRec &rb = itr.next();
        if (ldid == rb.did && rb.rid + rb.n > lrd.drcnt &&
                rb.rid < lrd.drcnt - 1 + n) {
            int i = 0;
            if (rb.rid >= lrd.drcnt)
                i = rb.rid - lrd.drcnt + 1;
            int nn = rb.n - rb.l.size();
            if (nn > n)
                nn = n;
            for (; i < nn; i++)
                rb.l.append(rsr.l.at(i+index));
        }
    }
}

void RecDB::pri_addDate(const Rec_Date &r)
{
#ifdef RecDB_Debug
    QElapsedTimer etimer;
    etimer.start();
#endif
    QSqlQuery q(m_db);
    q.prepare(quPreInsD);
    q.addBindValue(lrc.dcnt);
    q.addBindValue(r.prm_id);
    q.addBindValue(r.prm_name);
    q.addBindValue(r.start_time.toString("yyyy-MM-dd hh:mm:ss"));
    q.addBindValue(r.end_time.toString("yyyy-MM-dd hh:mm:ss"));
    q.addBindValue(r.weight1);
    q.addBindValue(r.weight2);
    q.addBindValue(r.weight3);
    q.addBindValue(r.speed);
    q.addBindValue(r.hegeCnt);
    q.addBindValue(r.totalWeight);
    q.addBindValue(r.totalPakCnt);
    q.addBindValue(r.prec);
    q.addBindValue(r.unit);
    q.addBindValue(0);
    if (q.exec()) {
        lrd = r;
        lrd.drcnt = 0;
        rbuf_addDate(lrd);
        lrc.dcnt++;
    }
#ifdef RecDB_Debug
    qDebug() << "增加日期 ms:" << etimer.elapsed();
#endif
}

void RecDB::pri_addRec(const RecSave_Rec &rsr, int index, int n)
{
#ifdef RecDB_Debug
    QElapsedTimer etimer;
    etimer.start();
#endif
    if (lrc.dcnt <= 0)
        return;

    QSqlQuery q(m_db);
    m_db.transaction();
    if (!q.exec(quUpdD.arg(lrc.dcnt - 1)
                .arg(rsr.d.end_time.toString("yyyy-MM-dd hh:mm:ss"))
                .arg(rsr.d.hegeCnt)
                .arg(rsr.d.totalWeight)
                .arg(rsr.d.totalPakCnt)
                .arg(lrd.drcnt + n))) {
        m_db.rollback();
        return;
    }
    const int ls = 7;
    QVariantList list[ls];
    for (int li = 0; li < ls; li++)
        list[li].reserve(n);
    int drcnt = lrd.drcnt;
    for (int i = 0; i < n; i++) {
        const Rec_Rec &r = rsr.l.at(i+index);
        int li = 0;
        list[li++].append(lrc.dcnt - 1);
        list[li++].append(drcnt++);
        list[li++].append(r.weight);
        list[li++].append(r.time.toString("yyyy-MM-dd hh:mm:ss"));
        list[li++].append(r.hege);
        list[li++].append(r.prec);
        list[li++].append(r.unit);
    }
    q.prepare(quPreInsR);
    for (int li = 0; li < ls; li++)
        q.addBindValue(list[li]);
    if (q.execBatch()) {
        m_db.commit();
        rbuf_addRec(rsr, index, n);
        lrc.rcnt += drcnt - lrd.drcnt;
        lrd = rsr.d;
        lrd.drcnt = drcnt;
    }
    else {
        m_db.rollback();
        return;
    }
#ifdef RecDB_Debug
    qDebug() << "增加记录" << "ms:" << etimer.elapsed() << "size:" << n;
#endif
}

void RecDB::pri_quNextD(QSqlQuery &q, QList<Rec_Date> &list)
{
    while (q.next()) {
        int i = 1;
        Rec_Date r;
        r.prm_id = q.value(i++).toInt();
        r.prm_name = q.value(i++).toString();
        r.start_time = q.value(i++).toDateTime();
        r.end_time = q.value(i++).toDateTime();
        r.weight1 = q.value(i++).toInt();
        r.weight2 = q.value(i++).toInt();
        r.weight3 = q.value(i++).toInt();
        r.speed = q.value(i++).toInt();
        r.hegeCnt = q.value(i++).toInt();
        r.totalWeight = q.value(i++).toLongLong();
        r.totalPakCnt = q.value(i++).toLongLong();
        r.prec = q.value(i++).toInt();
        r.unit = q.value(i++).toInt();
        r.drcnt = q.value(i++).toInt();
        list.append(r);
    }
}

void RecDB::pri_quNextR(QSqlQuery &q, QList<Rec_Rec> &list)
{
    while (q.next()) {
        int i = 2;
        Rec_Rec r;
        r.weight = q.value(i++).toInt();
        r.time = q.value(i++).toDateTime();
        r.hege = q.value(i++).toUInt();
        r.prec = q.value(i++).toUInt();
        r.unit = q.value(i++).toUInt();
        list.append(r);
    }
}
#else

#include <QTimer>
#include <QElapsedTimer>
#include <QJsonDocument>
#include <QJsonArray>

RecDB::RecDB(QTcpSocket *s) :
    m_mutex(QMutex::Recursive)
{
    socket = s;
    jtt.wms = LONG_LONG_MAX;
    jtt.json_maxsize = 1024*1024;
    key = "sk#`~`#CBM5";
    random = socket->property("random").toString();

    recCnt.rcnt = -1;
    recCnt.dcnt = -1;
    startTimer(200);
    this->moveToThread(&thd);
    thd.start();
}

RecDB::~RecDB()
{
    thd.quit();
    thd.wait();
    socket->deleteLater();
}

RecQR_Cnt RecDB::queryCnt()
{
    QMutexLocker locker(&m_mutex);
    RecQR_Cnt rqr(-1);
    QJsonObject jo = request("rec_queryCnt");
    if (jo.isEmpty())
        return rqr;

    rqr.err = 0;
    rqr.rcnt = jo.value("rcnt").toInt();
    rqr.dcnt = jo.value("dcnt").toInt();
    return rqr;
}

RecQR_Date RecDB::queryDate(int did, int n)
{
    QMutexLocker locker(&m_mutex);
    RecQR_Date rqr(-1);
    QJsonObject jo;
    jo.insert("did", did);
    jo.insert("n", n);
    jo = request("rec_queryDate", jo);
    if (jo.isEmpty())
        return rqr;

    rqr.err = 0;
    rqr.did = jo.value("did").toInt();
    int dn = jo.value("n").toInt();
    rqr.l.reserve(dn);
    jo = jo.value("z_list").toObject();
    QJsonArray ja_prm_id = jo.value("prm_id").toArray();
    QJsonArray ja_prm_name = jo.value("prm_name").toArray();
    QJsonArray ja_start_time = jo.value("start_time").toArray();
    QJsonArray ja_end_time = jo.value("end_time").toArray();
    QJsonArray ja_weight1 = jo.value("weight1").toArray();
    QJsonArray ja_weight2 = jo.value("weight2").toArray();
    QJsonArray ja_weight3 = jo.value("weight3").toArray();
    QJsonArray ja_speed = jo.value("speed").toArray();
    QJsonArray ja_hegeCnt = jo.value("hegeCnt").toArray();
    QJsonArray ja_totalWeight = jo.value("totalWeight").toArray();
    QJsonArray ja_totalPakCnt = jo.value("totalPakCnt").toArray();
    QJsonArray ja_prec = jo.value("prec").toArray();
    QJsonArray ja_unit = jo.value("unit").toArray();
    QJsonArray ja_drcnt = jo.value("drcnt").toArray();
    for (int i = 0; i < dn; i++) {
        Rec_Date r;
        r.prm_id = ja_prm_id.at(i).toInt();
        r.prm_name = ja_prm_name.at(i).toString();
        r.start_time = QDateTime::fromString(ja_start_time.at(i).toString(), Qt::ISODate);
        r.end_time = QDateTime::fromString(ja_end_time.at(i).toString(), Qt::ISODate);
        r.weight1 = ja_weight1.at(i).toInt();
        r.weight2 = ja_weight2.at(i).toInt();
        r.weight3 = ja_weight3.at(i).toInt();
        r.speed = ja_speed.at(i).toInt();
        r.hegeCnt = ja_hegeCnt.at(i).toInt();
        r.totalWeight = ja_totalWeight.at(i).toString().toLongLong();
        r.totalPakCnt = ja_totalPakCnt.at(i).toString().toLongLong();
        r.prec = ja_prec.at(i).toInt();
        r.unit = ja_unit.at(i).toInt();
        r.drcnt = ja_drcnt.at(i).toInt();
        rqr.l.append(r);
    }
    return rqr;
}

RecQR_DRec RecDB::queryDRec(int did, int rid, int n)
{
    QMutexLocker locker(&m_mutex);
    RecQR_DRec rqr(-1);
    QJsonObject jo;
    jo.insert("did", did);
    jo.insert("rid", rid);
    jo.insert("n", n);
    jo = request("rec_queryDRec", jo);
    if (jo.isEmpty())
        return rqr;

    rqr.err = 0;
    rqr.did = jo.value("did").toInt();
    rqr.rid = jo.value("rid").toInt();
    int rn = jo.value("n").toInt();
    rqr.l.reserve(rn);
    jo = jo.value("z_list").toObject();
    QJsonArray ja_weight = jo.value("weight").toArray();
    QJsonArray ja_time = jo.value("time").toArray();
    QJsonArray ja_hege = jo.value("hege").toArray();
    QJsonArray ja_prec = jo.value("prec").toArray();
    QJsonArray ja_unit = jo.value("unit").toArray();
    for (int i = 0; i < rn; i++) {
        Rec_Rec r;
        r.weight = ja_weight.at(i).toInt();
        r.time = QDateTime::fromString(ja_time.at(i).toString(), Qt::ISODate);
        r.hege = ja_hege.at(i).toInt();
        r.prec = ja_prec.at(i).toInt();
        r.unit = ja_unit.at(i).toInt();
        rqr.l.append(r);
    }
    return rqr;
}

RecQR_Handy RecDB::queryHandy(RecQ_Handy rq)
{
    QMutexLocker locker(&m_mutex);
    RecQR_Handy rqr(-1);
    QJsonObject jo;
    jo.insert("did", rq.did);
    jo.insert("dn", rq.dn);
    jo.insert("di", rq.di);
    jo.insert("drevs", rq.drevs);
    jo.insert("rid", rq.rid);
    jo.insert("rn", rq.rn);
    jo.insert("rrevs", rq.rrevs);
    jo = request("rec_queryHandy", jo);
    if (jo.isEmpty())
        return rqr;

    rqr.err = 0;
    rqr.rc.rcnt = jo.value("rcnt").toInt();
    rqr.rc.dcnt = jo.value("dcnt").toInt();
    rqr.did = jo.value("did").toInt();
    rqr.di = jo.value("di").toInt();
    rqr.rid = jo.value("rid").toInt();
    int dn = jo.value("dn").toInt();
    int rn = jo.value("rn").toInt();
    rqr.ld.reserve(dn);
    rqr.lr.reserve(rn);
    {
        QJsonObject ja = jo.value("z_list_r").toObject();
        QJsonArray ja_weight = ja.value("weight").toArray();
        QJsonArray ja_time = ja.value("time").toArray();
        QJsonArray ja_hege = ja.value("hege").toArray();
        QJsonArray ja_prec = ja.value("prec").toArray();
        QJsonArray ja_unit = ja.value("unit").toArray();
        for (int i = 0; i < rn; i++) {
            Rec_Rec r;
            r.weight = ja_weight.at(i).toInt();
            r.time = QDateTime::fromString(ja_time.at(i).toString(), Qt::ISODate);
            r.hege = ja_hege.at(i).toInt();
            r.prec = ja_prec.at(i).toInt();
            r.unit = ja_unit.at(i).toInt();
            rqr.lr.append(r);
        }
    }
    {
        QJsonObject ja = jo.value("z_list_d").toObject();
        QJsonArray ja_prm_id = ja.value("prm_id").toArray();
        QJsonArray ja_prm_name = ja.value("prm_name").toArray();
        QJsonArray ja_start_time = ja.value("start_time").toArray();
        QJsonArray ja_end_time = ja.value("end_time").toArray();
        QJsonArray ja_weight1 = ja.value("weight1").toArray();
        QJsonArray ja_weight2 = ja.value("weight2").toArray();
        QJsonArray ja_weight3 = ja.value("weight3").toArray();
        QJsonArray ja_speed = ja.value("speed").toArray();
        QJsonArray ja_hegeCnt = ja.value("hegeCnt").toArray();
        QJsonArray ja_totalWeight = ja.value("totalWeight").toArray();
        QJsonArray ja_totalPakCnt = ja.value("totalPakCnt").toArray();
        QJsonArray ja_prec = ja.value("prec").toArray();
        QJsonArray ja_unit = ja.value("unit").toArray();
        QJsonArray ja_drcnt = ja.value("drcnt").toArray();
        for (int i = 0; i < dn; i++) {
            Rec_Date r;
            r.prm_id = ja_prm_id.at(i).toInt();
            r.prm_name = ja_prm_name.at(i).toString();
            r.start_time = QDateTime::fromString(ja_start_time.at(i).toString(), Qt::ISODate);
            r.end_time = QDateTime::fromString(ja_end_time.at(i).toString(), Qt::ISODate);
            r.weight1 = ja_weight1.at(i).toInt();
            r.weight2 = ja_weight2.at(i).toInt();
            r.weight3 = ja_weight3.at(i).toInt();
            r.speed = ja_speed.at(i).toInt();
            r.hegeCnt = ja_hegeCnt.at(i).toInt();
            r.totalWeight = ja_totalWeight.at(i).toString().toLongLong();
            r.totalPakCnt = ja_totalPakCnt.at(i).toString().toLongLong();
            r.prec = ja_prec.at(i).toInt();
            r.unit = ja_unit.at(i).toInt();
            r.drcnt = ja_drcnt.at(i).toInt();
            rqr.ld.append(r);
        }
    }
    return rqr;
}

void RecDB::addRec(const RecSave_Rec_cbm5_1 &rsr)
{
    Q_UNUSED(rsr);
}

void RecDB::addRecStart(const Rec_Date &r)
{
    Q_UNUSED(r);
}

void RecDB::addRecEnd(const QDateTime &time)
{
    Q_UNUSED(time);
}

void RecDB::clear()
{
    QMutexLocker locker(&m_mutex);
    request("rec_clear");
}

QJsonObject RecDB::request(const QString &fun, const QJsonObject &content)
{
    QJsonObject joc;
    QByteArray data;
    myRandom = jtt.getRandom();
    jtt.writeJsonReq(socket, key, random, myRandom, fun, content);
    if (!jtt.waitForReadJson(socket, data))
        return joc;
    int len = strlen(data.constData());
    int len2 = strlen(data.constData() + len + 1);
    QByteArray json = QByteArray::fromRawData(data.constData(), len);
    QByteArray md5 = QByteArray::fromRawData(data.constData() + len + 1, len2);
    if (md5.size() != 32 || md5.toLower() != jtt.md5(json, key))
        return joc;

    QJsonObject jo = QJsonDocument::fromJson(json).object();
    data.clear();
    if (jo.isEmpty())
        return joc;
    if (myRandom != jo.value("random").toString())
        return joc;
    random = jo.value("myRandom").toString();
    joc = jo.value("z_content").toObject();
    return joc;
}

void RecDB::timerEvent(QTimerEvent *)
{
    RecQR_Cnt rc = queryCnt();
    if (rc.err)
        return;

    if (rc.rcnt != recCnt.rcnt || rc.dcnt != recCnt.dcnt) {
        recCnt = rc;
        emit recUpdate();
    }
}
#endif
