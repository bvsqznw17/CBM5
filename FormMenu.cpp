#include "FormMenu.h"
#include "ui_FormMenu.h"
#include "RecSave.h"
#include "MTools.h"
#include <QButtonGroup>

FormMenu::FormMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenu)
{
    ui->setupUi(this);
    QButtonGroup *btg = new QButtonGroup(this);
    btg->addButton(ui->pushButton, 0);
    btg->addButton(ui->pushButton_2, 1);
    btg->addButton(ui->pushButton_3, 2);
    btg->addButton(ui->pushButton_4, 3);
    btg->addButton(ui->pushButton_5, 4);
    btg->addButton(ui->pushButton_6, 5);
    btg->addButton(ui->pushButton_7, 6);
//    connect(btg, SIGNAL(buttonClicked(int)),
//            ui->page, SLOT(setCurrentIndex(int)));
    connect(btg, SIGNAL(buttonClicked(int)),
            this, SLOT(onButtonClicked(int)));
    ui->pushButton->click();
}

FormMenu::~FormMenu()
{
    delete ui;
}

void FormMenu::init(FormA *f)
{
    form = f;
    mptrw = new MPTRWidget(this, &form->mac, MP.prm_id, form->mpri*2);
    mptrw->reader.appendMP(MP.prm_name);

    ui->page_PList->init(form);
    ui->page_PSet->init(form);
    ui->page_IO->init(form);
    ui->page_Debug->init(form);
    ui->page_Calib->init(form);
    ui->page_HSet->init(form);
    ui->page_Mac->init(form);

    connect(MTools::instance(), SIGNAL(authChange(int)), this, SLOT(onAuthChanged(int)));
    onAuthChanged(MTools::auth());
}

void FormMenu::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();

    form->writeCMD(CTL_INTO_PAGE, 0x10);
    form->writeCMD(CTL_PRMSAVE, 1);
#ifdef MYOPT_TEST
    form->writeCMD(0, 0);
#endif
}

void FormMenu::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormMenu::refreshUI()
{
    QMutexLocker locker(&form->recs->pf_mutex);
    int id = MP.sys_prm_ids[0]->value().toInt();
    if (id <= 0 || id >= 100) {
        id = MP.prm_id->value().toInt() & 0xFF;
        if (id <= 0 || id >= 100)
            id = 0;
    }
    ui->prm_id->setText(QString::number(id));
    ui->prm_name->setText(form->recs->pf[id].name);
}

void FormMenu::onRegUpdated(const MRegAreas &areas)
{
    if (areas.isIn(MMP.Prm))
        refreshUI();
}

void FormMenu::onAuthChanged(int auth)
{
    ui->pushButton_2->setVisible(auth >= 1);
    ui->pushButton_3->setVisible(auth >= 2);
    ui->pushButton_4->setVisible(auth >= 3);
    ui->pushButton_5->setVisible(auth >= 3);
    ui->pushButton_6->setVisible(auth >= 3);
    ui->pushButton_7->setVisible(auth >= 3);

    if ((ui->pushButton_2->isChecked() && ui->pushButton_2->isHidden())
            || (ui->pushButton_3->isChecked() && ui->pushButton_3->isHidden())
            || (ui->pushButton_4->isChecked() && ui->pushButton_4->isHidden())
            || (ui->pushButton_5->isChecked() && ui->pushButton_5->isHidden())
            || (ui->pushButton_6->isChecked() && ui->pushButton_6->isHidden())
            || (ui->pushButton_7->isChecked() && ui->pushButton_7->isHidden())
            )
        ui->pushButton->click();
}

void FormMenu::onButtonClicked(int id)
{
    if (ui->page->currentWidget() == ui->page_Debug)
        ui->page_Debug->waitDebugStop();
    ui->page->setCurrentIndex(id);
}

void FormMenu::on_home_clicked()
{
    if (ui->page->currentWidget() == ui->page_Debug)
        ui->page_Debug->waitDebugStop();
    form->showMain();
}
