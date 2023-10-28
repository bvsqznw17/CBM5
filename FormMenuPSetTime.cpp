#include "FormMenuPSetTime.h"
#include "ui_FormMenuPSetTime.h"

FormMenuPSetTime::FormMenuPSetTime(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuPSetTime)
{
    ui->setupUi(this);
}

FormMenuPSetTime::~FormMenuPSetTime()
{
    delete ui;
}

void FormMenuPSetTime::init(FormA *f)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP., form->mpri);

    starti = form->keybm->startadd();
    form->keybm->add(KEYBMET(ui->p_1), MP.prm_HCD_Dly);
    form->keybm->add(KEYBMET(ui->p_2), MP.prm_Dou_PauseTime[0]);
    form->keybm->add(KEYBMET(ui->p_3), MP.prm_CZD_Dly);
    form->keybm->add(KEYBMET(ui->p_4), MP.prm_Dou_PauseTime[1]);
    form->keybm->add(KEYBMET(ui->p_5), MP.prm_JLD_Dly);
    form->keybm->add(KEYBMET(ui->p_6), MP.prm_Dou_PauseTime[3]);
    form->keybm->add(KEYBMET(ui->p_7), MP.prm_JLD_OpenTime);

    form->keybm->add(KEYBMET(ui->p_8), MP.prm_FL_dly);
    form->keybm->add(KEYBMET(ui->p_9), MP.prm_FL_time);
    form->keybm->add(KEYBMET(ui->p_10), MP.prm_ZHFL_time);
    form->keybm->add(KEYBMET(ui->p_11), MP.prm_MSV_Dly);
    form->keybm->add(KEYBMET(ui->p_12), MP.prm_AutoZeroTime);
    form->keybm->add(KEYBMET(ui->p_13), MP.prm_AutoZeroCnt);

    form->keybm->add(KEYBMET(ui->p_14), MP.prm_JYD_Dly);
    form->keybm->add(KEYBMET(ui->p_15), MP.prm_Dou_PauseTime[2]);
    form->keybm->add(KEYBMET(ui->p_16), MP.prm_JYD_Dly2);
    endi = form->keybm->endadd();
}

void FormMenuPSetTime::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
}

void FormMenuPSetTime::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormMenuPSetTime::refreshUI()
{
    form->keybm->refreshMParam(starti, endi);
    int zdzlfs = MP.sys_AutoZeroMode->value().toInt();
    bool b = zdzlfs == 1 || zdzlfs == 3;
    ui->p_12->setVisible(!b);
    ui->p_13->setVisible(b);
    bool qbdg = MP.sys_QPDGSel->value().toInt();
    ui->p_14->setEnabled(qbdg);
    ui->p_15->setEnabled(qbdg);
    ui->p_16->setEnabled(qbdg);
}

void FormMenuPSetTime::onRegUpdated(const MRegAreas &areas)
{
    if (areas.isIn(MMP.Prm))
        refreshUI();
}
