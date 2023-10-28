#ifndef GRAPHMOTORMODE_H
#define GRAPHMOTORMODE_H

#include <QWidget>

class GraphMotorMode : public QWidget
{
    Q_OBJECT
public:
    explicit GraphMotorMode(QWidget *parent = nullptr);
    ~GraphMotorMode();

    void setData(const int *data, int size);

signals:
    void clicked(int i);

private:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    int *m_data;
    int m_size;
    QPointF *m_pos;
    qreal m_smax;
    qreal m_tmsum;
    int m_pressedi;
};

#endif // GRAPHMOTORMODE_H
