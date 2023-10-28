#include "FormHelpIOState.h"
#include "ui_FormHelpIOState.h"
#include "FormMenuIO1.h"
#include "FormMenuIO2.h"

FormHelpIOState::FormHelpIOState(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormHelpIOState)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);
}

FormHelpIOState::~FormHelpIOState()
{
    delete ui;
}

void FormHelpIOState::init(FormA *f)
{
    form = f;
    mptrw = new MPTRWidget(this, &form->mac, MMP.IO_Set, form->mpri);
    new MPTRWidget(this, &form->mac, MMP.IO_Status, 100);

#define MyReplace(I) \
    in_nums[I-1] = ui->io_in_n_##I;\
    in_funs[I-1] = ui->io_in_f_##I;
    MyReplace(1);
    MyReplace(2);
    MyReplace(3);
    MyReplace(4);
    MyReplace(5);
    MyReplace(6);
    MyReplace(7);
    MyReplace(8);
#undef MyReplace
#define MyReplace(I) \
    out_nums[I-1] = ui->io_out_n_##I;\
    out_funs[I-1] = ui->io_out_f_##I;
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
    MyReplace(21);
    MyReplace(22);
    MyReplace(23);
    MyReplace(24);
#undef MyReplace
}

void FormHelpIOState::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
    refreshStatus();
}

void FormHelpIOState::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormHelpIOState::refreshUI()
{
    QByteArray in_ba = MP.sys_ioin_pin->value().toByteArray();
    QStringList in_list = FormMenuIO1::getInFunStrsAll();
    for (int i = 0; i < in_n; i++) {
        int funi = (in_ba.at(i)&0x7F) % in_list.size();
        in_funs[i]->setText(in_list.at(funi));
    }
    QByteArray out_ba = MP.sys_ioout_pin->value().toByteArray();
    QStringList out_list = FormMenuIO2::getOutFunStrsAll();
    for (int i = 0; i < out_n; i++) {
        int funi = (out_ba.at(i)&0x7F) % out_list.size();
        out_funs[i]->setText(out_list.at(funi));
    }
}

void FormHelpIOState::refreshStatus()
{
    uint in_s = MP.sys_ioin_status->value().toUInt();
    for (int i = 0; i < in_n; i++) {
        in_funs[i]->setEnabled(!(in_s & (0x01 << i)));
    }
    uint out_s = MP.sys_ioout_status->value().toUInt();
    for (int i = 0; i < out_n; i++) {
        out_funs[i]->setEnabled(!(out_s & (0x01 << i)));
    }
}

void FormHelpIOState::onRegUpdated(const MRegAreas &areas)
{
    if (areas.isIn(MMP.IO_Set))
        refreshUI();
    if (areas.isIn(MMP.IO_Status))
        refreshStatus();
}
