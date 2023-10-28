#ifndef RECSAVE_H
#define RECSAVE_H

#include <QThread>
#include <QEventLoop>
#include "FormA.h"
#include "RecDB.h"

class RecSaveWorker : public QObject
{
    Q_OBJECT
public:
    explicit RecSaveWorker(RecDB *rdb, QObject *parent = 0);
public slots:
    void addRec(const QVariant &v);
    void addRecStart(const QVariant &v);
    void addRecEnd(const QVariant &v);
private:
    RecDB *recDB;
};

class RecSave : public QObject
{
    Q_OBJECT
public:
    explicit RecSave(FormA *f, RecDB *rdb);
    ~RecSave();

    int error;
    QThread thd;
    RecSaveWorker worker;

    struct PFInfo {
        QString name;
        int setw[3];
        int worP;
        int speed;
        int afc;
        int dww;
        int dwwup;
        int dwwdn;
        int dwwai;
        PFInfo() {
            setw[0] = setw[1] = setw[2] = 0;
            worP = speed = afc = 0;
            dww = dwwup = dwwdn = dwwai = 0;
        }
    };
    PFInfo pf[100];
    QMutex pf_mutex;

signals:
    void readFns(int addr, QByteArray ba);
    void errChanged(int err);

private slots:
    void onReadFirst();
    void onReadRec();
    void onReadPF();
    void onReadFns(int err, const SignalEcho_zy1 &ec);
    void onReadFnsFF(int err, const SignalEcho_zy1 &ec);
    void onReadFF();

private:
    void savePFInfo();
    void trySaveRec();
    void addRecStart();
    void addRecEnd();

private:
    QThread workerThd;
    MachineParamList0 mp;
    Machine *mac;
    MRegRWInfo rir; // 记录读取信息
    MRegRWInfo rip; // 配方读取信息
    MRegRWInfo ris; // 系统读取信息
    QEventLoop *loop;
    SignalEcho echoff;
    SignalEcho echof;
    SignalEcho echor;
    SignalEcho echop;
    SignalEchoIds ecIds;

    FormA *form;
    bool isRun;
    bool isAddStart;
    uint rn;
    uint rn2;
    RecSave_Rec rrs;
    int rrs_pfi;

public:
    QDateTime dt();
    void writeCMD(quint16 cmd, quint16 cmdp);
    inline void writeCMD(quint16 cmdH, quint16 cmdL, quint16 cmdpH, quint16 cmdpL)
    { writeCMD((cmdH<<8)|(cmdL), (cmdpH<<8)|(cmdpL)); }

    Q_SLOT void mc_in_tweight(int w);
    Q_SIGNAL void inTWeightChanged(int w);
};

#endif // RECSAVE_H
