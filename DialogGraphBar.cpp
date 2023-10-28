#include "DialogGraphBar.h"
#include "ui_DialogGraphBar.h"
#include <QScrollBar>

DialogGraphBar::DialogGraphBar(QWidget *parent) :
    DlgArm(parent),
    ui(new Ui::DialogGraphBar)
{
    ui->setupUi(this);

    connect(ui->OK, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->pushButton_jia, SIGNAL(clicked()),
            ui->spinBox, SLOT(stepUp()));
    connect(ui->pushButton_jian, SIGNAL(clicked()),
            ui->spinBox, SLOT(stepDown()));

    QScrollBar *sb = ui->scrollArea->verticalScrollBar();
    connect(sb, SIGNAL(rangeChanged(int,int)),
            this, SLOT(onRangeChanged(int,int)));
}

DialogGraphBar::~DialogGraphBar()
{
    delete ui;
}

GraphBar *DialogGraphBar::graph()
{
    return ui->widget;
}

void DialogGraphBar::setDistanceValue(int d)
{
    ui->spinBox->blockSignals(true);
    ui->spinBox->setValue(d);
    ui->spinBox->blockSignals(false);
}

void DialogGraphBar::onRangeChanged(int min, int max)
{
    QScrollBar *sb = ui->scrollArea->verticalScrollBar();
    int v = ui->widget->gety0bd() * (max - min);
    sb->setValue(v + min);
}

void DialogGraphBar::on_spinBox_valueChanged(int arg1)
{
    ui->widget->setDistance(arg1);
    ui->widget->adjustMinSize();
}
