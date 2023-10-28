#include "FormSamping.h"
#include "ui_FormSamping.h"

FormSamping::FormSamping(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormSamping)
{
    ui->setupUi(this);
    dounum = 1;
}

FormSamping::~FormSamping()
{
    delete ui;
    form->mac.echom.destroy(ecIds, this);
}

void FormSamping::init(FormA *f)
{
    form = f;
    mptrw = new MPTRWidget(this, &form->mac, MMP.Prm, form->mpri);
    new MPTRWidget(this, &form->mac, MP.doustatus_union, 200);
    form->mac.echom.create(echo, ecIds, this, "onReadSampDataFns");

    connect(ui->btn_back, SIGNAL(clicked()), form, SLOT(backPage()));

    starti = form->keybm->startadd();
    form->keybm->add(KEYBMET(ui->val_1), MP.prm_speed);
    form->keybm->add(KEYBMET(ui->val_2), MP.prm_ASF);
    form->keybm->add(KEYBMET(ui->val_3), MP.prm_CZD_Dly);
    form->keybm->add(KEYBMET(ui->val_4), MP.prm_MSV_Dly);
    endi = form->keybm->endadd();
}

void FormSamping::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
}

void FormSamping::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormSamping::refreshUI()
{
    form->keybm->refreshMParam(starti, endi);
    ui->dounum->setText(QString::number(dounum));
}

void FormSamping::refreshDouWeight()
{
    int doustatuscnt = (form->dn + 1) / 2;
    int v = MP.doustatus[dounum - 1 + doustatuscnt]->valuenp().toInt();
    if (v != 0x7FFE) {
        ui->weight->setEnabled(true);
        ui->weight->setText(MP.doustatus[0]->vPS(v));
    }
    else {
        ui->weight->setEnabled(false);
        ui->weight->setText("ERR");
    }
}

void FormSamping::onRegUpdated(const MRegAreas &areas)
{
    if (areas.isIn(MMP.Prm))
        refreshUI();
    if (areas.isIn(MP.doustatus_union))
        refreshDouWeight();
}

void FormSamping::onReadSampDataFns(int err, const SignalEcho_zy1 &echo)
{
    form->mac.echom.finish(echo);

    if (err) return;

    int datasize = MP.samp_datasize->value().toUInt();
    if (datasize > 1000)
        datasize = 1000;
#ifdef MYOPT_TEST
    datasize = 59;
#endif
    qint16 data[datasize];
    for (int i = 0; i < 10; i++) {
        QVariantList list = MP.samp_data[i]->value().toList();
        for (int j = 0; j < 100 && j < list.size(); j++) {
            int ii = i*100+j;
            data[ii] = list.at(j).toInt();
#ifdef MYOPT_TEST
            data[ii] = (qrand() % 100)-10;
#endif
            if (ii+1 >= datasize) {
                i = 9;
                break;
            }
        }
    }
    ui->graph->setData(data, datasize);
    ui->graph->setLine(MP.samp_time->value().toInt()*10,
                       MP.prm_CZD_Dly->value().toInt(),
                       MP.samp_max->valuenp().toInt());
#ifdef MYOPT_TEST
    ui->graph->setLine(0, 10*datasize/2, 1);
#endif

    ui->samp->setText(MP.samp_weight->vStr());
    ui->max->setText(MP.samp_weight->vPS(ui->graph->max()));

    /*
    ui->graph->ClearData();
    ui->graph->m_nXScale = datasize;
    ui->graph->m_nDouDelay = MP.prm_CZD_Dly->value().toInt();
    ui->graph->m_nSampingTime = MP.samp_time->value().toInt();
#ifdef MYOPT_TEST
    ui->graph->m_nDouDelay = 600;
    ui->graph->m_nSampingTime = 20;
#endif

    int max = 0;
    for (int i = 0; i < 10; i++) {
        QVariantList list = MP.samp_data[i]->value().toList();
        for (int j = 0; j < 100 && j < list.size(); j++) {
            int v = list.at(j).toInt();
#ifdef MYOPT_TEST
            v = qrand() % 100;
#endif
            ui->graph->m_CurveData[i*100+j] = v;
            if (max < v)
                max = v;
            if (j+1 >= datasize) {
                i = 10;
                break;
            }
        }
    }
    ui->graph->m_nMax = max;
    ui->graph->update();

    ui->samp->setText(MP.samp_weight->vStr());
    ui->max->setText(MP.samp_max->vStr(max));
    */
}

void FormSamping::on_btn_zero_clicked()
{
    form->writeCMD(CTL_SEZ, dounum);
}

void FormSamping::on_btn_test_clicked()
{
    form->writeCMD(CTL_DOU_TEST, dounum);
    form->waitCmdFns(13000);

    MAC.read(MMP.Samp, echo);
}
