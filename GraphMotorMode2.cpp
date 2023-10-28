#include "GraphMotorMode2.h"
#include "GraphDraw.h"
#include <QPainter>
#include <QMouseEvent>

#define m_datai_sp 0 // 速度
#define m_datai_ta 1 // 加速时间
#define m_datai_tm 2 // 匀速时间
#define m_datai_tb 3 // 减速时间
#define m_datai_ts 4 // 延时
#define m_datan 5

GraphMotorMode2::GraphMotorMode2(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);

    m_data = 0;
    m_size = 0;
    m_smax = 0;
    m_tmsum = 0;
    m_pressedi = -1;
#ifdef MYOPT_TEST
    int d[] = {
        60, 150, 100, 220, 0,
        20, 200, 150, 220, 20,
        -50, 160, 60, 200, 0,
        40, 200, 80, 250, 0,
    };
    setData(d, sizeof(d)/4);
#endif
}

GraphMotorMode2::~GraphMotorMode2()
{
    if (m_data)
        free(m_data);
}

void GraphMotorMode2::setData(const int *data, int size)
{
    if (m_data)
        free(m_data);

    size -= size % m_datan;
    m_data = (int *)malloc(size * sizeof(int));
    memcpy(m_data, data, size * sizeof(int));
    m_size = size;
    m_smax = 0;
    m_tmsum = 0;

    for (int i = 0; i < m_size; i+=m_datan) {
        int sp = qAbs(m_data[i+m_datai_sp]);
        if (m_smax < sp)
            m_smax = sp;
        m_tmsum += m_data[i+m_datai_ta];
        m_tmsum += m_data[i+m_datai_tm];
        m_tmsum += m_data[i+m_datai_tb];
        m_tmsum += m_data[i+m_datai_ts];
    }
    update();
}

// 边距
#define X0 10
#define Y0 10
#define X1 10
#define Y1 10

void GraphMotorMode2::paintEvent(QPaintEvent *event)
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
    pt.setPen(Qt::green);

    qreal dx = (qreal)(w - X0 - X1 - 1) / m_tmsum;
    qreal dy = (qreal)(hh) / m_smax;
    if (m_smax == 0)
        dy = 0;

    QFont font = this->font();
    font.setPixelSize(18);
    pt.setFont(font);

    int tms = 0;
    for (int i = 0; i < m_size; i+=m_datan) {
        int tm0 = tms;

        if (i == m_pressedi) {
            tms += m_data[i+m_datai_ta];
            tms += m_data[i+m_datai_tm];
            tms += m_data[i+m_datai_tb];
            tms += m_data[i+m_datai_ts];
            int x1 = qRound(tm0 * dx);
            int x2 = qRound(tms * dx);
            pt.setPen(Qt::NoPen);
            pt.setBrush(Qt::gray);
            pt.drawRect(x1+X0, ya, x2-x1+1, hh);
            pt.drawRect(x1+X0, yo+1, x2-x1+1, hh);
            pt.setPen(Qt::green);
            tms = tm0;
        }

        QPoint p[5];
        p[0].setX(X0 + qRound(dx * tms));
        p[0].setY(yo);
        p[1].setX(X0 + qRound(dx * (tms+=m_data[i+m_datai_ta])));
        p[1].setY(yo - qRound(dy * (m_data[i+m_datai_sp])));
        p[2].setX(X0 + qRound(dx * (tms+=m_data[i+m_datai_tm])));
        p[2].setY(p[1].y());
        p[3].setX(X0 + qRound(dx * (tms+=m_data[i+m_datai_tb])));
        p[3].setY(yo);
        p[4].setX(X0 + qRound(dx * (tms+=m_data[i+m_datai_ts])));
        p[4].setY(yo);
        pt.drawPolyline(p, 5);

        qreal textpx = X0 + (tms+tm0)*dx/2;
        pt.setPen(Qt::white);
        QRectF num_rect(textpx - 20, yo + 1, 40, 20);
        drawText(pt, num_rect, w, QString::number(i/m_datan + 1));
        pt.setPen(Qt::green);
    }
}

void GraphMotorMode2::mousePressEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();
    int w = width();
    int h = height();
    if (!(x >= X0 && x < w - X1 && y >= Y0 && y < h - Y1))
        return;

    qreal dx = (qreal)(w - X0 - X1) / m_tmsum;
    x -= X0;

    int tms = 0;
    for (int i = 0; i < m_size; i+=m_datan) {
        tms += m_data[i+m_datai_ta];
        tms += m_data[i+m_datai_tm];
        tms += m_data[i+m_datai_tb];
        tms += m_data[i+m_datai_ts];
        int tmx = qRound(tms * dx);
        if (x < tmx) {
            m_pressedi = i;
            break;
        }
    }
    update();
}

void GraphMotorMode2::mouseReleaseEvent(QMouseEvent *)
{
    if (m_pressedi == -1)
        return;
    clicked(m_pressedi);
    m_pressedi = -1;
    update();
}
