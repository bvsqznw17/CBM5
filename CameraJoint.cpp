#include "CameraJoint.h"
#include <QElapsedTimer>
#include <QDebug>

#ifdef __ARM_EABI__
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>

using namespace cv;
using namespace std;

static void jointMat(Mat &mat, const Mat &mat1, const Mat &mat2);
static void cvtPts(const QVector<float> &pts, Point2f *p);
static QVector<float> cvtPts(const QStringList &list);
static QStringList cvtPts(const QVector<float> &list);
#endif

CameraJoint::CameraJoint() : QObject(0)
{
    cnt = 0;
    etimer.start();
    elapsed = 0;
    isProcess = false;
    needCalib = true;
    imgw = 1024; imgh = 768;

    moveToThread(&thd);
    thd.start();
}

CameraJoint::~CameraJoint()
{
    thd.quit();
    thd.wait();
}

void CameraJoint::open()
{
    QMutexLocker locker(&mutex);

    cnt++;
    if (cnt != 1)
        return;

    img1 = QImage();
    img2 = QImage();
    elapsed = 0;

    if (cam1 && cam2) {
        connect(cam1, SIGNAL(imageUpdate(QImage)),
                this, SLOT(updateImage1(QImage)),
                Qt::DirectConnection);
        connect(cam2, SIGNAL(imageUpdate(QImage)),
                this, SLOT(updateImage2(QImage)),
                Qt::DirectConnection);
        cam1->open();
        cam2->open();
    }
}

void CameraJoint::close()
{
    QMutexLocker locker(&mutex);

    cnt--;
    if (cnt != 0)
        return;

    img1 = QImage();
    img2 = QImage();
    elapsed = 0;

    if (cam1 && cam2) {
        disconnect(cam1, SIGNAL(imageUpdate(QImage)),
                   this, SLOT(updateImage1(QImage)));
        disconnect(cam2, SIGNAL(imageUpdate(QImage)),
                   this, SLOT(updateImage2(QImage)));
        cam1->close();
        cam2->close();
    }
}

void CameraJoint::setImageSize(int w, int h)
{
    QMutexLocker locker(&mutexc);

    imgw = w; imgh = h;
    needCalib = true;
}

void CameraJoint::setPoints(const QVector<float> &pts, QVector<float> &p)
{
    QMutexLocker locker(&mutexc);

    p = pts;
    needCalib = true;
}

void CameraJoint::setPoints(const QVector<float> &p1, const QVector<float> &p2, const QVector<float> &p1c, const QVector<float> &p2c)
{
    QMutexLocker locker(&mutexc);
    pts1 = p1;
    pts2 = p2;
    pts1c = p1c;
    pts2c = p2c;
    needCalib = true;
}

void CameraJoint::updateImage1(const QImage &img)
{
    QMutexLocker locker(&mutex);

    qint64 ee = etimer.restart();
    if (!img2.isNull()) {
        img1 = img;
        process();
    }
    else if (ee >= elapsed * 0.4)
        img1 = img;
    else
        img1 = QImage();
    elapsed = ee;
}

void CameraJoint::updateImage2(const QImage &img)
{
    QMutexLocker locker(&mutex);

    qint64 ee = etimer.restart();
    if (!img1.isNull()) {
        img2 = img;
        process();
    }
    else if (ee >= elapsed * 0.4)
        img2 = img;
    else
        img2 = QImage();
    elapsed = ee;
}

