#include "GraphMotorMode.h"
#include "GraphDraw.h"
#include <math.h>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

#define m_datai_ag 0 // 角度
#define m_datai_sm 1 // 最大速度
#define m_datai_s0 2 // 起始速度
#define m_datai_s1 3 // 停止速度
#define m_datai_sa 4 // 加速度
#define m_datai_sb 5 // 减速度
#define m_datan 6
#define m_posn 4

GraphMotorMode::GraphMotorMode(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);

    m_data = 0;
    m_pos = 0;
    m_size = 0;
    m_smax = 0;
    m_tmsum = 0;
    m_pressedi = -1;
#ifdef MYOPT_TEST
    int d[] = {
        1000, 60, 0, 0, 20, 20,
        -1000, 60, 0, 0, 20, 20,
        500, 0, 60, 60, 0, 0,
        1600, 60, 0, 0, 20, 20,
    };
    setData(d, sizeof(d)/4);
#endif
}

GraphMotorMode::~GraphMotorMode()
{
    if (m_data)
        free(m_data);
    if (m_pos)
        free(m_pos);
}

static int y1c2(qreal a, qreal b, qreal c, qreal *x1, qreal *x2)
{
    qreal delta = b*b - 4*a*c;
    if (delta > 0) {
        *x1 = (-b + sqrt(delta)) / (2*a);
        *x2 = (-b - sqrt(delta)) / (2*a);
        return 2;
    }
    else if (delta == 0) {
        *x1 = (-b) / (2*a);
        *x2 = *x1;
        return 1;
    }
    else
        return 0;
}

void GraphMotorMode::setData(const int *data, int size)
{
    if (m_data)
        free(m_data);
    if (m_pos)
        free(m_pos);

    size -= size % m_datan;
    m_data = (int *)malloc(size * sizeof(int));
    memcpy(m_data, data, size * sizeof(int));
    m_pos = (QPointF *)malloc(size / m_datan * m_posn * sizeof(QPointF));
    m_size = size;
    m_smax = 0;
    m_tmsum = 0;
    bool lastStop = false;
    for (int i = 0, pi = 0; i < m_size; i+=m_datan, pi+=m_posn) {
        int fz = m_data[m_datai_ag + i] > 0 ? 1 : -1;
        qreal ag = qAbs(m_data[m_datai_ag + i]);
        qreal s0 = qAbs(m_data[m_datai_s0 + i]);
        qreal sa = qAbs(m_data[m_datai_sa + i]);
        qreal sm = qAbs(m_data[m_datai_sm + i]);
        qreal sb = qAbs(m_data[m_datai_sb + i]);
        qreal s1 = qAbs(m_data[m_datai_s1 + i]);
        ag /= 3600;
        s0 /= 60;
        s1 /= 60;
        sm /= 60;

        if (sm == 0) {
            // 暂停时间
            m_pos[0 + pi].setX(m_tmsum);
            m_pos[0 + pi].setY(0);
            m_pos[1 + pi].setX(m_tmsum);
            m_pos[1 + pi].setY(0);
            m_tmsum += ag * 3600 / 1000;
            m_pos[2 + pi].setX(m_tmsum);
            m_pos[2 + pi].setY(0);
            m_pos[3 + pi].setX(m_tmsum);
            m_pos[3 + pi].setY(0);
            if (i != 0)
                m_pos[0 + pi].setY(m_pos[pi-1].y());
            lastStop = true;
            continue;
        }

        qreal tma = (sm - s0) / sa; // 起始到最大速度所需时间
        qreal tmb = (sm - s1) / sb; // 最大到结束速度所需时间
        if (sa == 0) tma = 0;
        if (sb == 0) tmb = 0;
        qreal aga = (sa * tma * tma) / 2 + s0 * tma; // 起始到最大速度所需角度
        qreal agb = (sb * tmb * tmb) / 2 + s1 * tmb; // 最大到结束速度所需角度
        qreal agc = (ag - (aga + agb)); // 剩余角度
        if (agc < 0) {
            // agc<0，说明最大速度过大，需要求出合适的最大速度
            // 先通过解一元二次方程求出多余的时间
            qreal tmma = sa, tmmb = sb; // 加减速时间比例：1/加减速度
            qreal a = -sa/2/tmma/tmma + -sb/2/tmmb/tmmb;
            qreal b = sm/tmma + sm/tmmb;
            qreal c = agc;
            qreal x1, x2;
            int r = y1c2(a, b, c, &x1, &x2);
            if (r) {
                // 两个解必定>0，最小解才是多余时间
                qreal mintm = qMin(x1, x2);
                // tma,tmb-=后出现<0的情况不管，会反方向绘制
                tma -= mintm/tmma;
                tmb -= mintm/tmmb;
                aga = (sa * tma * tma) / 2 + s0 * tma;
                agb = (sb * tmb * tmb) / 2 + s1 * tmb;
                agc = (ag - (aga + agb));
                // 通过tma,tmb算出合适的最大速度
                sm = (sa * tma + s0 + sb * tmb + s1) / 2;
                qDebug() << "sm" << sa * tma + s0 << sb * tmb + s1;
            }
            else
                qDebug("wujie");
        }

        qreal tmc = agc / sm;
        m_pos[0 + pi].setX(m_tmsum);
        m_pos[0 + pi].setY(s0 * fz);
        m_pos[1 + pi].setX(m_tmsum + tma);
        m_pos[1 + pi].setY(sm * fz);
        m_pos[2 + pi].setX(m_tmsum + tma + tmc);
        m_pos[2 + pi].setY(sm * fz);
        m_pos[3 + pi].setX(m_tmsum + tma + tmc + tmb);
        m_pos[3 + pi].setY(s1 * fz);
        m_tmsum += tma + tmc + tmb;
        if (lastStop) {
            lastStop = false;
            m_pos[pi-1].setY(m_pos[0 + pi].y());
        }

        if (m_smax < sm)
            m_smax = sm;
        if (m_smax < s0)
            m_smax = s0;
        if (m_smax < s1)
            m_smax = s1;
    }
    update();
}

