#ifndef MACHINE_H
#define MACHINE_H

#include <QObject>
#include <QQueue>
#include "SignalEcho.h"
#include "MachineComm.h"
#include "MachineParamList.h"

class Machine : public QObject
{
    Q_OBJECT
public:
    SignalEchoManager echom;

    explicit Machine(int maddr, MachineComm *comm,
                     MachineParamList *mplist,
                     QObject *parent = 0);
    ~Machine();

    inline int maddr() const { return m_maddr; }
    inline MachineComm *mcomm() const { return m_comm; }
    inline bool isconn() const { return m_isconn; }
    inline bool isfirstconn() const { return m_isfirstconn; }
    inline bool isconnect() const { return m_isconn || m_isfirstconn; }

    // 读寄存器
    void read(const MRegRWInfo &info, const SignalEcho &echo = SignalEcho(), uint p = 0);
    void read(const QList<MRegRWInfo> &list, const SignalEcho &echo = SignalEcho(), uint p = 0);
    // 写寄存器
    void write(const MRegRWInfo &info, const SignalEcho &echo = SignalEcho(), uint p = 0);
    void write(const QList<MRegRWInfo> &list, const SignalEcho &echo = SignalEcho(), uint p = 0);
    // 写寄存器，但不会更新本地寄存器缓存
    void writeNoRU(const MRegRWInfo &info, const SignalEcho &echo = SignalEcho(), uint p = 0);
    void writeNoRU(const QList<MRegRWInfo> &list, const SignalEcho &echo = SignalEcho(), uint p = 0);

    // 读参数
    void read(const MParam *mp, const SignalEcho &echo = SignalEcho(), uint p = 0);
    void read(const QList<const MParam *> &mps, const SignalEcho &echo = SignalEcho(), uint p = 0);
    // 写参数
    void write(const MParam *mp, const QVariant &value, const SignalEcho &echo = SignalEcho(), uint p = 0);
    void write(const QList<const MParam *> &mps, const QVariantList &values, const SignalEcho &echo = SignalEcho(), uint p = 0);
    // 写参数，但不会更新本地寄存器缓存
    void writeNoRU(const MParam *mp, const QVariant &value, const SignalEcho &echo = SignalEcho(), uint p = 0);
    void writeNoRU(const QList<const MParam *> &mps, const QVariantList &values, const SignalEcho &echo = SignalEcho(), uint p = 0);

    // 参数p为1表示滞后(低优先级)处理

    // 以下为“不传入echo但传入p”的重构函数
    inline void read(const MRegRWInfo &info, uint p)
    { read(info, SignalEcho(), p); }
    inline void read(const QList<MRegRWInfo> &list, uint p)
    { read(list, SignalEcho(), p); }
    inline void write(const MRegRWInfo &info, uint p)
    { write(info, SignalEcho(), p); }
    inline void write(const QList<MRegRWInfo> &list, uint p)
    { write(list, SignalEcho(), p); }
    inline void writeNoRU(const MRegRWInfo &info, uint p)
    { writeNoRU(info, SignalEcho(), p); }
    inline void writeNoRU(const QList<MRegRWInfo> &list, uint p)
    { writeNoRU(list, SignalEcho(), p); }
    inline void read(const MParam *mp, uint p)
    { read(mp, SignalEcho(), p); }
    inline void read(const QList<const MParam *> &mps, uint p)
    { read(mps, SignalEcho(), p); }
    inline void write(const MParam *mp, const QVariant &value, uint p)
    { write(mp, value, SignalEcho(), p); }
    inline void write(const QList<const MParam *> &mps, const QVariantList &values, uint p)
    { write(mps, values, SignalEcho(), p); }
    inline void writeNoRU(const MParam *mp, const QVariant &value, uint p)
    { writeNoRU(mp, value, SignalEcho(), p); }
    inline void writeNoRU(const QList<const MParam *> &mps, const QVariantList &values, uint p)
    { writeNoRU(mps, values, SignalEcho(), p); }

    bool onf_priority; // 为0：所有请求都会加入到最高优先级的队列，默认为1
    int n_resentcnt; // 超时重发次数，默认为5

    static const int P = 2;
    bool onf_linkfram[P]; // “连帧”控制，0:停用；1:启用；默认只有最高优先级启用
    short n_linkfram[P];

    // 清除队列中跟指定echo相关的请求
    void clearAction(SignalEcho &echo, SignalEchoIds &ids);

signals:
    void disconn();
    void reconn();
    void regUpdated(const MRegAreas &areas);

public:
    // 一个请求会用一个Action来存储，一个请求可能会被分成多个帧来发送
    enum ActionType {
        ReadReg, WriteReg, WriteRegNoRU,
        ReadParam, WriteParam, WriteParamNoRU
    };
    struct Action {
        ActionType type;
        short n_linkfram;
        short n_resentcnt;
        SignalEcho echo;
        QList<MRegRWInfo> listr;
        QList<const MParam *> listm;
        QVariantList listv;
    };

    // 直接请求
    inline void request(Action &a, uint p = 0) { enqueue(a, p); }

private:
    MachineComm *m_comm;
    MachineParamList *m_mplist;
    int m_maddr;
    bool m_isconn;
    bool m_isfirstconn;

    SignalEcho m_echo;
    SignalEcho m_echof;
    SignalEchoIds m_ecIds;

    QList<QList<const MParam *> > firstReadMpslist0;
    QList<QList<const MParam *> > firstReadMpslist;

    // 优先级
    int p_cur;
    int p_cnt[P];
    // 发送队列
    QQueue<QList<Action> > q_act[P];
    QList<Action> q_enq[P];
    bool q_isenq;
    // 当前发送
    QList<Action> c_act;
    QList<int> c_acnt;
    int c_acti;
    int c_cnta;
    int c_cntf;

    MRegAreas regAreas;
    QList<QByteArray> datas;

private:
    Q_SLOT void clearAction_p(const SignalEcho_zy1 &echo);
    void enqueue(Action &a, uint p);
    Q_SLOT void enqueueFinished();
    void nextProcess();
    QList<MRegRWInfo> genMRegRWInfo(const Action &a);
    MCRequest genMCRequest(const MRegRWInfo &r, const Action &a);
    Q_SLOT void nextFirstReadMpslist();
    Q_SLOT void onResponse(int err,
                           const MCRequest_zy1 &req,
                           const MCResponse_zy1 &rsp,
                           const SignalEcho_zy1 &echo);
};

#endif // MACHINE_H
