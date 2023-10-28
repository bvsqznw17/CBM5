#include "GraphZhenDong.h"
#include <QPainter>

GraphZhenDong::GraphZhenDong(QWidget *parent) : QWidget(parent),
    rb1(500),
    rb2(500)
{
    m_max = 0;
}

void GraphZhenDong::add(int w1, int w2, int upn)
{
    if (rb1.size() % upn == 0)
        update();
    rb1.append(w1);
    rb2.append(w2);
    int w3 = qAbs(w1 - w2);
    w1 = qAbs(w1);
    w2 = qAbs(w2);
    if (m_max < w1)
        m_max = w1;
    if (m_max < w2)
        m_max = w2;
    if (m_max < w3)
        m_max = w3;
    /*
    for (int i = 0; i < rb1.size(); i++) {
        if (m_max < rb1[i])
            m_max = rb1[i];
        if (m_max < rb2[i])
            m_max = rb2[i];
    }*/
}

void GraphZhenDong::clear()
{
    m_max = 0;
    rb1.clear();
    rb2.clear();
    update();
}

// 边距
#define X0 1
#define Y0 31
#define X1 1
#define Y1 31

void GraphZhenDong::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    int w = width();
    int h = height();
    int yo = (h-Y1-Y0)/2+Y0;
    int hh = h-Y1-yo;
    int ya = yo-hh;
    int yb = yo+hh;

    QPainter pt(this);
    pt.setPen(Qt::white);
    pt.drawLine(X0-1, yo, w-X1-1, yo);
    pt.drawLine(X0-1, ya, X0-1, yb);

    int size = rb1.size();
    if (size == 0)
        return;

    qreal dx = (qreal)(w - X0 - X1 - 1) / (rb1.ssize() - 1);
    qreal dy = (qreal)(hh) / m_max;
    if (m_max == 0)
        dy = 0;

    QPoint p1[size], p2[size], p3[size];
    for (int i = 0; i < size; i++) {
        int x = X0 + qRound(i*dx);
        p1[i].setX(x);
        p1[i].setY(yo - qRound(rb1[i]*dy));
        p2[i].setX(x);
        p2[i].setY(yo - qRound(rb2[i]*dy));
        p3[i].setX(x);
        p3[i].setY(yo - qRound((rb1[i]-rb2[i])*dy));
    }
    pt.setPen(Qt::green);
    pt.drawPolyline(p1, size);
    pt.setPen(Qt::blue);
    pt.drawPolyline(p2, size);
    pt.setPen(Qt::red);
    pt.drawPolyline(p3, size);
}
