#include "GraphWeightDeviation.h"
#include <QPainter>

GraphWeightDeviation::GraphWeightDeviation(QWidget *parent) :
    QWidget(parent),
    rb(100)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setWeight(0, 0, 0, 0);

    setWeight(100, 15, 10, 0);
    for (int i = 0; i < rb.ssize(); i++)
        add(qrand() % 7 - 3 + w0);
    for (int i = 2; i < rb.ssize(); i+=2)
        rb[i] = (qrand() % 21 - 10 + w0);
    for (int i = 5; i < rb.ssize(); i+=5)
        rb[i] = (qrand() % 51 - 25 + w0);
    for (int i = 10; i < rb.ssize(); i+=10)
        rb[i] = (qrand() % 101 - 40 + w0);
    rb[10] = 15 + w0;
    rb[12] = 16 + w0;
    rb[14] = 14 + w0;
    rb[16] = 50 + w0;
    rb[18] = 13 + w0;
}

void GraphWeightDeviation::clear()
{
    rb.clear();
    setWeight(0, 0, 0, 0);
    update();
}

void GraphWeightDeviation::add(int w)
{
    rb.append(w);
    update();
}

void GraphWeightDeviation::add(int *w, int n)
{
    rb.append(w, n);
    update();
}

void GraphWeightDeviation::setWeight(int _w0, int _w1, int _w2, int _p)
{
    if (_w0 == w0 && _w1 == w1 && _w2 == w2 && _p == p)
        return;
    w0 = _w0;
    w1 = _w1;
    w2 = _w2;
    p = _p;
    updateText();
    update();
}

void GraphWeightDeviation::updateText()
{
    ytext[0] = getDotStr(w0, p);
    ytext[1] = getDotStr(w0+w1*2, p);
    ytext[2] = getDotStr(w0+w1, p);
    ytext[3] = getDotStr(w0-w2, p);
    ytext[4] = getDotStr(w0-w2*2, p);
}

// 点的大小
#define Ptsz 9
// 刻度线的长度
#define Markw 3
// 边距
#define X0 (1+3+Markw+Markw)
#define Y0 0
#define X1 0
#define Y1 0

void GraphWeightDeviation::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QFont font = this->font();
    font.setPixelSize(18);
    QPainter pt(this);
    pt.setFont(font);

    QRect ytr = pt.boundingRect(rect(), ytext[1]).toAlignedRect();
    int x0 = X0 + ytr.width();
    int x1 = X1;
    int y0 = Y0 + ytr.height() / 2;
    int y1 = Y1 + ytr.height() / 2;

    int w = width();
    int h = height();

    qreal dx = (qreal)(w - x0 - x1 - Ptsz - 1) / (rb.ssize() - 1);
    qreal dy = (qreal)(h - y1 - y0) / (w1 + w2) / 2;

    int yty[5];
    yty[0] = qRound(y0+(dy*w1*2));
    yty[1] = y0;
    yty[2] = qRound(y0+(dy*w1));
    yty[3] = qRound(h-y1-(dy*w2));
    yty[4] = h-y1;
    if (w1 + w2 == 0) {
        int yh = (h-y1-y0) / 2;
        yty[0] = y0 + yh;
        yty[2] = y0 + yh/2;
        yty[3] = h-y1 - yh/2;
    }
    int yo = yty[0];

    pt.setPen(Qt::white);
    pt.drawLine(x0-1, yo, w-x1-1, yo);
    pt.drawLine(x0-1, h-y1, x0-1, y0);
    for (int i = 0; i < 5; i++) {
        const int y = yty[i];
        const int h = ytr.height();
        pt.drawLine(x0-1, y, x0-1-Markw, y);
        QRect r(0, y-h/2, x0-1-Markw-Markw, h);
        pt.drawText(r, Qt::AlignRight|Qt::AlignHCenter, ytext[i]);
    }

    qreal ytysum = yty[3] - yty[2];
    QLinearGradient linearg(0, yty[2], 0, yty[3]);
    linearg.setColorAt(0, Qt::red);
    linearg.setColorAt((yty[0]-yty[2])/2.0/ytysum, Qt::yellow);
    linearg.setColorAt((yty[0]-yty[2])/ytysum, Qt::green);
    linearg.setColorAt(((yty[3]-yty[0])/2.0+yty[0]-yty[2])/ytysum, Qt::yellow);
    linearg.setColorAt(1, Qt::red);
    QPen pen(QBrush(linearg), Ptsz);
    pen.setCapStyle(Qt::RoundCap);
    QPen pen0(Qt::black, Ptsz);
    pen0.setCapStyle(Qt::RoundCap);
    pt.setRenderHint(QPainter::Antialiasing);

    QPointF pf[rb.size()];
    qreal x = Ptsz / 2.0;
    for (int i = 0; i < rb.size(); i++) {
        int w = rb[i] - w0;
        if (w <= w1 && w >= -w2)
            pt.setPen(pen);
        else
            pt.setPen(pen0);
        pf[i].setX(x + x0);
        pf[i].setY(yo - (w != 0 ? w * dy : 0));
        if (pf[i].y() < 0)
            pf[i].setY(0);
        if (pf[i].y() > h)
            pf[i].setY(h);
        x += dx;
        pt.drawPoint(pf[i]);
    }
}
