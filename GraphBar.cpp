#include "GraphBar.h"
#include <QPainter>

GraphBar::GraphBar(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);
    w0 = w1 = w2 = p = 0;
    dw = 1;

    for (int i = 0; i < 10000; i++) {
        data.append(qrand() % 200 - 100);
    }
    setRange(0, 50, 50, 1);
}

void GraphBar::clear()
{
    data.clear();
    w0 = w1 = w2 = p = 0;
    dw = 1;
    update();
}

void GraphBar::setData(QVector<int> _data)
{
    data = _data;
    update();
}

void GraphBar::setRange(int _w0, int _w1, int _w2, int _p)
{
    if (_w0 == w0 && _w1 == w1 && _w2 == w2 && _p == p)
        return;
    w0 = _w0;
    w1 = _w1;
    w2 = _w2;
    p = _p;
    update();
}

void GraphBar::setDistance(int d)
{
    if (d == dw || d <= 0)
        return;
    dw = d;
    update();
}

void GraphBar::adjustMinSize()
{
    int dw2 = (dw + 1) / 2 - 1;
    int wh1 = (w1 * 2 + dw2) / dw + 1;
    int wh2 = (w2 * 2 + dw2) / dw + 1;

    QFontMetrics fm(font());
    int fmh = fm.height();
    setMinimumHeight((wh1 + wh2 + 1) * fmh + 1);
}

double GraphBar::gety0bd()
{
    int dw2 = (dw + 1) / 2 - 1;
    int wh1 = (w1 * 2 + dw2) / dw + 1;
    int wh2 = (w2 * 2 + dw2) / dw + 1;

    return (wh1 + 0.5) / (wh1 + wh2 + 1);
}

void GraphBar::paintItem(QPainter &pt, int y, int fmw1, int fmw2, int fmh,
                         int ww, int wv, int wn, int wnmax)
{
    int x = 0;

    QString wvstr;
    if (wv == INT_MAX)
        wvstr = ">";
    else if (wv == INT_MIN)
        wvstr = "<";
    else
        wvstr = getDotStr(wv, p);
    pt.drawText(x, y, fmw1, fmh, Qt::AlignRight, wvstr);
    x += fmw1;

    int xw;
    if (wnmax != 0)
        xw = (qreal)wn / wnmax * ww;
    else
        xw = 0;
    pt.setBrush(Qt::darkGreen);
    pt.drawRect(x, y, xw, fmh);
    x += xw;

    xw = ww - xw;
    pt.setBrush(Qt::gray);
    pt.drawRect(x, y, xw, fmh);
    x += xw + 1;

    pt.drawText(x, y, fmw2, fmh, Qt::AlignLeft, QString::number(wn));
}

void GraphBar::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    int dw2 = (dw + 1) / 2 - 1;
    int wh1 = (w1 * 2 + dw2) / dw + 1;
    int wh2 = (w2 * 2 + dw2) / dw + 1;
    int wn1[wh1+1];
    int wn2[wh2+1];
    memset(wn1, 0, (wh1+1)*sizeof(int));
    memset(wn2, 0, (wh2+1)*sizeof(int));
    for (int i = 0; i < data.size(); i++) {
        int w = data.at(i);
        if (w >= 0) {
            int index = (w + dw2) / dw;
            if (index < wh1)
                wn1[index]++;
            else
                wn1[wh1]++;
        }
        else {
            int index = (-w + dw2) / dw;
            if (index < wh2)
                wn2[index]++;
            else
                wn2[wh2]++;
        }
    }
    int wnmax = wn1[0]+wn2[0];
    for (int i = 1; i <= wh1; i++)
        if (wnmax < wn1[i])
            wnmax = wn1[i];
    for (int i = 1; i <= wh2; i++)
        if (wnmax < wn2[i])
            wnmax = wn2[i];

    QFontMetrics fm(font());
    int fmh = fm.height();
    int fmw1 = fm.horizontalAdvance(getDotStr(qMax(w1,w2)*2*10, p));
    int fmw2 = fm.horizontalAdvance(QString::number(wnmax*10));
    int ww = width() - fmw1 - fmw2 - 1;
    int y = 0;

    QPainter pt(this);
    pt.setPen(Qt::white);

    paintItem(pt, y, fmw1, fmw2, fmh, ww, INT_MAX, wn1[wh1], wnmax);
    y += fmh;

    for (int i = wh1 - 1; i > 0; i--) {
        paintItem(pt, y, fmw1, fmw2, fmh, ww, i*dw, wn1[i], wnmax);
        y += fmh;
    }

    paintItem(pt, y, fmw1, fmw2, fmh, ww, 0, wn1[0]+wn2[0], wnmax);
    y += fmh;

    for (int i = 1; i < wh2; i++) {
        paintItem(pt, y, fmw1, fmw2, fmh, ww, -i*dw, wn2[i], wnmax);
        y += fmh;
    }

    paintItem(pt, y, fmw1, fmw2, fmh, ww, INT_MIN, wn2[wh2], wnmax);
    y += fmh;
}
