#include "FormMenuHSetMM2.h"
#include "ui_FormMenuHSetMM2.h"

FormMenuHSetMM2::FormMenuHSetMM2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuHSetMM2)
{
    ui->setupUi(this);
}

FormMenuHSetMM2::~FormMenuHSetMM2()
{
    delete ui;
}

void FormMenuHSetMM2::init(FormA *f)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP., form->mpri);
}

void FormMenuHSetMM2::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
}

void FormMenuHSetMM2::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormMenuHSetMM2::refreshUI()
{
}

void FormMenuHSetMM2::onRegUpdated(const MRegAreas &areas)
{
    Q_UNUSED(areas);
}
