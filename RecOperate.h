#ifndef RECOPERATE_H
#define RECOPERATE_H

#include "RecDB.h"
#include <QThread>

class RecOperateWorker : public QObject
{
    Q_OBJECT
public:
    explicit RecOperateWorker(RecDB *rdb, QObject *parent = 0);
    void cancel(bool b) { m_cancel = b; }

signals:
    void queryFns(QVariant v);

    // 当前进度信号
    // cur==total:正常完成
    // cur=-1:错误(未指出错误原因)
    // cur=-2:被取消
    // cur=-3:通信错误
    // cur=-4:写入文件失败
    // cur=-5:读入的文件格式错误
    // cur=-6:未检测到U盘
    void progress(int m_pCur, int m_pTotal);

public slots:
    void query(QVariant v);
    void queryDRec(int did);
    void clear();
    void exportRec(QString name);

private:
    RecDB *recDB;
    bool m_cancel;
};

class RecOperate : public QObject
{
    Q_OBJECT
public:
    RecOperate(RecDB *rdb, QObject *parent = 0);
    ~RecOperate();

    RecOperateWorker worker;
    void query(const RecQ_Handy &rq);
    void queryDRec(int did);
    void clear();
    void exportRec(const QString &name);

public slots:
    void cancel() { worker.cancel(true); }

private:
    QThread thread;
};

#endif // RECOPERATE_H
