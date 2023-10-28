#include "FormMenuHSetMM.h"
#include "ui_FormMenuHSetMM.h"
#include <QButtonGroup>

FormMenuHSetMM::FormMenuHSetMM(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuHSetMM)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);

    QButtonGroup *btg = new QButtonGroup(this);
    btg->addButton(ui->pushButton, 0);
    btg->addButton(ui->pushButton_2, 1);
    btg->addButton(ui->pushButton_3, 2);
    btg->addButton(ui->pushButton_4, 3);
    connect(btg, SIGNAL(buttonClicked(int)),
            ui->page, SLOT(setCurrentIndex(int)));
    ui->pushButton->click();
}

FormMenuHSetMM::~FormMenuHSetMM()
{
    delete ui;
}

void FormMenuHSetMM::init(FormA *f)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP., form->mpri);

    ui->page_1->init(form, MM_BUFFER);
    ui->page_2->init(form, MM_WEIGHT);
    ui->page_3->init(form, MM_COLLECT);
    ui->page_4->init(form, MM_MAIN_VIB);
    MP.motor_id->setValue(0xFFFF);
}

void FormMenuHSetMM::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
}

void FormMenuHSetMM::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormMenuHSetMM::refreshUI()
{
}

void FormMenuHSetMM::onRegUpdated(const MRegAreas &areas)
{
    Q_UNUSED(areas);
}