void CameraJoint::process2(QImage img1, QImage img2)
{
    Q_UNUSED(img1); Q_UNUSED(img2);
#ifdef __ARM_EABI__
    if (needCalib)
        calib();

    if (h1.empty() || h2.empty()) {
        isProcess = false;
        return;
    }

    Mat t1(img1.height(), img1.width(), CV_8UC3, (uchar*)img1.constBits());
    Mat t2(img2.height(), img2.width(), CV_8UC3, (uchar*)img2.constBits());

    Mat tt1, tt2;
    CameraJoint_Thread thd1, thd2;
    thd1.src = &t1; thd1.dst = &tt1, thd1.h = &h1; thd1.s = &s1;
    thd2.src = &t2; thd2.dst = &tt2, thd2.h = &h2; thd2.s = &s2;
    thd1.start();
    thd2.start();
    thd1.wait();
    thd2.wait();

    QByteArray data(s.height * s.width * 3, Qt::Uninitialized);
    Mat ttt(s.height, s.width, CV_8UC3, data.data());
    jointMat(ttt, tt2, tt1);
    emit imageUpdate(data, QSize(s.width, s.height));
    isProcess = false;
#endif
}

void CameraJoint::process()
{
    if (!isProcess) {
        isProcess = true;
        QMetaObject::invokeMethod(this, "process2",
                                  Q_ARG(QImage, img1),
                                  Q_ARG(QImage, img2));
    }

    img1 = QImage();
    img2 = QImage();
}

void CameraJoint::calib()
{
#ifdef __ARM_EABI__
    QMutexLocker locker(&mutexc);
    needCalib = false;
    h1 = Mat();
    h2 = Mat();

    int w, h;
    Point2f p1[4], p2[4], p1c[4], p2c[4];

    if (pts1.size() != 8 || pts2.size() != 8 ||
            pts1c.size() != 8 || pts2c.size() != 8)
        return;

    w = imgw; h = imgh;
    cvtPts(pts1, p1);
    cvtPts(pts2, p2);
    cvtPts(pts1c, p1c);
    cvtPts(pts2c, p2c);

    int www = 1024;
    int hhh = 768;
    int sss = 900;
    QSize size(sss, sss);
    size.scale(w, h, Qt::KeepAspectRatio);
    s.width = size.width(); s.height = size.height();
    s1.width = s.width; s1.height = s.height * 0.51 + 1;
    s2.width = s.width; s2.height = s1.height;

    float scale = (float)s.width / sss;
    float sx = (sss-www)/2.0 * scale;
    float sy2 = (sss-hhh)/2.0 * scale;
    float sy1 = sy2 + s1.height - s.height;

    for (int i = 0; i < 4; i++) {
        p1c[i].x = p1c[i].x * scale + sx;
        p1c[i].y = p1c[i].y * scale + sy1;
        p2c[i].x = p2c[i].x * scale + sx;
        p2c[i].y = p2c[i].y * scale + sy2;
    }

    h1 = getPerspectiveTransform(p1, p1c);
    h2 = getPerspectiveTransform(p2, p2c);
#endif
}

CameraJoint_Thread::CameraJoint_Thread() : QThread(0)
{
}

void CameraJoint_Thread::run()
{
#ifdef __ARM_EABI__
    warpPerspective(*src, *dst, *h, *s);
#endif
}

CameraCalib::CameraCalib() : QObject(0)
{
    qRegisterMetaType<QVector<float> >("QVector<float>");
    cnt = 0;
    thresh = 128;
    isProcess = false;

    moveToThread(&thd);
    thd.start();
}

CameraCalib::~CameraCalib()
{
    thd.quit();
    thd.wait();
}

void CameraCalib::open()
{
    QMutexLocker locker(&mutex);

    cnt++;
    if (cnt != 1)
        return;

    if (cam) {
        connect(cam, SIGNAL(imageUpdate(QImage)),
                this, SLOT(updateImage(QImage)),
                Qt::DirectConnection);
        cam->open();
    }
}

void CameraCalib::close()
{
    QMutexLocker locker(&mutex);

    cnt--;
    if (cnt != 0)
        return;

    if (cam) {
        disconnect(cam, SIGNAL(imageUpdate(QImage)),
                   this, SLOT(updateImage(QImage)));
        cam->close();
    }
}

