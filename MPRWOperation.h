#ifndef MPRWOPERATION_H
#define MPRWOPERATION_H

#include <QObject>
#include <QThread>
#include <QEventLoop>
#include "Machine.h"
#include "MachineParamList0.h"
#include "FormA.h"

class MPRWOperation : public QObject
{
    Q_OBJECT
public:
    explicit MPRWOperation(int maddr, MachineComm *comm);
    ~MPRWOperation();

public slots:
    void Import(const QString &name);
    void Export(const QString &name);
    void cancel() { m_cancel = true; }

public:
    static QString errStr(int err);
    
signals:
    // 当前进度信号
    // cur==total:正常完成
    // cur=-1:错误(未指出错误原因)
    // cur=-2:被取消
    // cur=-3:通信错误
    // cur=-4:写入文件失败
    // cur=-5:读入的文件格式错误
    // cur=-6:未检测到U盘
    void progress(int cur, int total);

private slots:
    void onMPRWFns(int err, const SignalEcho_zy1 &ec);

private:
    inline int waitMPRW() { return loop->exec(); }
    int waitMPRW_Import(QDataStream &in, MRegRWInfo &r, int &curp, int totalp);
    int waitMPRW_Export(QDataStream &out, MRegRWInfo &r, int &curp, int totalp);
    int waitMPRW_Import(QDataStream &in, MRegRWInfo *rs, int &curp, int totalp);
    int waitMPRW_Export(QDataStream &out, MRegRWInfo *rs, int &curp, int totalp);

    void writeCMD(quint16 cmd, quint16 cmdp);
    inline void writeCMD(quint16 cmdH, quint16 cmdL, quint16 cmdpH, quint16 cmdpL)
    { writeCMD((cmdH<<8)|(cmdL), (cmdpH<<8)|(cmdpL)); }

private:
    QThread thd;
    QEventLoop *loop;
    MachineParamList0 mp;
    Machine *mac;
    SignalEcho echo;
    SignalEchoIds ecIds;
    bool m_cancel;
};

#endif // MPRWOPERATION_H
