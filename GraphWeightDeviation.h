#ifndef GRAPHWEIGHTDEVIATION_H
#define GRAPHWEIGHTDEVIATION_H

#include <QWidget>
#include <qmath.h>
#include "RingBuffer.h"

class GraphWeightDeviation : public QWidget
{
    Q_OBJECT
public:
    explicit GraphWeightDeviation(QWidget *parent = nullptr);

    void clear();
    void add(int w);
    void add(int *w, int n);
    void setWeight(int _w0, int _w1, int _w2, int _p);

private:
    inline static QString getDotStr(int n, int dot)
    { return QString::number(n * pow(10, -dot), 'f', dot); }
    void updateText();
    void paintEvent(QPaintEvent *event) override;

private:
    RingBufferLimited<int> rb;
    int w0, w1, w2, p;
    QString ytext[5];
};

#endif // GRAPHWEIGHTDEVIATION_H
