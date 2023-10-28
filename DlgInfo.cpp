#include "DlgInfo.h"
#include "ui_DlgInfo.h"

DlgInfo::DlgInfo(QWidget *parent) :
    DlgArm(parent),
    ui(new Ui::DlgInfo)
{
    ui->setupUi(this);

    connect(ui->OK, SIGNAL(clicked()), this, SLOT(accept()));
}

DlgInfo::~DlgInfo()
{
    delete ui;
}

void DlgInfo::setText(const QString &str)
{
    ui->label->setText(str);
}

void DlgInfo::info(QWidget *p, const QString &str)
{
    DlgInfo dlg(p);
    dlg.ui->label->setText(str);
    dlg.exec();
}
