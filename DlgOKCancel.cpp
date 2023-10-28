#include "DlgOKCancel.h"
#include "ui_DlgOKCancel.h"

DlgOKCancel::DlgOKCancel(QWidget *parent) :
    DlgArm(parent),
    ui(new Ui::DlgOKCancel)
{
    ui->setupUi(this);

    connect(ui->OK, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->Cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

DlgOKCancel::~DlgOKCancel()
{
    delete ui;
}

void DlgOKCancel::setText(const QString &str)
{
    ui->label->setText(str);
}
