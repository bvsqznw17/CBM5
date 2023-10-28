#include "FormMenuHSetSortor.h"
#include "ui_FormMenuHSetSortor.h"

FormMenuHSetSortor::FormMenuHSetSortor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuHSetSortor)
{
    ui->setupUi(this);
}

FormMenuHSetSortor::~FormMenuHSetSortor()
{
    delete ui;
}

void FormMenuHSetSortor::init(FormA *f)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP., form->mpri);
}

void FormMenuHSetSortor::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
}

void FormMenuHSetSortor::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormMenuHSetSortor::refreshUI()
{
}

void FormMenuHSetSortor::onRegUpdated(const MRegAreas &areas)
{
    Q_UNUSED(areas);
}
