#include "FormMenuPSetSpecify.h"
#include "ui_FormMenuPSetSpecify.h"
#include "DialogSetSpecify.h"

FormMenuPSetSpecify::FormMenuPSetSpecify(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuPSetSpecify)
{
    ui->setupUi(this);
}

FormMenuPSetSpecify::~FormMenuPSetSpecify()
{
    delete ui;
}

void FormMenuPSetSpecify::init(FormA *f)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP., form->mpri);

    starti = form->keybm->startadd();
#define MyReplace(I) \
    zdd[I-1] = ui->zdd_##I; mps_zdd[I-1] = MP.prm_ZDD_Dou[I-1];\
    ddzh[I-1] = ui->ddzh_##I;\
    cqqp[I-1] = ui->cqqp_##I;\
    form->keybm->add(QString("%1 %2").arg(ui->lab_2->text(), QString::number(I)),\
                     ui->lab_2->whatsThis(), ui->zlsx_##I, MP.prm_ZDD_WeightUp[I-1]);\
    ui->zlsx_##I->setWhatsThis(ui->lab_2->whatsThis());\
    form->keybm->add(QString("%1 %2").arg(ui->lab_3->text(), QString::number(I)),\
                     ui->lab_3->whatsThis(), ui->zlxx_##I, MP.prm_ZDD_WeightDn[I-1]);\
    ui->zlxx_##I->setWhatsThis(ui->lab_3->whatsThis());
    MyReplace(1);
    MyReplace(2);
    MyReplace(3);
    MyReplace(4);
    MyReplace(5);
    MyReplace(6);
    MyReplace(7);
#undef MyReplace
    endi = form->keybm->endadd();

    QButtonGroup *btg_zdd = new QButtonGroup(this);
    QButtonGroup *btg_ddzh = new QButtonGroup(this);
    QButtonGroup *btg_cqqp = new QButtonGroup(this);
    for (int i = 0; i < zddn; i++) {
        old_zdd[i] = 0;
        zdd[i]->setText("0");
        btg_zdd->addButton(zdd[i], i);
        btg_ddzh->addButton(ddzh[i], i);
        btg_cqqp->addButton(cqqp[i], i);
    }
    btg_zdd->setExclusive(false);
    btg_ddzh->setExclusive(false);
    btg_cqqp->setExclusive(false);
    connect(btg_zdd, SIGNAL(buttonClicked(int)), this, SLOT(onZDDClicked(int)));
    connect(btg_ddzh, SIGNAL(buttonClicked(int)), this, SLOT(onDDZHClicked(int)));
    connect(btg_cqqp, SIGNAL(buttonClicked(int)), this, SLOT(onCQQPClicked(int)));
}

void FormMenuPSetSpecify::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
}

void FormMenuPSetSpecify::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

#include <QTextLayout>
void FormMenuPSetSpecify::refreshUI()
{
    form->keybm->refreshMParam(starti, endi);

    quint32 u_ddzh = MP.prm_ZDD_MultSel->value().toUInt();
    quint32 u_cqqp = MP.prm_ZDD_LowerQPSel->value().toUInt();
    for (int i = 0; i < zddn; i++) {
        quint32 u_zdd = mps_zdd[i]->value().toUInt();
        if (u_zdd != old_zdd[i]) {
            QString str;
            if (u_zdd == 0)
                str = "0";
            else {
                int zdd_w = zdd[i]->width();
                QFontMetricsF fm(zdd[i]->font());
                qreal fmw = 0;
                qreal fmw_dot = fm.horizontalAdvance(',');
                qreal zdd_mw = fm.horizontalAdvance('0');
                for (int i = 0; i < MAX_D; i++)
                    if (u_zdd & (0x01 << i)) {
                        QString str_num = QString::number(i+1);
                        qreal fmw_num = fm.horizontalAdvance(str_num) + fmw_dot;
                        fmw += fmw_num;
                        if (fmw >= zdd_w - zdd_mw*2) {
                            fmw = fmw_num;
                            str.append('\n');
                        }
                        str.append(str_num).append(',');
                    }
                str.chop(1);
            }
            old_zdd[i] = u_zdd;
            zdd[i]->setText(str);
        }
        ddzh[i]->setChecked(u_ddzh & (0x01 << i));
        cqqp[i]->setChecked(u_cqqp & (0x01 << i));
    }
}

void FormMenuPSetSpecify::onRegUpdated(const MRegAreas &areas)
{
    if (areas.isIn(MMP.Prm))
        refreshUI();
}

void FormMenuPSetSpecify::onZDDClicked(int i)
{
    quint32 sel, dis = 0;
    sel = mps_zdd[i]->value().toUInt();
    for (int j = 0; j < zddn; j++)
        if (j != i)
            dis |= mps_zdd[j]->value().toUInt();

    DialogSetSpecify dlg(form->dn, form);
    dlg.setWindowTitle(QString("%1 %2").arg(ui->lab_1->text(), QString::number(i+1)));
    dlg.setHelpText(ui->lab_1->whatsThis());
    dlg.setSel(sel, dis);
    if (dlg.exec() == QDialog::Accepted)
        MAC.write(mps_zdd[i], dlg.getSel());
}

void FormMenuPSetSpecify::onDDZHClicked(int i)
{
    quint32 u_ddzh = MP.prm_ZDD_MultSel->value().toUInt();
    quint32 flag = 0x01 << i;
    u_ddzh ^= flag;
    MAC.write(MP.prm_ZDD_MultSel, u_ddzh);
}

void FormMenuPSetSpecify::onCQQPClicked(int i)
{
    quint32 u_cqqp = MP.prm_ZDD_LowerQPSel->value().toUInt();
    quint32 flag = 0x01 << i;
    u_cqqp ^= flag;
    MAC.write(MP.prm_ZDD_LowerQPSel, u_cqqp);
}
