#include "FormMenuIO3.h"
#include "ui_FormMenuIO3.h"

FormMenuIO3::FormMenuIO3(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuIO3)
{
    ui->setupUi(this);
}

FormMenuIO3::~FormMenuIO3()
{
    delete ui;
}

void FormMenuIO3::init(FormA *f)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP., form->mpri);
}

void FormMenuIO3::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
}

void FormMenuIO3::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormMenuIO3::refreshUI()
{
}

void FormMenuIO3::onRegUpdated(const MRegAreas &areas)
{
    Q_UNUSED(areas);
}
