#ifndef CAMERAJOINT_H
#define CAMERAJOINT_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <QMutex>
#include <QElapsedTimer>
#ifdef __ARM_EABI__
#include <opencv2/core/core.hpp>
#endif

#include "CameraThread.h"

class CameraJoint : public QObject
{
    Q_OBJECT
public:
    explicit CameraJoint();
    ~CameraJoint();

    CameraThread *cam1, *cam2;

    void open();
    void close();

    void setImageSize(int w, int h);
    void setPoints(const QVector<float> &pts, QVector<float> &p);
    void setPoints(const QVector<float> &p1, const QVector<float> &p2,
                   const QVector<float> &p1c, const QVector<float> &p2c);
    inline void setImagePoints1(const QVector<float> &pts) { setPoints(pts, pts1); }
    inline void setImagePoints2(const QVector<float> &pts) { setPoints(pts, pts2); }
    inline void setCalibPoints1(const QVector<float> &pts) { setPoints(pts, pts1c); }
    inline void setCalibPoints2(const QVector<float> &pts) { setPoints(pts, pts2c); }

signals:
    void imageUpdate(QByteArray data, QSize size);

public slots:
    void updateImage1(const QImage &img);
    void updateImage2(const QImage &img);
    void process2(QImage img1, QImage img2);

private:
    void process();
    void calib();

private:
    int cnt;
    QThread thd;
    QMutex mutex, mutexc;
    QImage img1, img2;
    QElapsedTimer etimer;
    qint64 elapsed;
    bool isProcess;
    bool needCalib;
    int imgw, imgh;
    QVector<float> pts1, pts2;
    QVector<float> pts1c, pts2c;
#ifdef __ARM_EABI__
    cv::Size s, s1, s2;
    cv::Mat h1, h2;
#endif

};

class CameraJoint_Thread : public QThread
{
    Q_OBJECT
public:
    explicit CameraJoint_Thread();

#ifdef __ARM_EABI__
    cv::Mat *src, *dst, *h;
    cv::Size *s;
#endif

private:
    void run();
};

class CameraCalib : public QObject
{
    Q_OBJECT
public:
    explicit CameraCalib();
    ~CameraCalib();

    CameraThread *cam;

    void open();
    void close();

    static void loadPoints(QVector<float> &pts1, QVector<float> &pts2,
                           QVector<float> &pts1c, QVector<float> &pts2c);
    static void savePoints(const QVector<float> &pts1, const QVector<float> &pts2);
    QVector<float> getPoints();
    inline void setThresh(int t) { thresh = t; }

signals:
    void imageUpdate(QImage img, QImage img2, QVector<float> pts);

public slots:
    void updateImage(const QImage &img);
    void process(const QImage &img);

private:
    int cnt;
    QThread thd;
    QMutex mutex;
    QVector<float> pts;
    int thresh;
    bool isProcess;
};

#endif // CAMERAJOINT_H
