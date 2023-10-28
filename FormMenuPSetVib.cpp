#include "FormMenuPSetVib.h"
#include "ui_FormMenuPSetVib.h"

FormMenuPSetVib::FormMenuPSetVib(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuPSetVib)
{
    ui->setupUi(this);
}

FormMenuPSetVib::~FormMenuPSetVib()
{
    delete ui;
}

void FormMenuPSetVib::init(FormA *f)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP., form->mpri);

    starti = form->keybm->startadd();
    ushort flag = 0;
    form->keybm->add(KEYBMET(ui->zz_sj), MP.prm_DZJ_Z_RunTime);
    form->keybm->add(KEYBMET(ui->zz_zf), MP.prm_DZJ_Z_ZF);

    QLineEdit *le_xzsj[MAX_DPG];
    QLineEdit *le_xzzf[MAX_DPG];
    QLabel *lb_dn[MAX_DPG];
#define MyReplace(I) \
    le_xzsj[I-1] = ui->xz_sj_##I;\
    le_xzzf[I-1] = ui->xz_zf_##I;\
    lb_dn[I-1] = ui->dn_##I;
    MyReplace(1);
    MyReplace(2);
    MyReplace(3);
    MyReplace(4);
    MyReplace(5);
    MyReplace(6);
    MyReplace(7);
    MyReplace(8);
    MyReplace(9);
    MyReplace(10);
    MyReplace(11);
    MyReplace(12);
    MyReplace(13);
    MyReplace(14);
    MyReplace(15);
    MyReplace(16);
    MyReplace(17);
    MyReplace(18);
    MyReplace(19);
    MyReplace(20);
#undef MyReplace
    QString str_xzsj = tr("线振时间");
    QString str_xzzf = tr("线振振幅");
    QString str_xzsj_help = tr("线振时间?");
    QString str_xzzf_help = tr("线振振幅?");
    flag |= KEYBM_UnitNo;
    for (int i = 0; i < form->dng; i++) {
        form->keybm->add(str_xzsj, str_xzsj_help, le_xzsj[i], MP.prm_DZJ_X_RunTime, flag);
        form->keybm->add(QString("%1 %2").arg(str_xzzf, QString::number(i+1)),
                         str_xzzf_help, le_xzzf[i], MP.prm_DZJ_X_ZF[i+1], flag);
        le_xzsj[i]->setWhatsThis(str_xzsj_help);
        le_xzzf[i]->setWhatsThis(str_xzzf_help);
    }
    for (int i = form->dng; i < MAX_DPG; i++) {
        le_xzsj[i]->setEnabled(false);
        le_xzzf[i]->setEnabled(false);
        lb_dn[i]->setEnabled(false);
    }
    endi = form->keybm->endadd();

    connect(ui->lab_2, &QPushButton::clicked, this, &FormMenuPSetVib::onXZSJClicked);
    connect(ui->lab_3, &QPushButton::clicked, this, &FormMenuPSetVib::onXZZFClicked);
    connect(ui->lab_6, &QPushButton::clicked, this, &FormMenuPSetVib::onXZSJClicked);
    connect(ui->lab_7, &QPushButton::clicked, this, &FormMenuPSetVib::onXZZFClicked);
}

void FormMenuPSetVib::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
}

void FormMenuPSetVib::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormMenuPSetVib::refreshUI()
{
    form->keybm->refreshMParam(starti, endi);
}

void FormMenuPSetVib::onRegUpdated(const MRegAreas &areas)
{
    if (areas.isIn(MMP.Prm))
        refreshUI();
}

void FormMenuPSetVib::onXZSJClicked()
{
    Keyboard *kb = form->keybm->keybn;
    kb->reset(KbInputInfo(MP.prm_DZJ_X_RunTime->getRange()),
              tr("线振时间"), tr("线振时间?"));
    if (kb->exec() == QDialog::Accepted) {
        MAC.write(MP.prm_DZJ_X_RunTime, kb->value());
    }
}

void FormMenuPSetVib::onXZZFClicked()
{
    Keyboard *kb = form->keybm->keybn;
    kb->reset(KbInputInfo(MP.prm_DZJ_X_ZF[0]->getRange()),
            tr("线振振幅"), tr("线振振幅?"));
    if (kb->exec() == QDialog::Accepted) {
        double v = kb->value().toDouble();
        QList<const MParam *>mps;
        QVariantList vals;
        mps.reserve(MAX_DPG);
        vals.reserve(MAX_DPG);
        for (int i = 0; i < MAX_DPG; i++) {
            mps.append(MP.prm_DZJ_X_ZF[i+1]);
            vals.append(v);
        }
        MAC.write(mps, vals);
    }
}