// 边距
#define X0 10
#define Y0 10
#define X1 10
#define Y1 10

void GraphMotorMode::paintEvent(QPaintEvent *event)
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

    qreal tms = 0;
    for (int i = 0, pi = 0, num = 0; i < m_size; i+=m_datan, pi+=m_posn, num++) {
        qreal ag = qAbs(m_data[m_datai_ag + i]);
        qreal sm = qAbs(m_data[m_datai_sm + i]);

        qreal tm0 = tms;
        tms = m_pos[m_posn-1+pi].x();
        if (i == m_pressedi) {
            int x1 = qRound(tm0 * dx);
            int x2 = qRound(tms * dx);
            pt.setPen(Qt::NoPen);
            pt.setBrush(Qt::gray);
            pt.drawRect(x1+X0, ya, x2-x1+1, hh);
            pt.drawRect(x1+X0, yo+1, x2-x1+1, hh);
            pt.setPen(Qt::green);
        }

        QPoint p[4];
        p[0].setX(X0 + qRound(m_pos[0 + pi].x() * dx));
        p[0].setY(yo - qRound(m_pos[0 + pi].y() * dy));
        p[1].setX(X0 + qRound(m_pos[1 + pi].x() * dx));
        p[1].setY(yo - qRound(m_pos[1 + pi].y() * dy));
        p[2].setX(X0 + qRound(m_pos[2 + pi].x() * dx));
        p[2].setY(yo - qRound(m_pos[2 + pi].y() * dy));
        p[3].setX(X0 + qRound(m_pos[3 + pi].x() * dx));
        p[3].setY(yo - qRound(m_pos[3 + pi].y() * dy));
        pt.drawPolyline(p, 4);

        qreal textpx = X0 + (m_pos[0 + pi].x()+m_pos[3 + pi].x())*dx/2;
        pt.setPen(Qt::white);
        QRectF num_rect(textpx - 20, yo + 1, 40, 20);
        drawText(pt, num_rect, w, QString::number(num + 1));
        pt.setPen(Qt::green);
        QRectF num_rect2(textpx - 100, yo - 21, 200, 20);
        if (sm != 0)
            drawText(pt, num_rect2, w, QString::number(ag / 10, 'f', 1));
//        else
//            drawText(pt, num_rect2, w, QString::number(ag));
    }
}

void GraphMotorMode::mousePressEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();
    int w = width();
    int h = height();
    if (!(x >= X0 && x < w - X1 && y >= Y0 && y < h - Y1))
        return;

    qreal dx = (qreal)(w - X0 - X1) / m_tmsum;
    x -= X0;

    for (int i = 0, pi = 0; i < m_size; i+=m_datan, pi+=m_posn) {
        qreal tm = m_pos[m_posn-1+pi].x();
        int tmx = qRound(tm * dx);
        if (x < tmx) {
            m_pressedi = i;
            break;
        }
    }
    update();
}

void GraphMotorMode::mouseReleaseEvent(QMouseEvent *)
{
    if (m_pressedi == -1)
        return;
    clicked(m_pressedi);
    m_pressedi = -1;
    update();
}