#include <QSettings>
void CameraCalib::loadPoints(QVector<float> &pts1, QVector<float> &pts2,
                             QVector<float> &pts1c, QVector<float> &pts2c)
{
    Q_UNUSED(pts1); Q_UNUSED(pts2); Q_UNUSED(pts1c); Q_UNUSED(pts2c);
#ifdef __ARM_EABI__
    QSettings sc("camera.ini", QSettings::IniFormat);
    QSettings si("/MainProgram_FenLi/camera.ini", QSettings::IniFormat);
    pts1 = cvtPts(si.value("Transform/img1").toStringList());
    pts2 = cvtPts(si.value("Transform/img2").toStringList());
    pts1c = cvtPts(sc.value("Transform/calib1_14").toStringList());
    pts2c = cvtPts(sc.value("Transform/calib2_14").toStringList());
#endif
}

void CameraCalib::savePoints(const QVector<float> &pts1, const QVector<float> &pts2)
{
    Q_UNUSED(pts1); Q_UNUSED(pts2);
#ifdef __ARM_EABI__
    QSettings si("/MainProgram_FenLi/camera.ini", QSettings::IniFormat);
    si.setValue("Transform/img1", cvtPts(pts1));
    si.setValue("Transform/img2", cvtPts(pts2));
    si.sync();
    system("sync");
#endif
}

QVector<float> CameraCalib::getPoints()
{
    QMutexLocker locker(&mutex);
    return pts;
}

void CameraCalib::updateImage(const QImage &img)
{
    if (isProcess)
        return;

    QMutexLocker locker(&mutex);
    if (isProcess)
        return;
    isProcess = true;
    QMetaObject::invokeMethod(this, "process", Q_ARG(QImage, img));
}

