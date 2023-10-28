#ifndef DIALOGWEIGHTDEVIATION_H
#define DIALOGWEIGHTDEVIATION_H

#include "DlgArm.h"
#include "GraphWeightDeviation.h"

namespace Ui {
class DialogWeightDeviation;
}

class DialogWeightDeviation : public DlgArm
{
    Q_OBJECT

public:
    explicit DialogWeightDeviation(QWidget *parent = nullptr);
    ~DialogWeightDeviation();

    GraphWeightDeviation *graph();

private:
    Ui::DialogWeightDeviation *ui;
};

#endif // DIALOGWEIGHTDEVIATION_H
