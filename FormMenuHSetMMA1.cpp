#include "FormMenuHSetMMA1.h"
#include "ui_FormMenuHSetMMA1.h"

FormMenuHSetMMA1::FormMenuHSetMMA1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuHSetMMA1)
{
    ui->setupUi(this);
    ui->val_7->hide();
}

FormMenuHSetMMA1::~FormMenuHSetMMA1()
{
    delete ui;
}

void FormMenuHSetMMA1::init(FormA *f)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP., form->mpri);
}

void FormMenuHSetMMA1::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
}

void FormMenuHSetMMA1::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormMenuHSetMMA1::refreshUI()
{
}

void FormMenuHSetMMA1::onRegUpdated(const MRegAreas &areas)
{
    Q_UNUSED(areas);
}
