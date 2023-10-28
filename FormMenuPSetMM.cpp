#include "FormMenuPSetMM.h"
#include "ui_FormMenuPSetMM.h"

FormMenuPSetMM::FormMenuPSetMM(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuPSetMM)
{
    ui->setupUi(this);

    ui->groupBox->hide();
}

FormMenuPSetMM::~FormMenuPSetMM()
{
    delete ui;
}

void FormMenuPSetMM::init(FormA *f)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP., form->mpri);

    starti = form->keybm->startadd();
    form->keybm->add(KEYBMET(ui->mm_main), MP.prm_ZZMDMode);
    form->keybm->add(KEYBMET(ui->mm_1), MP.prm_MaDaMode[0]);
    form->keybm->add(KEYBMET(ui->mm_2), MP.prm_MaDaMode[1]);
    form->keybm->add(KEYBMET(ui->mm_3), MP.prm_MaDaMode[3]);
    endi = form->keybm->endadd();
}

void FormMenuPSetMM::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
}

void FormMenuPSetMM::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormMenuPSetMM::refreshUI()
{
    form->keybm->refreshMParam(starti, endi);
}

void FormMenuPSetMM::onRegUpdated(const MRegAreas &areas)
{
    if (areas.isIn(MMP.Prm))
        refreshUI();
}
