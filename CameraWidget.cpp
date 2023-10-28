#include "CameraWidget.h"
#include <QPainter>

CameraWidget::CameraWidget(QWidget *parent) : QWidget(parent)
{
    cam = 0;
    isOpen = false;
}

CameraWidget::~CameraWidget()
{
    if (isOpen) {
        isOpen = false;
        cam->close();
    }
}

void CameraWidget::updateImage(const QImage &img)
{
    image = img;
    update();
}

void CameraWidget::showEvent(QShowEvent *)
{
    image = QImage();

    if (cam) {
        connect(cam, SIGNAL(imageUpdate(QImage)),
                this, SLOT(updateImage(QImage)),
                Qt::UniqueConnection);
        if (!isOpen) {
            isOpen = true;
            cam->open();
        }
    }
}

void CameraWidget::hideEvent(QHideEvent *)
{
    if (cam) {
        disconnect(cam, SIGNAL(imageUpdate(QImage)),
                   this, SLOT(updateImage(QImage)));
        if (isOpen) {
            isOpen = false;
            cam->close();
        }
    }
}

void CameraWidget::paintEvent(QPaintEvent *)
{
    QSize size = image.size();
    QSize tsize = this->size();
    size.scale(tsize, Qt::KeepAspectRatio);
    int x = (tsize.width() - size.width()) / 2;
    int y = (tsize.height() - size.height()) / 2;

    QPainter p(this);
    p.fillRect(0, 0, tsize.width(), tsize.height(), Qt::black);
    p.drawImage(QRect(x, y, size.width(), size.height()), image);
}

CameraWidgetJoint::CameraWidgetJoint(QWidget *parent) : QWidget(parent)
{
    m_size = QSize(0, 0);
    isOpen = false;
}

CameraWidgetJoint::~CameraWidgetJoint()
{
    if (isOpen) {
        isOpen = false;
        cam.close();
    }
}

void CameraWidgetJoint::updateImage(QByteArray data, QSize size)
{
    m_data = data;
    m_size = size;
    update();
}

void CameraWidgetJoint::resizeEvent(QResizeEvent *)
{
    QSize s = this->size();
    cam.setImageSize(s.width(), s.height());
}

void CameraWidgetJoint::showEvent(QShowEvent *)
{
    m_data.clear();
    m_size = QSize(0, 0);

    connect(&cam, SIGNAL(imageUpdate(QByteArray,QSize)),
            this, SLOT(updateImage(QByteArray,QSize)),
            Qt::UniqueConnection);
    if (!isOpen) {
        isOpen = true;
        cam.open();
    }
}

void CameraWidgetJoint::hideEvent(QHideEvent *)
{
    disconnect(&cam, SIGNAL(imageUpdate(QByteArray,QSize)),
               this, SLOT(updateImage(QByteArray,QSize)));
    if (isOpen) {
        isOpen = false;
        cam.close();
    }
}

void CameraWidgetJoint::paintEvent(QPaintEvent *)
{
    QImage image((uchar*)m_data.constData(), m_size.width(), m_size.height(),
                 m_size.width()*3, QImage::Format_RGB888);
    QSize size = m_size;
    QSize tsize = this->size();
    int x = (tsize.width() - size.width()) / 2;
    int y = (tsize.height() - size.height()) / 2;

    QPainter p(this);
    p.fillRect(0, 0, tsize.width(), tsize.height(), Qt::black);
    p.drawImage(x, y, image);
}

CameraWidgetCalib::CameraWidgetCalib(QWidget *parent) : QWidget(parent)
{
    cam = 0;
    isOpen = false;
    isErZhiHua = false;
}

CameraWidgetCalib::~CameraWidgetCalib()
{
    if (isOpen) {
        isOpen = false;
        cam->close();
    }
}

void CameraWidgetCalib::updateImage(QImage img, QImage img2, QVector<float> pts)
{
    image = img;
    image2 = img2;
    points = pts;
    update();
}

void CameraWidgetCalib::showEvent(QShowEvent *)
{
    image = QImage();
    image2 = QImage();
    points.clear();

    if (cam) {
        connect(cam, SIGNAL(imageUpdate(QImage,QImage,QVector<float>)),
                this, SLOT(updateImage(QImage,QImage,QVector<float>)),
                Qt::UniqueConnection);
        if (!isOpen) {
            isOpen = true;
            cam->open();
        }
    }
}

void CameraWidgetCalib::hideEvent(QHideEvent *)
{
    if (cam) {
        disconnect(cam, SIGNAL(imageUpdate(QImage,QImage,QVector<float>)),
                   this, SLOT(updateImage(QImage,QImage,QVector<float>)));
        if (isOpen) {
            isOpen = false;
            cam->close();
        }
    }
}

void CameraWidgetCalib::paintEvent(QPaintEvent *)
{
    QImage &image = isErZhiHua ? this->image2 : this->image;
    QSize size = image.size();
    QSize tsize = this->size();
    float scale = size.width();
    size.scale(tsize, Qt::KeepAspectRatio);
    scale = size.width() / scale;
    int x = (tsize.width() - size.width()) / 2;
    int y = (tsize.height() - size.height()) / 2;

    QPainter p(this);
    p.fillRect(0, 0, tsize.width(), tsize.height(), Qt::black);
    p.drawImage(QRect(x, y, size.width(), size.height()), image);

    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::red);

    for (int i = 0; i+1 < points.size(); i += 2) {
        float xx = points.at(i) * scale + x;
        float yy = points.at(i+1) * scale + y;
        p.drawLine(QPointF(xx-10, yy), QPointF(xx+10, yy));
        p.drawLine(QPointF(xx, yy-10), QPointF(xx, yy+10));
    }
}
