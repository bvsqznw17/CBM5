#include "FormTestZhenDong.h"
#include "ui_FormTestZhenDong.h"

FormTestZhenDong::FormTestZhenDong(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormTestZhenDong)
{
    ui->setupUi(this);
}

FormTestZhenDong::~FormTestZhenDong()
{
    delete ui;
}

void FormTestZhenDong::init(FormA *f)
{
    form = f;
    mptrw = new MPTRWidget(this, &form->mac, MP.sys_VibrationCoe, form->mpri);
    mptr = new MPTReader(this, &form->mac, MMP.ZdTest, 10, MPTReader::Interval);
    mptr->s = 1;

    connect(mptr, SIGNAL(readFinished(int)), this, SLOT(onReadWeightFns()));
    connect(mptr, SIGNAL(readSkip()), this, SLOT(onReadWeightFns()));

    starti = form->keybm->startadd();
    form->keybm->add(KEYBMET(ui->val_1), MP.sys_VibrationCoe);
    endi = form->keybm->endadd();
}

void FormTestZhenDong::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
}

void FormTestZhenDong::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
    mptr->stop();
    ui->graph->clear();
    ui->weight_dou->setText("");
    ui->weight_zd->setText("");
}

void FormTestZhenDong::refreshUI()
{
    form->keybm->refreshMParam(starti, endi);
    ui->dounum->setText(QString::number(dounum));
    ui->btn_cpt->setEnabled(MP.zdtest_cpt->value().toBool());
}

void FormTestZhenDong::onRegUpdated(const MRegAreas &areas)
{
    Q_UNUSED(areas);
    if (areas.isIn(MP.sys_VibrationCoe))
        refreshUI();
}

void FormTestZhenDong::onReadWeightFns()
{
    int w1 = MP.zdtest_dou_weight->valuenp().toInt();
    int w2 = MP.zdtest_zd_weight->valuenp().toInt();
#ifdef MYOPT_TEST
    w1 = qrand() % 10 + 150;
    w2 = qrand() % 10 + 100;
#endif
    ui->graph->add(w1, w2, 5);
    ui->weight_dou->setText(MP.zdtest_dou_weight->vStrU());
    ui->weight_zd->setText(MP.zdtest_zd_weight->vStrU());
}

void FormTestZhenDong::on_btn_start_clicked()
{
    ui->graph->clear();
    form->writeCMD(CTL_TEST_ZHENDONG, dounum);
    mptr->start();
}

void FormTestZhenDong::on_btn_stop_clicked()
{
    mptr->stop();
    form->writeCMD(CTL_TEST_ZHENDONG, 'S');
}

void FormTestZhenDong::on_btn_cpt_clicked()
{
    form->writeCMD(CTL_TEST_ZHENDONG, 'J');
}

void FormTestZhenDong::on_btn_zero_clicked()
{
    form->writeCMD(CTL_TEST_ZHENDONG, 'Z');
}

void FormTestZhenDong::on_btn_back_clicked()
{
    form->writeCMD(CTL_TEST_ZHENDONG, 'S');
    form->backPage();
}
