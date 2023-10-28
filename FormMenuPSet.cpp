#include "FormMenuPSet.h"
#include "ui_FormMenuPSet.h"
#include <QButtonGroup>

FormMenuPSet::FormMenuPSet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuPSet)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);

    QButtonGroup *btg = new QButtonGroup(this);
    btg->addButton(ui->pushButton, 0);
    btg->addButton(ui->pushButton_2, 1);
    btg->addButton(ui->pushButton_3, 2);
    btg->addButton(ui->pushButton_4, 3);
    btg->addButton(ui->pushButton_5, 4);
    btg->addButton(ui->pushButton_6, 5);
    connect(btg, SIGNAL(buttonClicked(int)),
            ui->page, SLOT(setCurrentIndex(int)));
    ui->pushButton->click();
}

FormMenuPSet::~FormMenuPSet()
{
    delete ui;
}

void FormMenuPSet::init(FormA *f)
{
    form = f;
    mptrw = new MPTRWidget(this, &form->mac, MMP.Prm, form->mpri);

    ui->page_Base->init(form);
    ui->page_Vib->init(form);
    ui->page_Time->init(form);
    ui->page_Other->init(form);
    ui->page_Specify->init(form);
    ui->page_MM->init(form);
}

void FormMenuPSet::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
}

void FormMenuPSet::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormMenuPSet::refreshUI()
{
}

void FormMenuPSet::onRegUpdated(const MRegAreas &areas)
{
    Q_UNUSED(areas);
}
