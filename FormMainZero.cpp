#include "FormMainZero.h"
#include "ui_FormMainZero.h"
#include "Doum.h"
#include "Keyboard.h"
#include "DlgList.h"

FormMainZero::FormMainZero(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMainZero)
{
    ui->setupUi(this);
}

FormMainZero::~FormMainZero()
{
    delete ui;
}

void FormMainZero::init(FormA *f)
{
    form = f;
    mptrw = new MPTRWidget(this, &form->mac, MP.doustatus_union, 100);
    MRegRWInfo mri;
    int doustatuscnt = (form->dn + 1) / 2;
    mri.span(MP.doustatus[0], MP.doustatus[doustatuscnt-1]);
    mptr = new MPTReader(this, &form->mac, mri, 100);

    QRect dr = ui->dou_widget->geometry();
    int px = dr.x()+(dr.width()-ui->dou_bg->width())/2;
    int py = dr.y()+(dr.height()-ui->dou_bg->height())/2;
    Doum *dm = form->doum;
    ui->dou_bg->setPixmap(dm->bgimg);
    ui->dou_bg->setStyleSheet("background: white;");
    dLab = dm->genDouWidget(this, px, py);
    gLab = dm->genGDouWidget(this, px, py);
    dwLab = dm->genDWeightWidget(this, px, py);
    gwLab = dm->genGWeightWidget(this, px, py);
}

void FormMainZero::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    mptr->stop();
    refreshUI();
    refreshBtnStatus();

    form->writeCMD(CTL_INTO_PAGE, 0);
}

void FormMainZero::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
    mptr->stop();
}

void FormMainZero::refreshUI()
{
    int ds[dLab.size()];
    for (int i = 0; i < dLab.size(); i++) {
        int s = form->doum->getDouStatus(MP.doustatus, i);
        dLab.at(i)->setPixmap(form->doum->dimg.at(i).at(s));
        ds[i] = s;
    }

    int doustatuscnt = (form->dn + 1) / 2;
    for (int i = 0; i < dwLab.size(); i++) {
        int v = MP.doustatus[i + doustatuscnt]->valuenp().toInt();
        if (v != 0x7FFE) {
            QString str = MP.doustatus[0]->vPS(v);
            if (form->doum->dstate.at(ds[i] + 48))
                str.append(':').append(ds[i]+(ds[i]<10?'0':'A'-10));

            dwLab.at(i)->setEnabled(true);
            dwLab.at(i)->setText(str);
        }
        else {
            dwLab.at(i)->setEnabled(false);
            dwLab.at(i)->setText("ERR");
        }
    }
    bool zz = MP.sys_LWJC_FS->value().toInt() != 0;
    for (int i = 0; i < gwLab.size(); i++) {
        int v = MP.doustatus[i + doustatuscnt + form->dn]->valuenp().toInt();
        if (v != 0x7FFE) {
            gwLab.at(i)->setEnabled(true);
            gwLab.at(i)->setText(MP.prm_ZZJ_ULW->vPS(v));
        }
        else {
            gwLab.at(i)->setEnabled(false);
            gwLab.at(i)->setText("ERR");
        }
        gwLab.at(i)->setVisible(zz);
    }
}

void FormMainZero::refreshBtnStatus()
{
    bool bz = form->isManualZero();
    bool be = form->isManualEmpty();
    bool bc = form->isManualClean();
    bool b = bz || be || bc;

    ui->pushButton->setEnabled(!b);
    ui->pushButton_2->setEnabled(!b || be);
    ui->pushButton_2->setChecked(be);
    ui->pushButton_3->setEnabled(!b || bc);
    ui->pushButton_3->setChecked(bc);

    if (b) {
        mptrw->reader.stop();
        mptr->start();
    }
    else if (!b && isVisible()) {
        mptrw->reader.start();
        mptr->stop();
    }
}

void FormMainZero::onRegUpdated(const MRegAreas &areas)
{
    if (areas.isIn(MP.doustatus_union))
        refreshUI();
    if (areas.isIn(MP.runstatus_union))
        refreshBtnStatus();
}

void FormMainZero::on_pushButton_clicked()
{
    form->writeCMD(CTL_MANUAL_ZERO, 0x01);
    form->mac.read(MP.runstatus_union);
    mptrw->reader.stop();
    mptr->start();
}

void FormMainZero::on_pushButton_2_clicked()
{
    form->writeCMD(CTL_MANUAL_EMPTY, 0x01);
    form->mac.read(MP.runstatus_union);
    mptrw->reader.stop();
    mptr->start();
}

void FormMainZero::on_pushButton_3_clicked()
{
    form->writeCMD(CTL_MANUAL_CLEAN, 0x01);
    form->mac.read(MP.runstatus_union);
    mptrw->reader.stop();
    mptr->start();
    //QTimer::singleShot(200, this, SLOT(onClean2()));
    //注意200毫秒内不能再点击
}

void FormMainZero::onClean2()
{
    form->writeCMD(CTL_MANUAL_CLEAN, 0x10);
    form->mac.read(MP.runstatus_union);
    mptrw->reader.stop();
    mptr->start();
}
