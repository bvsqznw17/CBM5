#include "FormHelp.h"
#include "ui_FormHelp.h"

FormHelp::FormHelp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormHelp)
{
    ui->setupUi(this);
    ui->page->setCurrentIndex(0);
}

FormHelp::~FormHelp()
{
    delete ui;
}

void FormHelp::init(FormA *f)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP., form->mpri);

    ui->page_1->init(form);
    ui->page_2->init(form);
}

void FormHelp::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
}

void FormHelp::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormHelp::refreshUI()
{
}

void FormHelp::onRegUpdated(const MRegAreas &areas)
{
    Q_UNUSED(areas);
}

void FormHelp::on_menu_clicked()
{
    form->showMain();
}

void FormHelp::on_nextPage_clicked()
{
    if (ui->page->currentIndex() == 0)
        ui->page->setCurrentIndex(1);
    else
        ui->page->setCurrentIndex(0);
}
