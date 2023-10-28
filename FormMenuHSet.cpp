#include "FormMenuHSet.h"
#include "ui_FormMenuHSet.h"
#include <QButtonGroup>

FormMenuHSet::FormMenuHSet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuHSet)
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
    btg->addButton(ui->pushButton_7, 6);
    connect(btg, SIGNAL(buttonClicked(int)),
            ui->page, SLOT(setCurrentIndex(int)));
    ui->pushButton->click();
    ui->pushButton_3->hide();
}

FormMenuHSet::~FormMenuHSet()
{
    delete ui;
}

void FormMenuHSet::init(FormA *f)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP., form->mpri);

    ui->page_SysParam->init(form);
    ui->page_MM->init(form);
    ui->page_MMA->init(form);
    ui->page_PL->init(form);
    ui->page_Sortor->init(form);
    ui->page_PBak->init(form);
    ui->page_Mainboard->init(form);
}

void FormMenuHSet::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
}

void FormMenuHSet::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
    if (ui->page->currentWidget() == ui->page_Mainboard)
        ui->pushButton->click();
}

void FormMenuHSet::refreshUI()
{
}

void FormMenuHSet::onRegUpdated(const MRegAreas &areas)
{
    Q_UNUSED(areas);
}
