#include "FormMainComb.h"
#include "ui_FormMainComb.h"
#include "Doum.h"
#include "RecSave.h"
#include "Tool.h"

FormMainComb::FormMainComb(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMainComb)
{
    ui->setupUi(this);

    inTWeight = INT_MAX;
}

FormMainComb::~FormMainComb()
{
    delete ui;
}

void FormMainComb::init(FormA *f)
{
    form = f;
    mptrw = new MPTRWidget(this, &form->mac, MP.doustatus_union, 100);

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
    dstate.resize(dLab.size());

    int i;
    i = 0;
    vals[0][i++] = ui->val_1_1;
    vals[0][i++] = ui->val_1_2;
    vals[0][i++] = ui->val_1_3;
    vals[0][i++] = ui->val_1_4;
    vals[0][i++] = ui->val_1_5;
    vals[0][i++] = ui->val_1_6;
    vals[0][i++] = ui->val_1_7;
    vals[0][i++] = ui->val_1_8;
    vals[0][i++] = ui->val_1_9;
    vals[0][i++] = ui->weight_1;
    vals[0][i++] = ui->unit_1;
    vals[0][i++] = ui->afc_1;
    vals[0][i++] = ui->over_1;

    connect(form->recs, SIGNAL(inTWeightChanged(int)),
            this, SLOT(onInTWeightChanged(int)));
}

void FormMainComb::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
    refreshDouWeight();

    form->writeCMD(CTL_INTO_PAGE, 1);
}

void FormMainComb::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormMainComb::refreshUI()
{
    int *ds = dstate.data();
    for (int i = 0; i < dLab.size(); i++) {
        int g = i / form->dng;
        int n = i % form->dng;
        QByteArray ba = MP.cmb_DouStatus[g]->value().toByteArray();
        int s = form->doum->getDouStatus(ba, n);
        dLab.at(i)->setPixmap(form->doum->dimg.at(i).at(s));
        ds[i] = s;
    }

    QMutexLocker locker(&form->recs->pf_mutex);
    int dot_sys = MP.sys_dot_num->value().toInt() % 4;
    for (int g = 0; g < form->gn && g < 1; g++) {
        int id = MP.sys_prm_ids[g]->value().toInt();
        if (id <= 0 || id >= 100) {
            id = MP.prm_id->value().toInt() & 0xFF;
            if (id <= 0 || id >= 100)
                id = 0;
        }
        int i = 0;
        vals[g][i++]->setText(QString::number(id));
        vals[g][i++]->setText(form->recs->pf[id].name);
        vals[g][i++]->setText(MP.cmb_DWW[g]->vStr());

        int dwwc = MP.cmb_DWW[g]->vIntNp();
        int dww = form->recs->pf[id].dww;
        int dwwup = form->recs->pf[id].dwwup;
        int dwwdn = form->recs->pf[id].dwwdn;
        vals[g][i-1]->setEnabled(dwwc <= dww+dwwup && dwwc >= dww-dwwdn);

        int dot = dot_sys;
        int worP = form->recs->pf[id].worP;
        if (worP ==1 || worP == 2)
            dot = 0;

        if (inTWeight != INT_MAX && form->isRun(g)) {
            vals[g][i]->setEnabled(false);
            vals[g][i++]->setText(Tool::getDotStr(inTWeight, dot));
        }
        else {
            inTWeight = INT_MAX;
            vals[g][i]->setEnabled(true);
            vals[g][i++]->setText(Tool::getDotStr(form->recs->pf[id].setw[0], dot));
        }
        vals[g][i++]->setText(Tool::getDotStr(form->recs->pf[id].setw[1], dot));
        vals[g][i++]->setText(Tool::getDotStr(form->recs->pf[id].setw[2], dot));

        int cbtype = MP.cmb_cbtype[g]->value().toUInt();
        vals[g][i++]->setText(QString::number((cbtype >> 8)/10.0, 'f', 1));
        vals[g][i++]->setText(QString::number(form->recs->pf[id].speed));
        vals[g][i++]->setText(MP.cmb_speed[g]->vStr());

        int weight = MP.cmb_Weight[g]->valuenp().toInt();
        vals[g][i++]->setText(Tool::getDotStr(weight, dot));

        if (worP ==1 || worP == 2)
            vals[g][i++]->setText("P");
        else
            vals[g][i++]->setText(MP.doustatus[0]->getUStr());

        vals[g][i++]->setText(Tool::getAFCStr(form->recs->pf[id].afc));

        vals[g][i++]->setVisible(MP.cmb_RunStatus[g]->value().toInt() & 0x0010);
    }
}

void FormMainComb::refreshDouWeight()
{
    bool baoshu = MP.prm_WorP->vInt() == 2 && form->isRun();
    const int *ds = dstate.constData();
    int doustatuscnt = (form->dn + 1) / 2;
    for (int i = 0; i < dwLab.size(); i++) {
        int v = MP.doustatus[i + doustatuscnt]->valuenp().toInt();
        if (v != 0x7FFE) {
            QString str = baoshu ? QString::number(v) : MP.doustatus[0]->vPS(v);
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

void FormMainComb::onRegUpdated(const MRegAreas &areas)
{
    if (areas.isIn(MP.runstatus_union))
        refreshUI();
    else if (areas.isIn(MP.doustatus_union))
        refreshDouWeight();
}

void FormMainComb::onInTWeightChanged(int w)
{
    inTWeight = w;
    if (isVisible())
        refreshUI();
}
