#ifndef WIFICHECKTHREAD_H
#define WIFICHECKTHREAD_H

#include <QThread>

extern class WiFiCheckThread g_wific;
class WiFiCheckThread : public QThread
{
    Q_OBJECT
public:
    explicit WiFiCheckThread(QObject *parent = nullptr);
    ~WiFiCheckThread();

signals:
    void stateChanged(bool conn);

private:
    bool m_conn;
    bool m_run;
    void run();
};

#endif // WIFICHECKTHREAD_H
