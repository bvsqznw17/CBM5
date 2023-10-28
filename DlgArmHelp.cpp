#include "DlgArmHelp.h"
#include "ui_DlgArmHelp.h"

DlgArmHelp::DlgArmHelp(QWidget *parent) :
    DlgArm(parent),
    ui(new Ui::DlgArmHelp)
{
    ui->setupUi(this);

    connect(ui->OK, SIGNAL(clicked()), this, SLOT(accept()));
}

DlgArmHelp::~DlgArmHelp()
{
    delete ui;
}

void DlgArmHelp::setText(const QString &str)
{
    ui->textBrowser->setText(str);
}
