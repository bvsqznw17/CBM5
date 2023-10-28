#include "MPTReader.h"

MPTReader::MPTReader(QObject *parent, Machine *mac, const QList<const MParam *> &mp, int interval, MPTReader::ReadMode mode) :
    QObject(parent)
{
    appendMP(mp);
    init(mac, interval, mode);
}

MPTReader::MPTReader(QObject *parent, Machine *mac, const MParam *mp, int interval, MPTReader::ReadMode mode) :
    QObject(parent)
{
    appendMP(mp);
    init(mac, interval, mode);
}

MPTReader::MPTReader(QObject *parent, Machine *mac, const QList<MRegRWInfo> &mp, int interval, ReadMode mode) :
    QObject(parent)
{
    appendMP(mp);
    init(mac, interval, mode);
}

MPTReader::MPTReader(QObject *parent, Machine *mac, const MRegRWInfo &mp, int interval, ReadMode mode) :
    QObject(parent)
{
    appendMP(mp);
    init(mac, interval, mode);
}

MPTReader::~MPTReader()
{
    m_mac->echom.destroy(m_ecIds, this);
}

void MPTReader::appendMP(const QList<const MParam *> &mp)
{
    Machine::Action a;
    a.type = Machine::ReadParam;
    a.listm = mp;
    m_list.append(a);
}

void MPTReader::appendMP(const MParam *mp)
{
    Machine::Action a;
    a.type = Machine::ReadParam;
    a.listm.append(mp);
    m_list.append(a);
}

void MPTReader::appendMP(const QList<MRegRWInfo> &mp)
{
    Machine::Action a;
    a.type = Machine::ReadReg;
    a.listr = mp;
    m_list.append(a);
}

void MPTReader::appendMP(const MRegRWInfo &mp)
{
    Machine::Action a;
    a.type = Machine::ReadReg;
    a.listr.append(mp);
    m_list.append(a);
}

void MPTReader::init(Machine *mac, int interval, ReadMode mode)
{
    p = 1;
    s = 0;
    s1 = 0;
    m_mac = mac;
    m_mode = mode;
    if (interval != 0 && m_mode == Once)
        m_mode = Delay;

    isStart = false;
    isReadFirt = false;
    isReadWating = false;
    timerRead = new QTimer(this);
    timerRead->setInterval(interval);
    timerRead->setSingleShot(m_mode == Delay);
    connect(timerRead, SIGNAL(timeout()), this, SLOT(onTimerRead()));
    m_mac->echom.create(m_echo, m_ecIds, this, "onActionFinished");
}

void MPTReader::readOnce()
{
    if (s1 && !m_echo1.isValid())
        m_mac->echom.create(m_echo1, m_ecIds, this, "onActionFinished");
    for (int i = 0; i < m_list.size(); i++) {
        m_list[i].echo = m_echo1;
        m_mac->request(m_list[i]);
    }
}

void MPTReader::start()
{
    if (!isStart) {
        isStart = true;
        isReadFirt = true;

        switch (m_mode) {
        case Once:
            onTimerRead();
            break;
        case Delay:
            onTimerRead();
            break;
        case Interval:
            timerRead->start();
            onTimerRead();
            break;
        }
    }
}

void MPTReader::stop()
{
    if (isStart) {
        isStart = false;
        isReadWating = false;
        m_mac->clearAction(m_echo, m_ecIds);

        switch (m_mode) {
        case Once:
            break;
        case Delay:
            timerRead->stop();
            break;
        case Interval:
            timerRead->stop();
            break;
        }
    }
}

void MPTReader::onTimerRead()
{
    if (m_mode == Interval && isReadWating) {
        if (s)
            emit readSkip();
        return;
    }

    uint pp = isReadFirt ? 0 : p;
    isReadFirt = false;

    bool _bkb = m_mac->onf_linkfram[pp];
    m_mac->onf_linkfram[pp] = 0; // 停用连帧

    for (int i = 0; i < m_list.size(); i++) {
        m_list[i].echo = m_echo;
        m_mac->request(m_list[i]);
    }
    isReadWating = m_list.size();

    m_mac->onf_linkfram[pp] = _bkb;
}

void MPTReader::onActionFinished(int err, const SignalEcho &echo)
{
    m_mac->echom.finish(echo);
    if (echo == m_echo) {
        isReadWating--;

        if (!isReadWating && m_mode == Delay)
            timerRead->start();

        if (s)
            emit readFinished(err);
    }
    else if (echo == m_echo1) {
        if (s1)
            emit read1Finished(err);
    }
}
