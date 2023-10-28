#ifndef PRINTTHREAD_H
#define PRINTTHREAD_H

#include <QThread>
#include <QMutex>

class PrintThread : public QThread
{
    Q_OBJECT
public:
    explicit PrintThread(QObject *parent = nullptr);

    QString m_str;
    static QMutex mutex;
    static void printString(const QString &str);

private:
    void run();
};

#endif // PRINTTHREAD_H
