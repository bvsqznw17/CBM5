#include "sampingcurve.h"
#include "QPainter"
#include "QMouseEvent"
#include "QDebug"
#include "math.h"

SampingCurve::SampingCurve(QWidget *parent) :
    QWidget(parent)
{
    //背景色初始化
    m_bkColor = QColor(64,64,64);//白色
    m_eMouseAction = MOUSE_ACTION_UNDEFINE;
    m_nStartNo = 0;
    m_nXScale = 0;
    m_nDouDelay = 0;    //称重斗延时
    m_nSampingTime = 0; //采样时间


    m_nMax = 0;
    for(int i = 0; i < 1024; i++)
    {
        m_CurveData[i] = 0;
    }
}

void SampingCurve::SetBKColor(QColor color)
{
    m_bkColor = color;
}

void SampingCurve::ClearData()
{
     m_nMax = 0;
     m_nXScale = 0;

     for(int i = 0; i < 1024; i++)
     {
         m_CurveData[i] = 0;
     }

     update();
}

void SampingCurve::paintEvent(QPaintEvent *)
{
    QPainter dc(this);
    QRect rcClient = this->rect();

    dc.setPen(m_bkColor);
    dc.setBrush(m_bkColor);//设置绘制背景的笔的颜色
    dc.drawRect(rcClient);//0,0,rcClient.width(),rcClient.height());//绘制背景

    DrawAxis(dc,rcClient);

    QRect rCurve(30,10,rcClient.width()-30-10,rcClient.height()-10-60);

    DrawPoint(dc,rCurve);

}

void SampingCurve::mousePressEvent(QMouseEvent *e)
{
    m_pointPress = e->pos();
}

void SampingCurve::mouseReleaseEvent(QMouseEvent *e)
{
    m_pointRelease = e->pos();


//    ParseMouseEvent();
}

void SampingCurve::DrawAxis(QPainter &dc, QRect rect)
{    //坐标轴
    QPen pen(Qt::white);
    pen.setWidth(1);
    dc.setPen(pen);

    //x轴
    dc.drawLine(rect.left()+30,rect.bottom()-10,rect.left()+30,rect.top()+10);

    //y轴
    dc.drawLine(rect.left()+30,rect.bottom()-60,rect.right()-10,rect.bottom()-60);

    dc.drawLine(rect.left()+30,rect.top()+10,rect.left()+25,rect.top()+15);
    dc.drawLine(rect.left()+30,rect.top()+10,rect.left()+35,rect.top()+15);

    dc.drawLine(rect.right()-10,rect.bottom()-60,rect.right()-15,rect.bottom()-55);
    dc.drawLine(rect.right()-10,rect.bottom()-60,rect.right()-15,rect.bottom()-65);

    //0点
    QRect rect1 = QRect(rect.left()+10,rect.bottom()-70,20,20);
    dc.drawText(rect1,Qt::AlignCenter,"0");
}

void SampingCurve::DrawAxis1(QPainter &dc, QRect rect)
{
    //坐标轴
    QPen pen(Qt::gray);
    pen.setWidth(2);
    dc.setPen(pen);

    //x轴
    dc.drawLine(rect.bottomLeft(),rect.bottomRight());
    //y轴
    dc.drawLine(rect.bottomLeft(),rect.topLeft());

    //刻度
    pen.setWidth(2);
    dc.setPen(pen);
    for(int i = 1 ; i <= 10;i++)
    {
        int x0 = rect.left();
        int y0 = rect.bottom() - rect.height()*i/10;
        int x1 = x0 + 5;
        int y1 = y0;

        if(i == 5)
            x1 = x0 + 10;
        pen.setColor(Qt::gray);
        dc.setPen(pen);
        dc.drawLine(QPoint(x0,y0),QPoint(x1,y1));

        if(1)
        {
            x0 = rect.left()+10;
            y0 = rect.bottom() - rect.height()*i/10;
            x1 = rect.right();
            y1 = y0;
            pen.setStyle(Qt::DashDotLine);
            pen.setColor(QColor(215,242,232));
            dc.setPen(pen);
            dc.drawLine(QPoint(x0,y0),QPoint(x1,y1));
        }
    }

    pen.setColor(Qt::gray);
    dc.setPen(pen);
    for(int i = 1 ; i <= 10;i++)
    {
        int x0 = rect.left() + rect.width()*i/10;
        int y0 = rect.bottom();
        int x1 = x0;
        int y1 = y0 - 5;

        if(i == 5)
            y1 = y0 - 10;
        pen.setColor(Qt::gray);
        dc.setPen(pen);
        dc.drawLine(QPoint(x0,y0),QPoint(x1,y1));

//        if(bDrawGrid)
//        {
//            y0 = y0 -10;
//            y1 = rect.top();
//            pen.setStyle(Qt::DashDotLine);
//            pen.setColor(QColor(215,242,232));
//            dc.setPen(pen);
//            dc.drawLine(QPoint(x0,y0),QPoint(x1,y1));
//        }
    }

    QBrush brush(Qt::red);
    dc.setBrush(brush);
    QPoint yPoints[3] = {
            QPoint(rect.left()-5,rect.top()),
            QPoint(rect.left()+5,rect.top()),
            QPoint(rect.left(),rect.top()-10)
            };
    dc.drawPolygon(yPoints,3);

    QPoint xPoints[3] = {
            QPoint(rect.right(),rect.bottom()-5),
            QPoint(rect.right(),rect.bottom()+5),
            QPoint(rect.right()+10,rect.bottom())
            };
    dc.drawPolygon(xPoints,3);
}

