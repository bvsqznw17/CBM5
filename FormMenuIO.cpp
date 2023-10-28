#include "FormMenuIO.h"
#include "ui_FormMenuIO.h"
#include <QButtonGroup>

FormMenuIO::FormMenuIO(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuIO)
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

FormMenuIO::~FormMenuIO()
{
    delete ui;
}

void FormMenuIO::init(FormA *f)
{
    form = f;
    mptrw = new MPTRWidget(this, &form->mac, MMP.IO_Set, form->mpri);
    new MPTRWidget(this, &form->mac, MMP.IO_Status, 100);

    ui->page_1->init(form);
    ui->page_2->init(form);
    ui->page_3->init(form, 8);
    ui->page_4->init(form, 16);
}

void FormMenuIO::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
}

void FormMenuIO::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormMenuIO::refreshUI()
{
}

void FormMenuIO::onRegUpdated(const MRegAreas &areas)
{
    Q_UNUSED(areas);
}
