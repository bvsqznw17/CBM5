#ifndef DIALOGGRAPHBAR_H
#define DIALOGGRAPHBAR_H

#include "DlgArm.h"
#include "GraphBar.h"

namespace Ui {
class DialogGraphBar;
}

class DialogGraphBar : public DlgArm
{
    Q_OBJECT

public:
    explicit DialogGraphBar(QWidget *parent = nullptr);
    ~DialogGraphBar();

    GraphBar *graph();
    void setDistanceValue(int d);

private slots:
    void onRangeChanged(int min, int max);
    void on_spinBox_valueChanged(int arg1);

private:
    Ui::DialogGraphBar *ui;
};

#endif // DIALOGGRAPHBAR_H
