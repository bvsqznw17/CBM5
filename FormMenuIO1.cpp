#include "FormMenuIO1.h"
#include "ui_FormMenuIO1.h"

QStringList FormMenuIO1::in_fun_strs_all;

FormMenuIO1::FormMenuIO1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuIO1)
{
    ui->setupUi(this);
}

FormMenuIO1::~FormMenuIO1()
{
    delete ui;
}

void FormMenuIO1::init(FormA *f, int i)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP.IO, form->mpri);

    index = i;
#define MyReplace(I) \
    in_funs[I-1] = ui->in_##I;\
    in_invs[I-1] = ui->in_inv_##I;\
    in_states[I-1] = ui->in_state_##I;\
    ui->lab_in_##I->setText(QString::number(I+index));
    MyReplace(1);
    MyReplace(2);
    MyReplace(3);
    MyReplace(4);
    MyReplace(5);
    MyReplace(6);
    MyReplace(7);
    MyReplace(8);
#undef MyReplace

    QButtonGroup *btg_in_fun = new QButtonGroup(this);
    QButtonGroup *btg_in_inv = new QButtonGroup(this);
    for (int i = 0; i < ion; i++) {
        btg_in_fun->addButton(in_funs[i], i);
        btg_in_inv->addButton(in_invs[i], i);
    }
    btg_in_fun->setExclusive(false);
    btg_in_inv->setExclusive(false);
    connect(btg_in_fun, SIGNAL(buttonClicked(int)), this, SLOT(onInFunClicked(int)));
    connect(btg_in_inv, SIGNAL(buttonClicked(int)), this, SLOT(onInInvClicked(int)));

    if (in_fun_strs_all.isEmpty()) {
        in_fun_strs_all.append(tr("无"));
        in_fun_strs_all.append(tr("包装机A请求放料信号输入"));
        in_fun_strs_all.append(tr("包装机B请求放料信号输入"));
        in_fun_strs_all.append(tr("包装机C请求放料信号输入"));
        in_fun_strs_all.append(tr("包装机D请求放料信号输入"));
        in_fun_strs_all.append(tr("料位检测信号输入 A"));
        in_fun_strs_all.append(tr("料位检测信号输入 B"));
        in_fun_strs_all.append(tr("料位检测信号输入 C"));
        in_fun_strs_all.append(tr("料位检测信号输入 D"));
        in_fun_strs_all.append(tr(""));
        in_fun_strs_all.append(tr("运行"));
    }

    const int inn = 2;
    in_fun_strs.reserve(inn+1);
    in_fun_vals.reserve(inn+1);
    in_fun_strs.append(in_fun_strs_all.at(0));
    in_fun_vals.append(0);
    for (int g = 0; g < form->gn; g++)
        for (int i = 0; i < inn; i++) {
            in_fun_strs.append(in_fun_strs_all.at(i*4+g+1));
            in_fun_vals.append(i*4+g+1);
        }

    // 额外增加
    in_fun_strs.append(in_fun_strs_all.at(2));
    in_fun_vals.append(2);

    in_fun_strs.append(in_fun_strs_all.at(10));
    in_fun_vals.append(10);
}

void FormMenuIO1::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
    refreshStatus();
}

void FormMenuIO1::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormMenuIO1::refreshUI()
{
    QByteArray ba = MP.sys_ioin_pin->value().toByteArray();
    for (int i = 0; i < ion; i++) {
        int funi = (ba.at(i+index)&0x7F) % in_fun_strs_all.size();
        in_funs[i]->setText(in_fun_strs_all.at(funi));
        in_invs[i]->setChecked(ba.at(i+index)&0x80);
    }
}

void FormMenuIO1::refreshStatus()
{
    uint status = MP.sys_ioin_status->value().toUInt();
    for (int i = 0; i < ion; i++) {
        in_states[i]->setChecked(status & (0x01 << i));
    }
}

void FormMenuIO1::onRegUpdated(const MRegAreas &areas)
{
    if (areas.isIn(MMP.IO_Set))
        refreshUI();
    if (areas.isIn(MMP.IO_Status))
        refreshStatus();
}

void FormMenuIO1::onInFunClicked(int i)
{
    DlgList dlg(form);
    dlg.setWindowTitle(QString("%1 %2").arg(ui->label_in_2->text(), QString::number(i+index+1)));
    dlg.setHelpText(ui->label_in_2->whatsThis());
    dlg.setList(in_fun_strs);
    dlg.setVList(in_fun_vals);
    if (dlg.exec() == QDialog::Accepted) {
        QByteArray v = MP.sys_ioin_pin->value().toByteArray();
        v[i+index] = (v.at(i+index)&0x80) | (dlg.currentv().toInt()&0x7F);
        MAC.write(MP.sys_ioin_pin, v);
    }
}

void FormMenuIO1::onInInvClicked(int i)
{
    QByteArray v = MP.sys_ioin_pin->value().toByteArray();
    v[i+index] = v.at(i+index)^0x80;
    MAC.write(MP.sys_ioin_pin, v);
}
