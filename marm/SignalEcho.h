#ifndef SIGNALECHO_H
#define SIGNALECHO_H

#include <QObject>
#include <QVariant>
#include <QMutexLocker>
#include <QVector>

// 用来配合实现函数的回调
class SignalEcho
{
    friend class SignalEchoManager;
public:
    SignalEcho() : dis(0), id(-1) {}
    inline bool isValid() const { return id >= 0; }
    inline bool operator ==(const SignalEcho &o) const { return id == o.id; }
    inline bool operator !=(const SignalEcho &o) const { return !(*this == o); }
    QVariant val;

    int dis : 1;
    int id : 31;
};
typedef SignalEcho SignalEcho_zy1;
#define Q_ARG_SignalEcho(data) Q_ARG(SignalEcho_zy1, data)

class SignalEchoIds
{
    friend class SignalEchoManager;
private:
    QVector<int> v;
};

// 用来配合实现函数的回调
class SignalEchoManager
{
public:
    SignalEchoManager();
    ~SignalEchoManager();

    // 获取一个属于obj的echo。该函数一般在obj的构造函数里调用
    // ids存放obj所有已获得的echo的id，member指定echo要回调的函数
    void create(SignalEcho &ec, SignalEchoIds &ids,
                QObject *obj, const char *member);

    // 释放ids中所有的echo。该函数必须在obj的析构函数里调用
    void destroy(SignalEchoIds &ids, QObject *obj);

    // 重新获取echo。如果重新获取了echo，
    // 则在回调函数里必须先判断echo是否相等
    void reCreate(SignalEcho &ec, SignalEchoIds &ids);

    // 准备回调n次
    void from(const SignalEcho &ec, int n = 1);

    // 完成回调n次
    void finish(const SignalEcho &ec, int n = 1);

    // 空回调n次
    void echoEmpty(const SignalEcho &ec, int n = 1);

    // 回调1次
    void echo(const SignalEcho &ec, Qt::ConnectionType type,
              QGenericArgument val0 = QGenericArgument(0),
              QGenericArgument val1 = QGenericArgument(),
              QGenericArgument val2 = QGenericArgument(),
              QGenericArgument val3 = QGenericArgument(),
              QGenericArgument val4 = QGenericArgument(),
              QGenericArgument val5 = QGenericArgument(),
              QGenericArgument val6 = QGenericArgument(),
              QGenericArgument val7 = QGenericArgument(),
              QGenericArgument val8 = QGenericArgument(),
              QGenericArgument val9 = QGenericArgument());
    void echo(const SignalEcho &ec,
              QGenericArgument val0 = QGenericArgument(0),
              QGenericArgument val1 = QGenericArgument(),
              QGenericArgument val2 = QGenericArgument(),
              QGenericArgument val3 = QGenericArgument(),
              QGenericArgument val4 = QGenericArgument(),
              QGenericArgument val5 = QGenericArgument(),
              QGenericArgument val6 = QGenericArgument(),
              QGenericArgument val7 = QGenericArgument(),
              QGenericArgument val8 = QGenericArgument(),
              QGenericArgument val9 = QGenericArgument())
    {
        echo(ec, Qt::AutoConnection,
             val0, val1, val2, val3, val4,
             val5, val6, val7, val8, val9);
    }

private:
    struct SignalEchoInfo {
        int state : 1;
        int countw : 31;
        int count;
        QObject *obj;
        const char *member;
        SignalEchoInfo() {
            state = countw = count = 0;
            obj = 0;
        }
    };

private:
    QMutex mutex;
    int m_ccnt;
    int m_ucnt;
    int m_size;
    int m_ssize;
    int m_curid;
    QVector<SignalEchoInfo> m_data;
};

#endif // SIGNALECHO_H
