#include "FormMenuPSetOther.h"
#include "ui_FormMenuPSetOther.h"

QStringList trs_afc;

FormMenuPSetOther::FormMenuPSetOther(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuPSetOther)
{
    if (trs_afc.isEmpty())
        trs_afc << tr("关闭") << tr("AFCT")
                    << tr("AFCI") << tr("AFCR");

    ui->setupUi(this);
}

FormMenuPSetOther::~FormMenuPSetOther()
{
    delete ui;
}

void FormMenuPSetOther::init(FormA *f)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP., form->mpri);

#define MyReplace(I) form->connectMPEdit(ui->val_##I, trs_##I)
    MyReplace(afc);
#undef MyReplace

    starti = form->keybm->startadd();
    form->keybm->add(KEYBMET(ui->val_afc), MP.prm_AFCMode);
    form->keybm->add(KEYBMET(ui->val_afct_1), MP.prm_AFCT_TDS);
    form->keybm->add(KEYBMET(ui->val_afct_2), MP.prm_AFCT_LDS);
    form->keybm->add(KEYBMET(ui->val_afct_3), MP.prm_AFCT_Cnt);
    form->keybm->add(KEYBMET(ui->val_afci_1), MP.prm_AFCI_DDW);
    form->keybm->add(KEYBMET(ui->val_afci_2), MP.prm_AFCI_LW);
    form->keybm->add(KEYBMET(ui->val_afci_3), MP.prm_AFCI_Cnt);
    form->keybm->add(KEYBMET(ui->val_afcr_1), MP.prm_AFCR_Time);
    endi = form->keybm->endadd();
}

void FormMenuPSetOther::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
}

void FormMenuPSetOther::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormMenuPSetOther::refreshUI()
{
    form->keybm->refreshMParam(starti, endi);
#define MyReplace(I) form->refreshMPEdit(ui->val_##I, trs_##I)
    MyReplace(afc);
#undef MyReplace
}

void FormMenuPSetOther::onRegUpdated(const MRegAreas &areas)
{
    if (areas.isIn(MMP.Prm))
        refreshUI();
}
