#include "DialogDouStateAdjust.h"
#include "ui_DialogDouStateAdjust.h"
#include "Doum.h"

DialogDouStateAdjust::DialogDouStateAdjust(QWidget *parent) :
    DlgArm(parent),
    ui(new Ui::DialogDouStateAdjust)
{
    ui->setupUi(this);
    connect(ui->OK, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->Cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(refreshImage()));
    connect(ui->spinBox_2, SIGNAL(valueChanged(int)), this, SLOT(refreshImage()));
    connect(ui->spinBox_3, SIGNAL(valueChanged(int)), this, SLOT(refreshImage()));
}

DialogDouStateAdjust::~DialogDouStateAdjust()
{
    delete ui;
}

void DialogDouStateAdjust::setContent(const QImage &img, int h, int s, int l, bool b)
{
    m_src = img;
    ui->spinBox->blockSignals(true);
    ui->spinBox_2->blockSignals(true);
    ui->spinBox_3->blockSignals(true);
    ui->spinBox->setValue(h);
    ui->spinBox_2->setValue(s);
    ui->spinBox_3->setValue(l);
    ui->horizontalSlider->setValue(h);
    ui->horizontalSlider_2->setValue(s);
    ui->horizontalSlider_3->setValue(l);
    ui->spinBox->blockSignals(false);
    ui->spinBox_2->blockSignals(false);
    ui->spinBox_3->blockSignals(false);
    refreshImage();
    ui->checkBox->setChecked(b);
}

int DialogDouStateAdjust::hue()
{
    return ui->spinBox->value();
}

int DialogDouStateAdjust::saturation()
{
    return ui->spinBox_2->value();
}

int DialogDouStateAdjust::lightness()
{
    return ui->spinBox_3->value();
}

bool DialogDouStateAdjust::isShow0f()
{
    return ui->checkBox->isChecked();
}

void DialogDouStateAdjust::refreshImage()
{
    int h = ui->spinBox->value();
    int s = ui->spinBox_2->value();
    int l = ui->spinBox_3->value();
    m_img = Doum::adjustHSL(m_src, h, s, l);
    ui->dou->setPixmap(QPixmap::fromImage(m_img));
}

void DialogDouStateAdjust::on_pushButton_clicked()
{
    if (ui->spinBox->value() == ui->spinBox->minimum())
        ui->spinBox->setValue(ui->spinBox->maximum());

    ui->spinBox->stepDown();
}

void DialogDouStateAdjust::on_pushButton_2_clicked()
{
    if (ui->spinBox->value() == ui->spinBox->maximum())
        ui->spinBox->setValue(ui->spinBox->minimum());

    ui->spinBox->stepUp();
}
