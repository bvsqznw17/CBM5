#include "FormMenuHSetSysParam.h"
#include "ui_FormMenuHSetSysParam.h"

bool trs_sysp_init = true;
QStringList trs_sysp_1;
QStringList trs_sysp_2;
QStringList trs_sysp_4;
QStringList trs_sysp_7;
QStringList trs_sysp_22;
QStringList trs_sysp_8;
QStringList trs_sysp_9;
QStringList trs_sysp_10;
QStringList trs_sysp_12;
QStringList trs_sysp_13;
QStringList trs_sysp_14;
QStringList trs_sysp_15;
QStringList trs_sysp_19;

FormMenuHSetSysParam::FormMenuHSetSysParam(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuHSetSysParam)
{
    if (trs_sysp_init) {
        trs_sysp_init = false;
        trs_sysp_1 << tr("关") << tr("开");
        trs_sysp_2 << tr("关") << tr("开");
        trs_sysp_4 << ("g") << ("kg") << ("lb") << ("oz");
        trs_sysp_7 << tr("关") << tr("开");
        trs_sysp_22 << tr("关") << tr("开");
        trs_sysp_8 << tr("振动") << tr("旋转");
        trs_sysp_9 << tr("单循环") << tr("双循环")
                   << tr("多循环") << tr("异步");
        trs_sysp_10 << tr("计时间") << tr("计次数");
        trs_sysp_12 << tr("脉冲带记忆") << tr("脉冲不记忆")
                    << tr("电平带记忆") << tr("电平不记忆");
        trs_sysp_13 << tr("单集料斗") << tr("超差选别") << tr("AB各自");
        trs_sysp_14 << tr("间隔时间") << tr("总体时间");
        trs_sysp_15 << tr("关") << tr("开");
        trs_sysp_19 << tr("关") << tr("开");
    }

    ui->setupUi(this);
}

FormMenuHSetSysParam::~FormMenuHSetSysParam()
{
    delete ui;
}

void FormMenuHSetSysParam::init(FormA *f)
{
    form = f;
    mptrw = new MPTRWidget(this, &form->mac, MMP.Sys0, form->mpri);


#define MyReplace(I) form->connectMPEdit(ui->p_##I, trs_sysp_##I)
    MyReplace(1);
    MyReplace(2);
    MyReplace(4);
    MyReplace(7);
    MyReplace(22);
    MyReplace(8);
    MyReplace(9);
    MyReplace(10);
    MyReplace(12);
    MyReplace(13);
    MyReplace(14);
    MyReplace(15);
    MyReplace(19);
#undef MyReplace

    starti = form->keybm->startadd();
    form->keybm->add(KEYBMET(ui->p_1), MP.sys_JLD_state);
    form->keybm->add(KEYBMET(ui->p_2), MP.sys_LWJC_FS);
    form->keybm->add(KEYBMET(ui->p_3), MP.sys_dot_num);
    form->keybm->add(KEYBMET(ui->p_4), MP.sys_Unit);
    form->keybm->add(KEYBMET(ui->p_5), MP.sys_SXBS);
    form->keybm->add(KEYBMET(ui->p_6), MP.sys_QZPF_DS,
                     KbInputInfo(QString("5 ~ %2").arg(form->dn)),
                     KEYBM_MPIINo);
    form->keybm->add(KEYBMET(ui->p_7), MP.sys_QP_2CJL);
    form->keybm->add(KEYBMET(ui->p_22), MP.sys_QPDGSel);

    form->keybm->add(KEYBMET(ui->p_8), MP.sys_ZDSense);
    form->keybm->add(KEYBMET(ui->p_9), MP.sys_RunMode);
    form->keybm->add(KEYBMET(ui->p_10), MP.sys_AutoZeroMode);
    form->keybm->add(KEYBMET(ui->p_11), MP.sys_AutoZeroWeight);
    form->keybm->add(KEYBMET(ui->p_12), MP.sys_TrackMode);
    form->keybm->add(KEYBMET(ui->p_13), MP.sys_FL_FS);
    form->keybm->add(KEYBMET(ui->p_14), MP.sys_YC_FLFS);
    form->keybm->add(KEYBMET(ui->p_15), MP.sys_PLCJMS);
    ui->p_16->hide();

    form->keybm->add(KEYBMET(ui->p_17), MP.sys_QXResetTime);
    form->keybm->add(KEYBMET(ui->p_18), MP.sys_div);
    form->keybm->add(KEYBMET(ui->p_19), MP.sys_VibrationSen);
    form->keybm->add(KEYBMET(ui->p_20), MP.sys_VibrationCoe);
    form->keybm->add(KEYBMET(ui->p_21), MP.sys_FDZ);
    endi = form->keybm->endadd();
}

void FormMenuHSetSysParam::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
}

void FormMenuHSetSysParam::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormMenuHSetSysParam::refreshUI()
{
    form->keybm->refreshMParam(starti, endi);
#define MyReplace(I) form->refreshMPEdit(ui->p_##I, trs_sysp_##I)
    MyReplace(1);
    MyReplace(2);
    MyReplace(4);
    MyReplace(7);
    MyReplace(22);
    MyReplace(8);
    MyReplace(9);
    MyReplace(10);
    MyReplace(12);
    MyReplace(13);
    MyReplace(14);
    MyReplace(15);
    MyReplace(19);
#undef MyReplace

    int jld = MP.sys_JLD_state->vInt();
    ui->p_13->setEnabled(jld);
}

void FormMenuHSetSysParam::onRegUpdated(const MRegAreas &areas)
{
    if (areas.isIn(MMP.Sys))
        refreshUI();
}
