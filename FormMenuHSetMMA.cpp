#include "FormMenuHSetMMA.h"
#include "ui_FormMenuHSetMMA.h"
#include <QButtonGroup>

FormMenuHSetMMA::FormMenuHSetMMA(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuHSetMMA)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);

    QButtonGroup *btg = new QButtonGroup(this);
    btg->addButton(ui->pushButton, 0);
    btg->addButton(ui->pushButton_2, 1);
    connect(btg, SIGNAL(buttonClicked(int)),
            ui->page, SLOT(setCurrentIndex(int)));
    ui->pushButton->click();
}

FormMenuHSetMMA::~FormMenuHSetMMA()
{
    delete ui;
}

void FormMenuHSetMMA::init(FormA *f)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP., form->mpri);

    ui->page_1->init(form);
    ui->page_2->init(form);
    ui->page_3->init(form);
}

void FormMenuHSetMMA::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
}

void FormMenuHSetMMA::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormMenuHSetMMA::refreshUI()
{
}

void FormMenuHSetMMA::onRegUpdated(const MRegAreas &areas)
{
    Q_UNUSED(areas);
}
