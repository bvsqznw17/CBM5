#ifndef GRAPHSAMPING_H
#define GRAPHSAMPING_H

#include <QWidget>

class GraphSamping : public QWidget
{
    Q_OBJECT
public:
    explicit GraphSamping(QWidget *parent = nullptr);
    ~GraphSamping();

    void setData(const qint16 *data, int size);
    void setLine(int l1, int l2, int l1ms);
    inline int max() { return m_max; }

private:
    void paintEvent(QPaintEvent *event) override;

private:
    qint16 *m_data;
    int m_size;
    int m_max;
    int m_min;

    int m_line1;
    int m_line2;
    int m_line1ms;
};

#endif // GRAPHSAMPING_H
