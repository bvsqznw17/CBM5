#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QWidget>
#include "CameraThread.h"

class CameraWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CameraWidget(QWidget *parent = 0);
    ~CameraWidget();

    CameraThread *cam;

public slots:
    void updateImage(const QImage &img);

private:
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);
    void paintEvent(QPaintEvent *);
    QImage image;
    bool isOpen;

};

#include "CameraJoint.h"

class CameraWidgetJoint : public QWidget
{
    Q_OBJECT
public:
    explicit CameraWidgetJoint(QWidget *parent = 0);
    ~CameraWidgetJoint();

    CameraJoint cam;

public slots:
    void updateImage(QByteArray data, QSize size);

private:
    void resizeEvent(QResizeEvent *);
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);
    void paintEvent(QPaintEvent *);
    QByteArray m_data;
    QSize m_size;
    bool isOpen;

};

class CameraWidgetCalib : public QWidget
{
    Q_OBJECT
public:
    explicit CameraWidgetCalib(QWidget *parent = 0);
    ~CameraWidgetCalib();

    CameraCalib *cam;

    inline void setErZhiHua(bool b) { isErZhiHua = b; }

public slots:
    void updateImage(QImage img, QImage img2, QVector<float> pts);

private:
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);
    void paintEvent(QPaintEvent *);
    QImage image, image2;
    QVector<float> points;
    bool isOpen;
    bool isErZhiHua;

};

#endif // CAMERAWIDGET_H
