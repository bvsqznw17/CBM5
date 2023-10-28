#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include <QThread>
#include <QMutex>
#include <QImage>

class CameraThread : public QThread
{
    Q_OBJECT
public:
    explicit CameraThread(QObject *parent = 0);

    QString dev;
    int width;
    int height;

    void open();
    void close();

signals:
    void imageUpdate(QImage img);

private:
    bool m_run;
    int m_cnt;
    QMutex m_mutex;

    void run();
    void process(const void *buf, int len);
};

#endif // CAMERATHREAD_H
