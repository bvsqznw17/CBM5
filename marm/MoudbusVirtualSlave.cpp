#include "MoudbusVirtualSlave.h"
#include "CRC.h"
#include <QDataStream>
#include <QDebug>

MoudbusVirtualSlave::MoudbusVirtualSlave(QObject *parent, int reg_size) :
    QIODevice(parent),
    m_reg(reg_size)
{
    b_process_inv = false;
}

qint64 MoudbusVirtualSlave::readData(char *data, qint64 maxlen)
{
    QMutexLocker locker(&m_mutex);

    qint64 len = (uint)m_send.size() <= (quint64)maxlen ? m_send.size() : maxlen;
    memcpy(data, m_send.constData(), len);
    m_send.remove(0, len);
    return len;
}

qint64 MoudbusVirtualSlave::writeData(const char *data, qint64 len)
{
    QMutexLocker locker(&m_mutex);

    m_recv.append(data, len);

    if (!b_process_inv) {
        b_process_inv = true;
        QMetaObject::invokeMethod(this, "process_inv", Qt::QueuedConnection);
    }
    return len;
}

void MoudbusVirtualSlave::process_inv()
{
    QMutexLocker locker(&m_mutex);

    b_process_inv = false;
    QByteArray recv = m_recv;
    m_recv.clear();

    if (recv.size() < 4 || recv.size() > 256) {
        qDebug() << "frame size err" << recv.toHex().toUpper();
        return;
    }

    if (!crc16_check(recv)) {
        qDebug() << "frame crc err" << recv.toHex().toUpper();
        return;
    }

    QByteArray reply;
    QDataStream out(&reply, QIODevice::ReadWrite);
    QDataStream in(&recv, QIODevice::ReadOnly);
    uchar mac; in >> mac;
    uchar fun; in >> fun;

    switch (fun) {
    case 0x03: {
        ushort addr; in >> addr;
        ushort count; in >> count;
        uchar len = count * 2;
        if (addr + count >= m_reg.size())
            break;

        out << mac;
        out << fun;
        out << len;
        for (ushort i = addr; i < addr + count; i++) {
            ushort data;
            data = m_reg.at(i);
            out << data;
        }
        out << (ushort)0;
        crc16_make(reply);
        m_send.append(reply);
        break;
    }
    case 0x10: {
        ushort addr; in >> addr;
        ushort count; in >> count;
        uchar len; in >> len;
        if (addr + count >= m_reg.size())
            break;

        ushort *regd = m_reg.data();
        for (ushort i = addr; i < addr + count; i++) {
            ushort data; in >> data;
            regd[i] = data;
        }
        out << mac;
        out << fun;
        out << addr;
        out << count;
        out << (ushort)0;
        crc16_make(reply);
        m_send.append(reply);
        break;
    }
    default:
        break;
    }
}
