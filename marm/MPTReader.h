#ifndef MPTREADER_H
#define MPTREADER_H

#include <QObject>
#include <QTimer>
#include "Machine.h"

// 机器参数定时读取器
// 定时读取某些参数，先读一次，然后每隔一段时间(以低优先级)读一次
class MPTReader : public QObject
{
    Q_OBJECT
public:
    // Once：只读取一次
    // Delay：每次收到读取回复后，等待一个间隔时间再读取
    // Interval：每隔一段时间读取。如果间隔时间到了却还没有收到上次读取的回复，将会等到下个间隔时间到再读
    enum ReadMode {Once, Delay, Interval};

    // 如果interval不为0，则mode默认为Delay
    MPTReader(QObject *parent, Machine *mac, const QList<const MParam *> &mp, int interval = 0, ReadMode mode = Once);
    MPTReader(QObject *parent, Machine *mac, const MParam *mp, int interval = 0, ReadMode mode = Once);
    MPTReader(QObject *parent, Machine *mac, const QList<MRegRWInfo> &mp, int interval = 0, ReadMode mode = Once);
    MPTReader(QObject *parent, Machine *mac, const MRegRWInfo &mp, int interval = 0, ReadMode mode = Once);
    ~MPTReader();

    void appendMP(const QList<const MParam *> &mp);
    void appendMP(const MParam *mp);
    void appendMP(const QList<MRegRWInfo> &mp);
    void appendMP(const MRegRWInfo &mp);

    ushort p; // 为1：除了第一次读取都是低优先级，为0：都不是低优先级，默认为1
    uchar s;  // 为1：每读取一次会发送一个readFinished信号，默认为0
    uchar s1; // 为1：每次readOnce会发送一个read1Finished信号，默认为0

private:
    void init(Machine *mac, int interval, ReadMode mode);

public slots:
    // 读一次，不会影响到间隔时间
    void readOnce();
    void start();
    void stop();

signals:
    void readSkip();
    void readFinished(int err);
    void read1Finished(int err);

private slots:
    void onTimerRead();
    void onActionFinished(int err, const SignalEcho_zy1 &echo);

private:
    Machine *m_mac;
    QList<Machine::Action> m_list;
    ReadMode m_mode;

    bool isStart;
    bool isReadFirt;
    int isReadWating;
    QTimer *timerRead;
    SignalEcho m_echo;
    SignalEcho m_echo1;
    SignalEchoIds m_ecIds;
};


#include <QWidget>

// 该Widget不可用，只是为了监听父Widget的showEvent和hideEvent
// 执行showEvent时开始读取，执行hideEvent时暂停读取
// 但是不敢保证该Widget与父Widget之间的showEvent和hideEvent执行顺序
class MPTRWidget : public QWidget
{
    Q_OBJECT
public:
    MPTReader reader;
    bool start_later;

    MPTRWidget(QWidget *parent, Machine *mac, const QList<const MParam *> &mps, int interval = 0, MPTReader::ReadMode mode = MPTReader::Once) :
        QWidget(parent),
        reader(this, mac, mps, interval, mode),
        start_later(false)
    {
        setFixedSize(0, 0);
    }
    MPTRWidget(QWidget *parent, Machine *mac, const MParam *mp, int interval = 0, MPTReader::ReadMode mode = MPTReader::Once) :
        QWidget(parent),
        reader(this, mac, mp, interval, mode),
        start_later(false)
    {
        setFixedSize(0, 0);
    }
    MPTRWidget(QWidget *parent, Machine *mac, const QList<MRegRWInfo> &list, int interval = 0, MPTReader::ReadMode mode = MPTReader::Once) :
        QWidget(parent),
        reader(this, mac, list, interval, mode),
        start_later(false)
    {
        setFixedSize(0, 0);
    }
    MPTRWidget(QWidget *parent, Machine *mac, const MRegRWInfo &info, int interval = 0, MPTReader::ReadMode mode = MPTReader::Once) :
        QWidget(parent),
        reader(this, mac, info, interval, mode),
        start_later(false)
    {
        setFixedSize(0, 0);
    }

public slots:
    void readOnce() { reader.readOnce(); }

protected:
    virtual void showEvent(QShowEvent *)
    {
        if (!start_later) reader.start();
        else QMetaObject::invokeMethod(&reader, "start", Qt::QueuedConnection);
    }
    virtual void hideEvent(QHideEvent *) { reader.stop(); }
};

#endif // MPTREADER_H
