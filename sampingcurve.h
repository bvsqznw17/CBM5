#ifndef SAMPINGCURVE_H
#define SAMPINGCURVE_H

#include <QWidget>
#include "cbDef.h"

typedef enum tag_E_MOUSEACTION	//鼠标划动操作
{
    MOUSE_ACTION_UNDEFINE = 0,	//未定义
    MOUSE_ACTION_UP  = 1,		//上划操作--放大
    MOUSE_ACTION_DOWN = 2,		//下划操作--缩小
    MOUSE_ACTION_LEFT = 3,		//左划操作--左移
    MOUSE_ACTION_RIGHT = 4		//右划操作--右移
}E_MOUSEACTION;

class SampingCurve : public QWidget
{
    Q_OBJECT
public:
    explicit SampingCurve(QWidget *parent = 0);
    
    void SetBKColor(QColor color);	//设置背景色
    void ClearData();

    qint16 m_CurveData[1024];
    int m_nXScale;      //X轴点数
    int m_nMax;
    int m_nDouDelay;    //称重斗延时
    int m_nSampingTime; //采样时间

signals:
    
public slots:

protected://Impliment
    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    int m_iTimerID_RefreshUI;//UI refresh timer ID
    QColor m_bkColor;//背景颜色
    
    void DrawAxis(QPainter &dc,QRect rect);
    void DrawAxis1(QPainter &dc,QRect rect);
    void DrawPoint(QPainter &dc,QRect rect);

    QPoint m_pointPress;
    QPoint m_pointRelease;
    E_MOUSEACTION m_eMouseAction;
    int m_nStartNo;     //起始数据

    void ParseMouseEvent();     //解析鼠标操作事件

};

#endif // SAMPINGCURVE_H
