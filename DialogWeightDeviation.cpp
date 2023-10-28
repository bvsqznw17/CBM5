#include "DialogWeightDeviation.h"
#include "ui_DialogWeightDeviation.h"

DialogWeightDeviation::DialogWeightDeviation(QWidget *parent) :
    DlgArm(parent),
    ui(new Ui::DialogWeightDeviation)
{
    ui->setupUi(this);

    connect(ui->OK, SIGNAL(clicked()), this, SLOT(accept()));
}

DialogWeightDeviation::~DialogWeightDeviation()
{
    delete ui;
}

GraphWeightDeviation *DialogWeightDeviation::graph()
{
    return ui->widget;
}
