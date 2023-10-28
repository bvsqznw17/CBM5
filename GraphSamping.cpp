#include "GraphSamping.h"
#include "GraphDraw.h"
#include <QPainter>

GraphSamping::GraphSamping(QWidget *parent) : QWidget(parent)
{
    m_data = 0;
    m_size = 0;
    m_max = 0;
    m_min = 0;

    m_line1 = 0;
    m_line2 = 0;
    m_line1ms = 0;
}

GraphSamping::~GraphSamping()
{
    if (m_data)
        free(m_data);
}

void GraphSamping::setData(const qint16 *data, int size)
{
    if (m_data)
        free(m_data);

    m_data = (qint16 *)malloc(size * sizeof(qint16));
    memcpy(m_data, data, size * sizeof(qint16));
    m_size = size;
    m_max = 0;
    m_min = 0;

    for (int i = 0; i < m_size; i++) {
        if (m_max < m_data[i])
            m_max = m_data[i];
        if (m_min > m_data[i])
            m_min = m_data[i];
    }
    update();
}

void GraphSamping::setLine(int l1, int l2, int l1ms)
{
    if (m_line1 == l1 && m_line2 == l2 && l1ms == m_line1ms)
        return;

    m_line1 = l1;
    m_line2 = l2;
    m_line1ms = l1ms;
    update();
}

// 边距
#define X0 1
#define Y0 31
#define X1 1
#define Y1 31

void GraphSamping::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    int w = width();
    int h = height();
    int yo = h - Y1 - 1;

    QPainter pt(this);
    pt.setPen(Qt::white);
    pt.drawLine(X0-1, yo, w-X1-1, yo);
    pt.drawLine(X0-1, yo, X0-1, Y0);
    pt.setPen(Qt::red);

    if (m_size == 0)
        return;
    qreal dx = (qreal)(w - X0 - X1 - 1) / (m_size - 1);
    qreal dy_max = (qreal)(h - Y1 - Y0 - 1) / m_max;
    qreal dy_min = (qreal)(Y1) / -m_min;
    qreal dy = qMin(dy_max, dy_min);
    if (qIsInf(dx) || qIsNaN(dx))
        dx = 0;
    if (qIsInf(dy) || qIsNaN(dy))
        dy = 0;

    QFont font = this->font();
    font.setPixelSize(18);
    pt.setFont(font);

    QPoint p[m_size];
    for (int i = 0; i < m_size; i++) {
        p[i].setX(X0 + qRound(i*dx));
        p[i].setY(yo - qRound(m_data[i]*dy));
    }
    pt.drawPolyline(p, m_size);

    int line1x = m_line1>=10 ? qRound((m_line1-10)*dx)/10+X0 : X0-1;
    QRectF rectl1(line1x-100, yo+1, 200, Y1);
    pt.setPen(Qt::green);
    pt.drawLine(line1x, Y0, line1x, yo);
    drawText(pt, rectl1, w, QString::number(m_line1ms).append("ms"));

    int line2x = m_line2>=10 ? qRound((m_line2-10)*dx)/10+X0 : X0-1;
    QRectF rectl2(line2x-100, 0, 200, Y0);
    pt.setPen(Qt::blue);
    pt.drawLine(line2x, Y0, line2x, yo);
    drawText(pt, rectl2, w, QString::number(m_line2).append("ms"));
}
