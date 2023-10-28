#include "FormMenuCalib.h"
#include "ui_FormMenuCalib.h"
#include "Doum.h"
#include "DlgWaiting.h"
#include "Tool.h"

FormMenuCalib::FormMenuCalib(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuCalib)
{
    ui->setupUi(this);
}

FormMenuCalib::~FormMenuCalib()
{
    delete ui;
}

void FormMenuCalib::init(FormA *f)
{
    form = f;
    mptrw = new MPTRWidget(this, &form->mac, MP.doustatus_union, 200);
    new MPTRWidget(this, &form->mac, MMP.Calib, form->mpri);

    starti = form->keybm->startadd();
    form->keybm->add(KEYBMET(ui->val_1), MP.sys_nov_zzj);
    form->keybm->add(KEYBMET(ui->val_2), MP.sys_max_zzj);
    form->keybm->add(KEYBMET(ui->val_x_1), MP.sys_nov_czd);
    form->keybm->add(KEYBMET(ui->val_x_2), MP.sys_max_czd);
    form->keybm->add(KEYBMET(ui->val_zd_1), MP.sys_nov_zd);
    form->keybm->add(KEYBMET(ui->val_zd_2), MP.sys_max_zd);
    endi = form->keybm->endadd();

    QRect dr = ui->dou_widget->geometry();
    int px = dr.x()+(dr.width()-ui->dou_bg->width())/2;
    int py = dr.y()+(dr.height()-ui->dou_bg->height())/2;
    Doum *dm = form->doum;
    ui->dou_bg->setPixmap(dm->bgimg);
    ui->dou_bg->setStyleSheet("background: white;");
    dLab = dm->genDouWidget(this, px, py);
    gLab = dm->genGDouWidget(this, px, py);
    dwLab = dm->genDWeightWidget(this, px, py);
    dm->setDouMask(dLab);
    dm->setGDouMask(gLab);
    dm->setDouNumber(dLab, this);
    dm->setGDouNumber(gLab, this);
    curseldn = -1;
    selall = false;
    for (int i = 0; i < dwLab.size(); i++)
        dwLab.at(i)->setVisible(false);

    btg_zd = new QButtonGroup(this);
    btg_zd->addButton(ui->zdsen_1, 21);
    btg_zd->addButton(ui->zdsen_2, 22);
    btg_zd->addButton(ui->zdsen_3, 23);
    btg_zd->addButton(ui->zdsen_4, 24);
    QRadioButton *zdsen_hide = new QRadioButton(this);
    zdsen_hide->hide();
    btg_zd->addButton(zdsen_hide, -2);
    connect(btg_zd, SIGNAL(buttonClicked(int)), this, SLOT(onZDSenClicked(int)));

    ui->widget_zd->installEventFilter(this);
    timer_zd = new QTimer(this);
    timer_zd->setInterval(5000);
    timer_zd->setSingleShot(true);
    connect(timer_zd, SIGNAL(timeout()), this, SLOT(onZDSenLongPreesed()));
}

void FormMenuCalib::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);

    ui->widget_zd->show();
    ui->zdsen_1->setEnabled(false);
    ui->zdsen_2->setEnabled(false);
    ui->zdsen_3->setEnabled(false);
    ui->zdsen_4->setEnabled(false);

    refreshUI();
    refreshDouWeight();

    setEnabledZero(false);
}

void FormMenuCalib::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));

    form->doum->setGDouImage(dLab, gLab, curseldn, 0);
    curseldn = -1;
    selAllDou(false);
    ui->c_anew->setChecked(true);
    btg_zd->button(-2)->setChecked(true);
}

bool FormMenuCalib::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->widget_zd) {
        if (!MP.sys_VibrationSen->value().toInt())
            return false;
        if (event->type() == QEvent::MouseButtonPress)
            timer_zd->start();
        else if (event->type() == QEvent::MouseButtonRelease)
            timer_zd->stop();
        return false;
    }
    if (event->type() == QEvent::MouseButtonPress) {
        QLabel *lab = (QLabel *)watched;
        int n = Doum::getDouNumber(lab);
        if (n == curseldn) {
            form->writeCMD(CTL_SEZ, n);
            return false;
        }
        selAllDou(false);
        form->doum->setGDouImage(dLab, gLab, curseldn, 0);
        form->doum->setGDouImage(dLab, gLab, n, 7);
        curseldn = n;
        btg_zd->button(-2)->setChecked(true);
        refreshUI();
        refreshDouWeight();
    }
    return false;
}

