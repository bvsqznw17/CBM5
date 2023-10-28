#include "FormMenuIO4.h"
#include "ui_FormMenuIO4.h"

FormMenuIO4::FormMenuIO4(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuIO4)
{
    ui->setupUi(this);
}

FormMenuIO4::~FormMenuIO4()
{
    delete ui;
}

void FormMenuIO4::init(FormA *f)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP., form->mpri);
}

void FormMenuIO4::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
}

void FormMenuIO4::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormMenuIO4::refreshUI()
{
}

void FormMenuIO4::onRegUpdated(const MRegAreas &areas)
{
    Q_UNUSED(areas);
}