void SampingCurve::DrawPoint(QPainter &dc, QRect rect)
{
   if(m_nXScale == 0) return;

   QPoint p1,p2;
   QPen pen(QColor(255,56,12));
   dc.setPen(pen);

    p1.setX(rect.left()+5+0*700/m_nXScale);

   if(m_nMax == 0)
       p1.setY(rect.bottom());
   else
       p1.setY(rect.bottom()- m_CurveData[0]*300/m_nMax);

   for(int i = 1; i <  m_nXScale; i++)
   {

       p2.setX(rect.left()+5+i*700/m_nXScale);

       if(m_nMax == 0)
           p2.setY(rect.bottom());
       else
           p2.setY(rect.bottom()-m_CurveData[i]*300/m_nMax);

       dc.drawLine(p1,p2);
       p1=p2;
   }

   //画竖线
   pen.setColor(QColor(0,0,222));
   dc.setPen(pen);
   dc.setFont(QFont("wenquanyi",8));

   int x = rect.left()+5+m_nDouDelay/10*700/m_nXScale;


   dc.drawLine(x,rect.bottom()+30,x,rect.bottom()-300);

   QRect rect1 = QRect(x - 40,rect.bottom()+30,80,15);
   QString strText = QString("%1ms").arg(m_nDouDelay);
   dc.drawText(rect1,Qt::AlignCenter,strText);

   pen.setColor(QColor(0,222,0));
   dc.setPen(pen);

   x = rect.left()+5+m_nSampingTime*700/m_nXScale;
   dc.drawLine(x,rect.bottom()+30,x,rect.bottom()-300);

   rect1 = QRect(x - 40,rect.bottom()+30,80,15);
   strText = QString("%1ms").arg(m_nSampingTime*10);
   dc.drawText(rect1,Qt::AlignCenter,strText);
}

void SampingCurve::ParseMouseEvent()
{
    int x1 = m_pointPress.x();
    int y1 = m_pointPress.y();
    int x2 = m_pointRelease.x();
    int y2 = m_pointRelease.y();
    double pi = 3.1415926;

    int nDistance = (int)(sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)));
    double dbAngle = 0.0;

    if(nDistance < 100) m_eMouseAction = MOUSE_ACTION_UNDEFINE;
    else
    {
        if( y1 == y2 )
        {
            if(x2 > x1) dbAngle = 0.0;
            else        dbAngle = pi;
        }
        else if( x2 == x1)
        {
            if(y2 > y1) dbAngle = pi*3/2;
            else        dbAngle = pi/2;
        }
        else
        {
            double k = abs((y2 - y1) / (x2 - x1));
            dbAngle = atan(k);

            if(x2 > x1 && y2 < y1)
            {
                //第一象限
                dbAngle = dbAngle + 0.0;
            }
            else if(x2 > x1 && y2 > y1)
            {
                //第四象限
                dbAngle = 2*pi - dbAngle;
            }
            else if(x2 < x1 && y2 < y1)
            {
                //第二象限
                dbAngle = pi - dbAngle;
            }
            else if(x2 < x1 && y2 > y1)
            {
                //第三象限
                dbAngle += pi;
            }
        }

        if(dbAngle >= pi*0/6 && dbAngle <=  pi*1/6)   m_eMouseAction = MOUSE_ACTION_RIGHT;
        else if(dbAngle >= pi*2/6 && dbAngle <=  pi*4/6)   m_eMouseAction = MOUSE_ACTION_UP;
        else if(dbAngle >= pi*5/6 && dbAngle <=  pi*7/6)   m_eMouseAction = MOUSE_ACTION_LEFT;
        else if(dbAngle >= pi*8/6 && dbAngle <=  pi*10/6)   m_eMouseAction = MOUSE_ACTION_DOWN;
        else if(dbAngle >= pi*11/6 && dbAngle <=  pi*12/6)   m_eMouseAction = MOUSE_ACTION_RIGHT;
    }

    QString strText = "";
    switch(m_eMouseAction)
    {
    case MOUSE_ACTION_UP:
        strText = "MOUSE_ACTION_UP\n";
        break;
    case MOUSE_ACTION_DOWN:
        strText = "MOUSE_ACTION_DOWN\n";
        break;
    case MOUSE_ACTION_RIGHT:
        strText = "MOUSE_ACTION_RIGHT\n";
        break;
    case MOUSE_ACTION_LEFT:
        strText = "MOUSE_ACTION_LEFT\n";
        break;
    default:
        break;
    }

//    qDebug()<<strText;

    if(m_eMouseAction == MOUSE_ACTION_UP )
    {
        if(m_nXScale == 128) m_nXScale = 256;
        else if(m_nXScale == 256) m_nXScale = 512;
        else if(m_nXScale == 512) m_nXScale = 1024;
        else  m_nXScale = 1024;
    }
    else if(m_eMouseAction == MOUSE_ACTION_DOWN)
    {
        if(m_nXScale == 1024) m_nXScale = 512;
        else if(m_nXScale == 512) m_nXScale = 256;
        else if(m_nXScale == 256) m_nXScale = 128;
        else  m_nXScale = 128;
    }
    else if(m_eMouseAction == MOUSE_ACTION_LEFT)
    {
        m_nStartNo -= m_nXScale;
        if(m_nStartNo < 0) m_nStartNo = 0;
    }
    else if(m_eMouseAction == MOUSE_ACTION_RIGHT)
    {
        m_nStartNo += m_nXScale;
        if(m_nStartNo > 1024 - m_nXScale) m_nStartNo = 1024 - m_nXScale;
    }
}
