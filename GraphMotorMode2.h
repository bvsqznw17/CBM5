#ifndef GRAPHMOTORMODE2_H
#define GRAPHMOTORMODE2_H

#include <QWidget>

class GraphMotorMode2 : public QWidget
{
    Q_OBJECT
public:
    explicit GraphMotorMode2(QWidget *parent = nullptr);
    ~GraphMotorMode2();

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
    int m_smax;
    int m_tmsum;
    int m_pressedi;

};

#endif // GRAPHMOTORMODE2_H
