#include "FormMainAlarm.h"
#include "ui_FormMainAlarm.h"
#include "Doum.h"
#include <QButtonGroup>

FormMainAlarm::FormMainAlarm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMainAlarm)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);

    QButtonGroup *btg = new QButtonGroup(this);
    btg->addButton(ui->pushButton, 0);
    btg->addButton(ui->pushButton_2, 1);
    btg->addButton(ui->pushButton_3, 2);
    btg->addButton(ui->pushButton_hide, -1);
    ui->pushButton_hide->hide();
    connect(btg, SIGNAL(buttonClicked(int)), this, SLOT(onFunClicked(int)));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(onAlarmClear()));
}

FormMainAlarm::~FormMainAlarm()
{
    delete ui;
    form->mac.echom.destroy(ecIds, this);
}

void FormMainAlarm::init(FormA *f)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP., form->mpri);

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
    dm->setDouNumber(dLab, this);
    curseldn = -1;

    form->mac.echom.create(echorw, ecIds, this, "onActionFinished");
    isrw = false;
}

void FormMainAlarm::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();

    form->writeCMD(CTL_INTO_PAGE, 5);
}

void FormMainAlarm::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));

    ui->pushButton_hide->click();
}

bool FormMainAlarm::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QLabel *lab = (QLabel *)watched;
        int n = Doum::getDouNumber(lab);
        if (!form->isHaveAlarm(n-1))
            return false;
        if (ui->pushButton->isChecked()) {
            form->writeCMD(CTL_MANUAL_HCD, n);
        }
        else if (ui->pushButton_2->isChecked()) {
            form->writeCMD(CTL_MANUAL_CZD, n);
        }
        else if (ui->pushButton_3->isChecked()) {
            if (n == curseldn)
                form->writeCMD(CTL_SEZ, n);
        }
        if (curseldn >= 0)
            form->doum->setGDouImage(dLab, gLab, curseldn, 0);
        form->doum->setGDouImage(dLab, gLab, n, 7);
        curseldn = n;
        refreshUI();
    }
    return false;
}

void FormMainAlarm::refreshUI()
{
    for (int i = 0; i < dLab.size(); i++) {
        if (form->isHaveAlarm(i)) {
            if (i+1 == curseldn)
                form->doum->setGDouImage(dLab, gLab, i+1, 7);
            else
                form->doum->setGDouImage(dLab, gLab, i+1, 0);
        }
        else
            form->doum->setGDouImage(dLab, gLab, i+1, 15);
    }

    int doustatuscnt = (form->dn + 1) / 2;
    for (int i = 0; i < dwLab.size(); i++) {
        if (i+1 == curseldn && ui->pushButton_3->isChecked()) {
            int v = MP.doustatus[i + doustatuscnt]->valuenp().toInt();
            if (v != 0x7FFE) {
                dwLab.at(i)->setEnabled(true);
                dwLab.at(i)->setText(MP.doustatus[0]->vPS(v));
            }
            else {
                dwLab.at(i)->setEnabled(false);
                dwLab.at(i)->setText("ERR");
            }
            dwLab.at(i)->show();
        }
        else
            dwLab.at(i)->hide();
    }
    this->update();
}

void FormMainAlarm::onRegUpdated(const MRegAreas &areas)
{
    if (areas.isIn(MP.doustatus_union) || areas.isIn(MP.runstatus_union))
        refreshUI();
}

void FormMainAlarm::onActionFinished(int , const SignalEcho_zy1 &echo)
{
    form->mac.echom.finish(echo);
    if (echo == echorw)
        QTimer::singleShot(100, this, SLOT(onReadWeight()));
}

void FormMainAlarm::onReadWeight()
{
    if (curseldn >= 0)
        form->writeCMD(CTL_MSV, curseldn);
    form->mac.read(MP.doustatus_union, echorw);
}

void FormMainAlarm::onFunClicked(int )
{
    if (curseldn >= 0)
        form->doum->setGDouImage(dLab, gLab, curseldn, 0);
    curseldn = -1;

    if (ui->pushButton_3->isChecked() && !isrw) {
        isrw = true;
        onReadWeight();
    }
    else if (!ui->pushButton_3->isChecked() && isrw) {
        isrw = false;
        form->mac.clearAction(echorw, ecIds);
    }

    refreshUI();
}

void FormMainAlarm::onAlarmClear()
{
    form->writeCMD(0x04,CTL_RUN,0x00,0x00);
    form->mac.read(MP.runstatus_union);
}
