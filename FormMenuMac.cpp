#include "FormMenuMac.h"
#include "ui_FormMenuMac.h"
#include "MTools.h"
#include "Tool.h"
#include "JServer_Mac.h"
#include "JServer_NetUp.h"
#include "ModbusTcpServer.h"
#include "JsonTcpServer.h"
#include <QButtonGroup>

FormMenuMac::FormMenuMac(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuMac)
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

FormMenuMac::~FormMenuMac()
{
    delete ui;
}

void FormMenuMac::init(FormA *f)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP., form->mpri);

    ui->page_1->init(form);
    ui->page_2->init(form);
}

void FormMenuMac::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
}

void FormMenuMac::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormMenuMac::refreshUI()
{
}

void FormMenuMac::onRegUpdated(const MRegAreas &areas)
{
    Q_UNUSED(areas);
}