void CameraCalib::process(const QImage &img)
{
    Q_UNUSED(img);
#ifdef __ARM_EABI__
    QVector<float> pts;
    QImage imggray(img.size(), QImage::Format_Grayscale8);
    Mat mat(img.height(), img.width(), CV_8UC3, (uchar*)img.constBits());
    Mat matgray(imggray.height(), imggray.width(), CV_8UC1, (uchar*)imggray.constBits());
    cvtColor(mat, matgray, CV_RGB2GRAY);
    threshold(matgray, matgray, thresh, 255, THRESH_BINARY_INV);

    Mat element = getStructuringElement (MORPH_ELLIPSE, Size(3, 3));
    erode (matgray, matgray, element);
    Mat element1 = getStructuringElement (MORPH_ELLIPSE, Size(3, 3));
    dilate (matgray, matgray, element1);
    std::vector< std::vector<Point> > contours;
    std::vector<Vec4i> hierarchy;
    findContours(matgray, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    int n=0;
    if(contours.size () > 0)
    {
        //创建包围轮廓的矩形边界
        std::vector<Point2f> points1,points2;
        for (uint i = 0; i<contours.size(); i++)
        {
            Point2f center,center1;
            float m_r,m_r1;
            double area = contourArea (contours[i]);
            std::vector<Point> points = contours[i];

            Moments mc=moments(contours[i]);
            center1.x = mc.m10/mc.m00;
            center1.y = mc.m01/mc.m00;
            if(area < 100 || area > 400)
            {
                continue;
            }
            m_r = sqrt(area/M_PI);
            minEnclosingCircle(Mat(points),center,m_r1);


            if(area < M_PI*m_r*m_r/5)
            {
                continue;
            }
            if(m_r1 - m_r < 4)
            {
                points1.push_back(center1);
            }

        }
        if(points1.size() > 0 )
        {
            for(uint i=0;i<points1.size();i++)
            {
                Point2f p0 = points1.at(i);
                int cnt = 0;
                for(uint j=0;j<points1.size();j++)
                {
                    if(i == j)
                    {
                        continue;
                    }
                    Point2f p = points1.at(j);
                    if((p.x-p0.x)*(p.x-p0.x) + (p.y-p0.y)*(p.y-p0.y) < 2000)
                    {
                        cnt = cnt + 1 ;
                    }
                }
                if(cnt == 2)
                {
                    n = n + 1;
                    points2.push_back(p0);
                }
            }
        }
        if(n == 4)
        {
            pts.resize(8);
            float *p = pts.data();
            for(uint i=0;i<points2.size();i++)
            {
                Point2f p0 = points2.at(i);
                int j=i+i-2;
                for (; j>=0 && p0.x < p[j]; j-=2) {
                    p[j+2] = p[j];
                    p[j+3] = p[j+1];
                }
                j+=2;
                p[j] = p0.x;
                p[j+1] = p0.y;
            }
        }
    }
    mutex.lock();
    this->pts = pts;
    mutex.unlock();
    emit imageUpdate(img, imggray, pts);
    isProcess = false;
#endif
}



#ifdef __ARM_EABI__
static void jointMat(Mat &mat, const Mat &mat1, const Mat &mat2)
{
    int y = mat.rows - mat2.rows;
    if (y < 0) y = 0;
    memcpy(mat.data, mat1.data, y*mat.cols*3);

    uchar* mm = 0;
    const uchar* mm1 = 0;
    const uchar* mm2 = 0;

    double n = mat1.rows - y;
    int i = y, i2 = 0;
    for (; i < mat.rows && i < mat1.rows && i2 < mat2.rows; i++, i2++) {
        uchar* m = mat.ptr<uchar>(i);
        const uchar* m1 = mat1.ptr<uchar>(i);
        const uchar* m2 = mat2.ptr<uchar>(i2);
        double a = i2 / n;
        for (int j = 0; j < mat.cols; j++) {
            mm = m + j * 3;
            mm1 = m1 + j * 3;
            mm2 = m2 + j * 3;
            /*
            if (mm2[0] == 0 && mm2[1] == 0 && mm2[2] == 0) {
                mm[0] = mm1[0];
                mm[1] = mm1[1];
                mm[2] = mm1[2];
            }
            else if (mm1[0] == 0 && mm1[1] == 0 && mm1[2] == 0) {
                mm[0] = mm2[0];
                mm[1] = mm2[1];
                mm[2] = mm2[2];
            }
            else {
                mm[0] = mm1[0]*(1-a) + mm2[0]*a;
                mm[1] = mm1[1]*(1-a) + mm2[1]*a;
                mm[2] = mm1[2]*(1-a) + mm2[2]*a;
            }
            */
            mm[0] = mm1[0]*(1-a) + mm2[0]*a;
            mm[1] = mm1[1]*(1-a) + mm2[1]*a;
            mm[2] = mm1[2]*(1-a) + mm2[2]*a;
        }
    }

    if (i >= mat.rows)
        return;
    mm += 3; mm1 += 3; mm2 += 3;
    int ilen = 0;
    if (i2 < mat2.rows) {
        ilen = min(mat.rows-i, mat2.rows-i2);
        memcpy(mm, mm2, ilen*mat.cols*3);
    }
    else if (i < mat1.rows) {
        ilen = min(mat.rows-i, mat1.rows-i);
        memcpy(mm, mm1, ilen*mat.cols*3);
    }

    i += ilen;
    mm += ilen*mat.cols*3;
    if (i < mat.rows)
        memset(mm, 0, (mat.rows-i)*mat.cols*3);
}

static void cvtPts(const QVector<float> &pts, Point2f *p)
{
    p[0].x = pts[0]; p[0].y = pts[1];
    p[1].x = pts[2]; p[1].y = pts[3];
    p[2].x = pts[4]; p[2].y = pts[5];
    p[3].x = pts[6]; p[3].y = pts[7];
}

static QVector<float> cvtPts(const QStringList &list)
{
    if (list.size() < 8)
        return QVector<float>();

    QVector<float> r(8);
    float *p = r.data();
    for (int i = 0; i < 8; i++)
        p[i] = list.at(i).toFloat();

    return r;
}

static QStringList cvtPts(const QVector<float> &list)
{
    if (list.size() < 8)
        return QStringList();

    QStringList r;
    r.reserve(8);
    for (int i = 0; i < 8; i++)
        r.append(QString::number(list.at(i)));

    return r;
}
#endif
