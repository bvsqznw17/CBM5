#ifndef MOUDBUSVIRTUALSLAVE_H
#define MOUDBUSVIRTUALSLAVE_H

#include <QIODevice>
#include <QMutex>

class MoudbusVirtualSlave : public QIODevice
{
    Q_OBJECT
public:
    explicit MoudbusVirtualSlave(QObject *parent = 0, int reg_size = 5000);

    // QIODevice interface
protected:
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

private:
    Q_INVOKABLE void process_inv();
    bool b_process_inv;

private:
    QMutex m_mutex;
    QVector<ushort> m_reg;
    QByteArray m_recv;
    QByteArray m_send;
};

#endif // MOUDBUSVIRTUALSLAVE_H
