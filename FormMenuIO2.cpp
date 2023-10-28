#include "FormMenuIO2.h"
#include "ui_FormMenuIO2.h"

QStringList FormMenuIO2::out_fun_strs_all;

FormMenuIO2::FormMenuIO2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuIO2)
{
    ui->setupUi(this);
}

FormMenuIO2::~FormMenuIO2()
{
    delete ui;
}

void FormMenuIO2::init(FormA *f, int i)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP.IO, form->mpri);

    index = i;
#define MyReplace(I) \
    out_funs[I-1] = ui->out_##I;\
    out_invs[I-1] = ui->out_inv_##I;\
    out_states[I-1] = ui->out_state_##I;\
    out_monis[I-1] = ui->out_moni_##I;\
    ui->lab_out_##I->setText(QString::number(I+index));
    MyReplace(1);
    MyReplace(2);
    MyReplace(3);
    MyReplace(4);
    MyReplace(5);
    MyReplace(6);
    MyReplace(7);
    MyReplace(8);
#undef MyReplace

    QButtonGroup *btg_out_fun = new QButtonGroup(this);
    QButtonGroup *btg_out_inv = new QButtonGroup(this);
    QButtonGroup *btg_out_moni = new QButtonGroup(this);
    for (int i = 0; i < ion; i++) {
        btg_out_fun->addButton(out_funs[i], i);
        btg_out_inv->addButton(out_invs[i], i);
        btg_out_moni->addButton(out_monis[i], i);
    }
    btg_out_fun->setExclusive(false);
    btg_out_inv->setExclusive(false);
    btg_out_moni->setExclusive(false);
    connect(btg_out_fun, SIGNAL(buttonClicked(int)), this, SLOT(onOutFunClicked(int)));
    connect(btg_out_inv, SIGNAL(buttonClicked(int)), this, SLOT(onOutInvClicked(int)));
    connect(btg_out_moni, SIGNAL(buttonPressed(int)), this, SLOT(onOutMoniPressed(int)));
    connect(btg_out_moni, SIGNAL(buttonReleased(int)), this, SLOT(onOutMoniReleased(int)));

    if (out_fun_strs_all.isEmpty()) {
        out_fun_strs_all.append(tr("无"));
        out_fun_strs_all.append(tr("组合称就绪 1A"));
        out_fun_strs_all.append(tr("放料 1B"));
        out_fun_strs_all.append(tr("超重/清空 1C"));
        out_fun_strs_all.append(tr("料位加料 1D"));

        out_fun_strs_all.append(tr("组合称就绪 2A"));
        out_fun_strs_all.append(tr("放料 2B"));
        out_fun_strs_all.append(tr("超重 2C"));
        out_fun_strs_all.append(tr("料位加料 2D"));

        out_fun_strs_all.append(tr("组合称就绪 3A"));
        out_fun_strs_all.append(tr("放料 3B"));
        out_fun_strs_all.append(tr("超重 3C"));
        out_fun_strs_all.append(tr("料位加料 3D"));

        out_fun_strs_all.append(tr("组合称就绪 4A"));
        out_fun_strs_all.append(tr("放料 4B"));
        out_fun_strs_all.append(tr("超重 4C"));
        out_fun_strs_all.append(tr("料位加料 4D"));

        out_fun_strs_all.append(tr("组合称就绪 5A"));
        out_fun_strs_all.append(tr("放料 5B"));
        out_fun_strs_all.append(tr("超重 5C"));
        out_fun_strs_all.append(tr("料位加料 5D"));

        out_fun_strs_all.append(tr("正在运行"));
        out_fun_strs_all.append(tr("有报警"));

        for (int i = out_fun_strs_all.size(); i < 30; i++)
            out_fun_strs_all.append("");
        for (int i = 0; i < form->dn; i++)
            out_fun_strs_all.append(tr("振机%1信号").arg(i+1));
    }

    const int outn = 4;
    out_fun_strs.reserve(outn+1);
    out_fun_vals.reserve(outn+1);
    out_fun_strs.append(out_fun_strs_all.at(0));
    out_fun_vals.append(0);
    for (int i = 0; i < outn; i++) {
        out_fun_strs.append(out_fun_strs_all.at(i+1));
        out_fun_vals.append(i+1);
    }
    // 额外增加
    for (int i = 0; i < outn-1; i++) {
        out_fun_strs.append(out_fun_strs_all.at(outn+i+1));
        out_fun_vals.append(outn+i+1);
    }

    out_fun_strs.append(out_fun_strs_all.at(21));
    out_fun_vals.append(21);
    out_fun_strs.append(out_fun_strs_all.at(22));
    out_fun_vals.append(22);

    for (int i = 0; i < form->dn; i++) {
        out_fun_strs.append(out_fun_strs_all.at(30+i));
        out_fun_vals.append(30+i);
    }
}

void FormMenuIO2::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
}

void FormMenuIO2::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormMenuIO2::refreshUI()
{
    QByteArray ba = MP.sys_ioout_pin->value().toByteArray();
    for (int i = 0; i < ion; i++) {
        int funi = (ba.at(i+index)&0x7F) % out_fun_strs_all.size();
        out_funs[i]->setText(out_fun_strs_all.at(funi));
        out_invs[i]->setChecked(ba.at(i+index)&0x80);
        out_monis[i]->setEnabled(funi);
    }
}

void FormMenuIO2::refreshStatus()
{
    uint status = MP.sys_ioout_status->value().toUInt();
    for (int i = 0; i < ion; i++) {
        out_states[i]->setChecked(status & (0x01 << (i+index)));
    }
}

void FormMenuIO2::onRegUpdated(const MRegAreas &areas)
{
    if (areas.isIn(MMP.IO_Set))
        refreshUI();
    if (areas.isIn(MMP.IO_Status))
        refreshStatus();
}

void FormMenuIO2::onOutFunClicked(int i)
{
    DlgList dlg(form);
    dlg.setWindowTitle(QString("%1 %2").arg(ui->label_out_2->text(), QString::number(i+index+1)));
    dlg.setHelpText(ui->label_out_2->whatsThis());
    dlg.setList(out_fun_strs, 2);
    dlg.setVList(out_fun_vals);
    dlg.resize(600, dlg.height());
    if (dlg.exec() == QDialog::Accepted) {
        QByteArray v = MP.sys_ioout_pin->value().toByteArray();
        v[i+index] = (v.at(i+index)&0x80) | (dlg.currentv().toInt()&0x7F);
        MAC.write(MP.sys_ioout_pin, v);
    }
}

void FormMenuIO2::onOutInvClicked(int i)
{
    QByteArray v = MP.sys_ioout_pin->value().toByteArray();
    v[i+index] = v.at(i+index)^0x80;
    MAC.write(MP.sys_ioout_pin, v);
}

void FormMenuIO2::onOutMoniPressed(int i)
{
    form->writeCMD(0x00,CTL_TEST_IOOUT,1,i+index+1);
}

void FormMenuIO2::onOutMoniReleased(int i)
{
    form->writeCMD(0x00,CTL_TEST_IOOUT,0,i+index+1);
}
