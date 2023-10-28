#ifndef DIALOGDOUSTATEADJUST_H
#define DIALOGDOUSTATEADJUST_H

#include "DlgArm.h"

namespace Ui {
class DialogDouStateAdjust;
}

class DialogDouStateAdjust : public DlgArm
{
    Q_OBJECT

public:
    explicit DialogDouStateAdjust(QWidget *parent = nullptr);
    ~DialogDouStateAdjust();

    void setContent(const QImage &img, int h, int s, int l, bool b);

    QImage img() { return m_img; }
    int hue();
    int saturation();
    int lightness();
    bool isShow0f();

private slots:
    void refreshImage();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::DialogDouStateAdjust *ui;
    QImage m_src;
    QImage m_img;
};

#endif // DIALOGDOUSTATEADJUST_H
