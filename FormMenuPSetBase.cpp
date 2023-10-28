#include "FormMenuPSetBase.h"
#include "ui_FormMenuPSetBase.h"

QStringList trs_bases_3;

FormMenuPSetBase::FormMenuPSetBase(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuPSetBase)
{
    if (trs_bases_3.isEmpty())
        trs_bases_3 << tr("重量-重量") << tr("重量-包数")
                    << tr("包数-包数") << tr("包数-皮重");

    ui->setupUi(this);
}

FormMenuPSetBase::~FormMenuPSetBase()
{
    delete ui;
}

void FormMenuPSetBase::init(FormA *f)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP.Prm, form->mpri);

#define MyReplace(I) form->connectMPEdit(ui->val_##I, trs_bases_##I)
    MyReplace(3);
#undef MyReplace

    starti = form->keybm->startadd();
    // 基本参数
    form->keybm->add(KEYBMET(ui->val_1), MP.prm_name);
    form->keybm->add(KEYBMET(ui->val_2), MP.prm_speed);
    form->keybm->add(KEYBMET(ui->val_3), MP.prm_WorP);

    form->keybm->add(KEYBMET(ui->val_4), MP.prm_SetWeight);
    form->keybm->add(KEYBMET(ui->val_5), MP.prm_SetWeight1);
    form->keybm->add(KEYBMET(ui->val_6), MP.prm_SetWeight2);
    form->keybm->add(KEYBMET(ui->val_7), MP.prm_PCL);

    form->keybm->add(KEYBMET(ui->val_8), MP.prm_DWW);
    form->keybm->add(KEYBMET(ui->val_9), MP.prm_DWW_UpLimit);
    form->keybm->add(KEYBMET(ui->val_10), MP.prm_DWW_DownLimit);
    form->keybm->add(KEYBMET(ui->val_11), MP.prm_TWW);

    // 中级参数
    form->keybm->add(KEYBMET(ui->val_m_1), MP.prm_WL_check);
    form->keybm->add(KEYBMET(ui->val_m_2), MP.prm_NoSel_check);
    form->keybm->add(KEYBMET(ui->val_m_3), MP.prm_MFL_Cnt);
    form->keybm->add(KEYBMET(ui->val_m_4), MP.prm_JLD_MFLNum);

    form->keybm->add(KEYBMET(ui->val_m_5), MP.prm_ASF);
    form->keybm->add(KEYBMET(ui->val_m_6), MP.prm_LDJL_Time);
    form->keybm->add(KEYBMET(ui->val_m_7), MP.prm_ZZJ_ULW);
    form->keybm->add(KEYBMET(ui->val_m_8), MP.prm_ZZJ_DLW);

    form->keybm->add(KEYBMET(ui->val_m_9), MP.prm_ZZJ_QLTJ);
    form->keybm->add(KEYBMET(ui->val_m_10), MP.prm_CombinWeight);
    form->keybm->add(KEYBMET(ui->val_m_11), MP.prm_OverWeight);
    form->keybm->add(KEYBMET(ui->val_m_12), MP.prm_P2Cnt);
    endi = form->keybm->endadd();
}

void FormMenuPSetBase::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
}

void FormMenuPSetBase::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormMenuPSetBase::refreshUI()
{
    form->keybm->refreshMParam(starti, endi);
#define MyReplace(I) form->refreshMPEdit(ui->val_##I, trs_bases_##I)
    MyReplace(3);
#undef MyReplace

    int WorP = MP.prm_WorP->value().toInt();
    ui->val_9->setEnabled(WorP == 2);
    ui->val_10->setEnabled(WorP == 2);
    ui->val_11->setEnabled(WorP == 3);
    ui->val_m_12->setEnabled(WorP == 2);
    int jld = MP.sys_JLD_state->vInt();
    int flfs = MP.sys_FL_FS->vInt();
    ui->val_m_3->setEnabled(!(jld && flfs));
    ui->val_m_4->setEnabled(!(jld && flfs));
}

void FormMenuPSetBase::onRegUpdated(const MRegAreas &areas)
{
    if (areas.isIn(MMP.Prm))
        refreshUI();
}
