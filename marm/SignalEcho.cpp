#include "SignalEcho.h"
#include <QMetaType>

SignalEchoManager::SignalEchoManager() :
    mutex(QMutex::Recursive)
{
    qRegisterMetaType<SignalEcho>("SignalEcho_zy1");
    m_ccnt = 0;
    m_ucnt = 0;
    m_size = 0;
    m_ssize = 0;
    m_curid = -1;
}

SignalEchoManager::~SignalEchoManager()
{
    QMutexLocker locker(&mutex);
#ifdef MYOPT_DEBUG
    if (m_ccnt != 0)
        qWarning("SignalEchoManager::~SignalEchoManager: error");
    //qDebug("echom:%lx ssize:%d ccnt:%d ucnt:%d", (ulong)this, m_ssize, m_ccnt, m_ucnt);
    for (int i = 0; i < m_ssize; i++)
        if (m_data.at(i).countw != 0)
            qDebug("data[%d].countw = %d", i, m_data.at(i).countw);
#endif
    m_data.clear();
}

void SignalEchoManager::create(SignalEcho &ec, SignalEchoIds &ids,
                               QObject *obj, const char *member)
{
    QMutexLocker locker(&mutex);

    if (m_ucnt >= m_size) {
        m_size += 12;
        m_ssize += 16;
        m_data.resize(m_ssize);
    }

    do {
        m_curid++;
        m_curid %= m_ssize;
    } while (m_data.at(m_curid).obj);
    m_data[m_curid].state = 1;
    m_data[m_curid].obj = obj;
    m_data[m_curid].member = member;

#ifdef MYOPT_DEBUG
    m_ccnt++;
#endif
    m_ucnt++;
    ec.dis = 0;
    ec.id = m_curid;
    ids.v.append(ec.id);
}

void SignalEchoManager::destroy(SignalEchoIds &ids, QObject *obj)
{
    QMutexLocker locker(&mutex);

    for (int i = 0; i < ids.v.size(); i++) {
        int id = ids.v.at(i);
        if (m_data.at(id).obj != obj)
            continue;
#ifdef MYOPT_DEBUG
        if (m_data.at(id).state) {
            m_ccnt--;
            m_data[id].state = 0;
        }
#else
        m_data[id].state = 0;
#endif
        m_data[id].count -= m_data.at(id).countw;
        m_data[id].countw = 0;
        if (m_data.at(id).count == 0) {
            // state和count都为0才算完全释放
            m_ucnt--;
            // 若obj为0则表示state和count都已为0
            m_data[id].obj = 0;
        }
    }
    ids.v.clear();
}

void SignalEchoManager::reCreate(SignalEcho &ec, SignalEchoIds &ids)
{
    QMutexLocker locker(&mutex);
    int id = ec.id;
#ifdef MYOPT_DEBUG
    if ((uint)id >= (uint)m_ssize || m_data.at(id).state == 0) {
        qWarning("SignalEchoManager::reCreate: error");
        return;
    }
#endif
    QObject *obj = m_data.at(id).obj;
    const char *member = m_data.at(id).member;

#ifdef MYOPT_DEBUG
    m_ccnt--;
#endif
    m_data[id].state = 0;
    if (m_data.at(id).count == 0) {
        m_ucnt--;
        m_data[id].obj = 0;
    }

    for (int i = ids.v.size() - 1; i >= 0; i--) {
        if (m_data.at(ids.v.at(i)).obj == obj)
            continue;
        ids.v.remove(i);
    }

    create(ec, ids, obj, member);
    //qDebug("ucnt: %d", m_ucnt);
}

void SignalEchoManager::from(const SignalEcho &ec, int n)
{
    QMutexLocker locker(&mutex);
    int id = ec.id;
#ifdef MYOPT_DEBUG
    if ((uint)id >= (uint)m_ssize || m_data.at(id).state == 0) {
        qWarning("SignalEchoManager::from: error");
        return;
    }
    int oldc = m_data.at(id).count;
#endif
    m_data[id].count += n;
#ifdef MYOPT_DEBUG
    if (m_data.at(id).count < oldc)
        qWarning("SignalEchoManager::from: m_data.at(id).count over");
#endif
}

void SignalEchoManager::finish(const SignalEcho &ec, int n)
{
    QMutexLocker locker(&mutex);
    int id = ec.id;
#ifdef MYOPT_DEBUG
    if ((uint)id >= (uint)m_ssize) {
        qWarning("SignalEchoManager::finish: error");
        return;
    }
    int oldc = m_data.at(id).count;
#endif
    m_data[id].count -= n;
    m_data[id].countw -= n;
    if (m_data.at(id).count == 0 && m_data.at(id).state == 0) {
        m_ucnt--;
        m_data[id].obj = 0;
    }
#ifdef MYOPT_DEBUG
    if ((uint)m_data.at(id).count > (uint)oldc)
        qWarning("SignalEchoManager::finish: m_data.at(id).count < 0");
#endif
}

void SignalEchoManager::echoEmpty(const SignalEcho &ec, int n)
{
    QMutexLocker locker(&mutex);
    int id = ec.id;
#ifdef MYOPT_DEBUG
    if ((uint)id >= (uint)m_ssize) {
        qWarning("SignalEchoManager::echoEmpty: error");
        return;
    }
    int oldc = m_data.at(id).count;
#endif
    m_data[id].count -= n;
    if (m_data.at(id).count == 0 && m_data.at(id).state == 0) {
        m_ucnt--;
        m_data[id].obj = 0;
    }
#ifdef MYOPT_DEBUG
    if ((uint)m_data.at(id).count > (uint)oldc)
        qWarning("SignalEchoManager::echoEmpty: m_data.at(id).count < 0");
#endif
}

void SignalEchoManager::echo(const SignalEcho &ec, Qt::ConnectionType type,
                             QGenericArgument val0, QGenericArgument val1,
                             QGenericArgument val2, QGenericArgument val3,
                             QGenericArgument val4, QGenericArgument val5,
                             QGenericArgument val6, QGenericArgument val7,
                             QGenericArgument val8, QGenericArgument val9)
{
    QMutexLocker locker(&mutex);
    int id = ec.id;
#ifdef MYOPT_DEBUG
    if ((uint)id >= (uint)m_ssize) {
        qWarning("SignalEchoManager::echo: error");
        return;
    }
#endif

    if (m_data.at(id).state != 0 && !ec.dis) {
        m_data[id].countw++;
        QObject *obj = m_data.at(id).obj;
        const char *member = m_data.at(id).member;
        QMetaObject::invokeMethod(obj, member, type,
                                  val0, val1, val2, val3, val4,
                                  val5, val6, val7, val8, val9);
    }
    else {
        m_data[id].count--;
        if (m_data.at(id).count == 0 && m_data.at(id).state == 0) {
            m_ucnt--;
            m_data[id].obj = 0;
        }
#ifdef MYOPT_DEBUG
        if (m_data.at(id).count == -1)
            qWarning("SignalEchoManager::echo: m_data.at(id).count == -1");
#endif
    }
}
