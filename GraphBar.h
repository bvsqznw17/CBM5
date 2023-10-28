#ifndef GRAPHBAR_H
#define GRAPHBAR_H

#include <QWidget>
#include <QVector>
#include <qmath.h>

class GraphBar : public QWidget
{
    Q_OBJECT
public:
    explicit GraphBar(QWidget *parent = nullptr);

    void clear();
    void setData(QVector<int> _data);
    void setRange(int _w0, int _w1, int _w2, int _p);
    void setDistance(int d);
    void adjustMinSize();
    double gety0bd();

private:
    inline static QString getDotStr(int n, int dot)
    { return QString::number(n * pow(10, -dot), 'f', dot); }
    void paintItem(QPainter &pt, int y, int fmw1, int fmw2, int fmh,
                   int ww, int wv, int wn, int wnmax);
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<int> data;
    int w0, w1, w2, p;
    int dw;

};

#endif // GRAPHBAR_H
