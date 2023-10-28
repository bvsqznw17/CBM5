#ifndef GRAPHZHENDONG_H
#define GRAPHZHENDONG_H

#include <QWidget>
#include "RingBuffer.h"

class GraphZhenDong : public QWidget
{
    Q_OBJECT
public:
    explicit GraphZhenDong(QWidget *parent = nullptr);

    void add(int w1, int w2, int upn = 1);
    void clear();

private:
    void paintEvent(QPaintEvent *event) override;

private:
    RingBufferLimited<int> rb1;
    RingBufferLimited<int> rb2;
    int m_max;
};

#endif // GRAPHZHENDONG_H
