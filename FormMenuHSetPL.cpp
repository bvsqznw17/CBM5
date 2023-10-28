#include "FormMenuHSetPL.h"
#include "ui_FormMenuHSetPL.h"
#include <QButtonGroup>

FormMenuHSetPL::FormMenuHSetPL(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuHSetPL)
{
    ui->setupUi(this);
}

FormMenuHSetPL::~FormMenuHSetPL()
{
    delete ui;
}

void FormMenuHSetPL::init(FormA *f)
{
    form = f;
    mptrw = new MPTRWidget(this, &form->mac, MMP.PLZY, form->mpri);

    starti = form->keybm->startadd();
    QLineEdit *le_xzpl[MAX_D];
    QLineEdit *le_xzzy[MAX_D];
#define MyReplace(I) \
    le_xzpl[I-1] = ui->xz_pl_##I;\
    le_xzzy[I-1] = ui->xz_zy_##I;\
    dou[I-1] = ui->dn_##I;
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
    QString str_xzpl = tr("线振频率");
    QString str_xzzy = tr("线振增益");
    QString str_xzpl_help = tr("线振频率?");
    QString str_xzzy_help = tr("线振增益?");
    for (int i = 0; i < form->dn; i++) {
        form->keybm->add(QString("%1 %2").arg(str_xzpl, QString::number(i+1)),
                         str_xzpl_help, le_xzpl[i], MP.sys_DZJ_FreqSet, KEYBM_MPIIOnly);
        form->keybm->add(QString("%1 %2").arg(str_xzzy, QString::number(i+1)),
                         str_xzzy_help, le_xzzy[i], MP.sys_DZJ_GainSet, KEYBM_MPIIOnly);
        le_xzpl[i]->setWhatsThis(str_xzpl_help);
        le_xzzy[i]->setWhatsThis(str_xzzy_help);
    }
    for (int i = form->dn; i < MAX_D; i++) {
        le_xzpl[i]->setEnabled(false);
        le_xzzy[i]->setEnabled(false);
        dou[i]->setEnabled(false);
    }
    form->keybm->add(KEYBMET(ui->zz_pl), MP.sys_DZJ_FreqSet, KEYBM_MPIIOnly);
    form->keybm->add(KEYBMET(ui->zz_zy), MP.sys_DZJ_GainSet, KEYBM_MPIIOnly);
    endi = form->keybm->endadd();

    for (int i = starti; i < endi; i++)
        connect(form->keybm->edit(i), SIGNAL(editingFinished()),
                this, SLOT(onEditFinished()));

    connect(ui->lab_2, &QPushButton::clicked, this, &FormMenuHSetPL::onXZPLClicked);
    connect(ui->lab_3, &QPushButton::clicked, this, &FormMenuHSetPL::onXZZYClicked);
    connect(ui->lab_5, &QPushButton::clicked, this, &FormMenuHSetPL::onXZPLClicked);
    connect(ui->lab_6, &QPushButton::clicked, this, &FormMenuHSetPL::onXZZYClicked);

//    connect(ui->lab_1, &QPushButton::clicked, this, &FormMenuHSetPL::onDouSelAll);
//    connect(ui->lab_4, &QPushButton::clicked, this, &FormMenuHSetPL::onDouSelAll);
    ui->lab_1->setEnabled(false);
    ui->lab_4->setEnabled(false);
    btg = new QButtonGroup(this);
    for (int i = 0; i < form->dn; i++)
        btg->addButton(dou[i], i);
}

void FormMenuHSetPL::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
    if (btg->checkedButton())
        btg->checkedButton()->setChecked(false);
}

void FormMenuHSetPL::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormMenuHSetPL::refreshUI()
{
    QByteArray v1 = MP.sys_DZJ_FreqSet->value().toByteArray();
    QByteArray v2 = MP.sys_DZJ_GainSet->value().toByteArray();
    int i = starti, j = 0;
    for (; i < endi-2; j++) {
        form->keybm->setText(i++, QString::number(v1.at(j)));
        form->keybm->setText(i++, QString::number(v2.at(j)));
    }
    form->keybm->setText(i++, QString::number(v1.at(j)).append(
                             MP.sys_DZJ_FreqSet->getUStr_()));
    form->keybm->setText(i++, QString::number(v2.at(j)).append(
                             MP.sys_DZJ_GainSet->getUStr_()));
}

void FormMenuHSetPL::onRegUpdated(const MRegAreas &areas)
{
    if (areas.isIn(MMP.PLZY))
        QTimer::singleShot(0, this, SLOT(refreshUI()));
}

void FormMenuHSetPL::onEditFinished()
{
    QByteArray v;
    int i = form->keybm->indexOf(sender());
    int j = i - starti;
    if (j < 0 || form->keybm->isError(i))
        return;

    if (j % 2 == 0) {
        v = MP.sys_DZJ_FreqSet->value().toByteArray();
        v[j/2] = form->keybm->edit(i)->text().toInt();
        MAC.write(MP.sys_DZJ_FreqSet, v, form->keybm->getEcho(i));
    }
    else {
        v = MP.sys_DZJ_GainSet->value().toByteArray();
        v[j/2] = form->keybm->edit(i)->text().toInt();
        MAC.write(MP.sys_DZJ_GainSet, v, form->keybm->getEcho(i));
    }
}

void FormMenuHSetPL::onXZPLClicked()
{
    Keyboard *kb = form->keybm->keybn;
    kb->reset(KbInputInfo(MP.sys_DZJ_FreqSet->getRange()),
              tr("线振频率"), tr("线振频率?"));
    if (kb->exec() == QDialog::Accepted) {
        int v = qRound(kb->value().toDouble());
        QByteArray ba = MP.sys_DZJ_FreqSet->value().toByteArray();
        char *d = ba.data();
        for (int i = 0; i < ba.size() && i < form->dn; i++)
            d[i] = v;
        MAC.write(MP.sys_DZJ_FreqSet, ba);
    }
}

void FormMenuHSetPL::onXZZYClicked()
{
    Keyboard *kb = form->keybm->keybn;
    kb->reset(KbInputInfo(MP.sys_DZJ_GainSet->getRange()),
              tr("线振增益"), tr("线振增益?"));
    if (kb->exec() == QDialog::Accepted) {
        int v = qRound(kb->value().toDouble());
        QByteArray ba = MP.sys_DZJ_GainSet->value().toByteArray();
        char *d = ba.data();
        for (int i = 0; i < ba.size() && i < form->dn; i++)
            d[i] = v;
        MAC.write(MP.sys_DZJ_GainSet, ba);
    }
}

void FormMenuHSetPL::onDouSelAll()
{
    bool b = false;
    for (int i = 0; i < form->dn; i++)
        if (!dou[i]->isChecked()) {
            b = true;
            break;
        }
    for (int i = 0; i < form->dn; i++)
        dou[i]->setChecked(b);
}

void FormMenuHSetPL::on_btn_test_z_clicked()
{
    form->writeCMD(CTL_MANUAL_ZZJ, 'A');
}

void FormMenuHSetPL::on_btn_test_x_clicked()
{
    if (btg->checkedButton())
        form->writeCMD(CTL_MANUAL_XZJ, btg->checkedId() + 1);
}
