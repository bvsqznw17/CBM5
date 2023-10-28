#include "WiFiCheckThread.h"
#include <QProcess>
#include <QFile>

WiFiCheckThread g_wific;
WiFiCheckThread::WiFiCheckThread(QObject *parent) : QThread(parent)
{
    m_run = true;
}

WiFiCheckThread::~WiFiCheckThread()
{
    m_run = false;
    wait();
}

#include <QDebug>
void WiFiCheckThread::run()
{
#ifdef __ARM_EABI__
    bool f = true;
    while (m_run) {
        QProcess p;
        p.start("route", QStringList() << "-n");
        p.waitForFinished(3000);
        QStringList list = QString::fromUtf8(p.readAllStandardOutput()).split('\n');

        bool b = false;
        for (int i = 0; i < list.size(); i++) {
            if (!list.at(i).endsWith("wlan0"))
                continue;
            QStringList list0 = list.at(i).split(QRegExp(" +"));
            if (list0.size() > 1 && list0.at(1) != "*" && list0.at(1) != "0.0.0.0") {
                p.start("ping", QStringList() << list0.at(1) << "-c1");
                p.waitForFinished(3000);
                if (!p.exitCode()) {
                    b = true;
                    break;
                }
            }
        }

        if (f || b != m_conn) {
            f = false;
            m_conn = b;
            emit stateChanged(b);
        }

        if (!m_run)
            break;
        msleep(1000);
    }
#endif
}