void FormMenuCalib::refreshUI()
{
    form->keybm->refreshMParam(starti, endi);
    bool d = selall || (curseldn >= 1 && curseldn <= form->dn);
    bool g = !d && curseldn >= 0;
    bool z = btg_zd->checkedId() > 0;
    ui->val_1->setVisible(g);
    ui->val_2->setVisible(g);
    ui->val_x_1->setVisible(d);
    ui->val_x_2->setVisible(d);
    ui->val_zd_1->setVisible(z);
    ui->val_zd_2->setVisible(z);
    ui->val_nosel_1->setVisible(!g && !d && !z);
    ui->val_nosel_2->setVisible(!g && !d && !z);
}

void FormMenuCalib::refreshDouWeight()
{
    if (selall) {
        ui->weight->setText("");
        int doustatuscnt = (form->dn + 1) / 2;
        for (int i = 0; i < dwLab.size(); i++) {
            int v = MP.doustatus[i + doustatuscnt]->valuenp().toInt();
            if (v != 0x7FFE) {
                dwLab.at(i)->setEnabled(true);
                dwLab.at(i)->setText(MP.doustatus[0]->vPS(v));
            }
            else {
                dwLab.at(i)->setEnabled(false);
                dwLab.at(i)->setText("ERR");
            }
        }
        return;
    }

    if (curseldn < 0) {
        ui->weight->setText("");
        return;
    }

    int v = form->doum->getDouWeight(MP.doustatus, curseldn);
    if (v == 0x7FFE) {
        ui->weight->setEnabled(false);
        ui->weight->setText("ERR");
    }
    else {
        ui->weight->setEnabled(true);
        bool b = curseldn >= 1 && curseldn <= form->dn;
        if (b)
            ui->weight->setText(MP.doustatus[0]->vPS(v));
        else if (curseldn >= 'A')
            ui->weight->setText(MP.prm_ZZJ_ULW->vPS(v));
        else
            ui->weight->setText(MP.sys_max_zd->vPS(v));
    }
}

void FormMenuCalib::onRegUpdated(const MRegAreas &areas)
{
    if (areas.isIn(MP.doustatus_union))
        refreshDouWeight();
    else if (areas.isIn(MMP.Calib))
        refreshUI();
}

void FormMenuCalib::on_zero_clicked()
{
    int n = curseldn;
    if (ui->c_all->isChecked())
        n = 0;
    if (n < 0)
        return;

    form->writeCMD(CTL_DOU_CALIZERO, n);
    form->waitCmdFns(3000);

    setEnabledZero(true);
    if (ui->c_anew->isChecked()) {
        DlgWaiting dlg(form, 2000);
        dlg.exec();
        setEnabledZero(false);
    }
}

void FormMenuCalib::on_full_clicked()
{
    int n = curseldn;
    if (ui->c_all->isChecked())
        n = 0;
    if (n < 0)
        return;

    form->writeCMD(CTL_DOU_CALIFULL, n);
    form->waitCmdFns(3000);

    setEnabledZero(false);
}

void FormMenuCalib::on_c_all_clicked(bool checked)
{
    selAllDou(checked);
    btg_zd->button(-2)->setChecked(true);
}

void FormMenuCalib::onZDSenClicked(int i)
{
    if (curseldn == i && i >= 0) {
        form->writeCMD(CTL_SEZ, i);
        return;
    }

    selAllDou(false);
    form->doum->setGDouImage(dLab, gLab, curseldn, 0);
    curseldn = i;
    refreshUI();
    refreshDouWeight();
}

void FormMenuCalib::onZDSenLongPreesed()
{
    ui->widget_zd->hide();
    ui->zdsen_1->setEnabled(true);
    ui->zdsen_2->setEnabled(true);
    ui->zdsen_3->setEnabled(true);
    ui->zdsen_4->setEnabled(true);
    DlgInfo::info(form, tr("振动传感器，谨慎标定"));
}

void FormMenuCalib::setEnabledZero(bool b)
{
    ui->zero->setEnabled(!b);
    ui->full->setEnabled(b);
    ui->c_anew->setEnabled(!b);
}

void FormMenuCalib::selAllDou(bool b)
{
    if (selall == b)
        return;

    int m = b ? 7 : 0;
    for (int i = 0; i < form->dn; i++)
        form->doum->setGDouImage(dLab, gLab, i+1, m);
    for (int i = 0; i < form->gn; i++)
        form->doum->setGDouImage(dLab, gLab, i+'A', 0);
    for (int i = 0; i < dwLab.size(); i++)
        dwLab.at(i)->setVisible(b);

    curseldn = -1;
    selall = b;
    ui->c_all->setChecked(b);
    refreshUI();
    refreshDouWeight();
}
