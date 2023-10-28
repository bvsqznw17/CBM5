#include "FormHelpDouState.h"
#include "ui_FormHelpDouState.h"
#include "MTools.h"
#include "Doum.h"
#include "DialogDouStateAdjust.h"
#include <QFile>
#include <QTextStream>

FormHelpDouState::FormHelpDouState(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormHelpDouState)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);
}

FormHelpDouState::~FormHelpDouState()
{
    delete ui;
}

void FormHelpDouState::init(FormA *f)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP., form->mpri);

#define MY_Replace(I) dou[I-1] = ui->dou_##I; lab[I-1] = ui->lab_##I;
    MY_Replace(1);
    MY_Replace(2);
    MY_Replace(3);
    MY_Replace(4);
    MY_Replace(5);
    MY_Replace(6);
    MY_Replace(7);
    MY_Replace(8);
    MY_Replace(9);
    MY_Replace(10);
    MY_Replace(11);
    MY_Replace(12);
    MY_Replace(13);
    MY_Replace(14);
    MY_Replace(15);
    MY_Replace(16);
#undef MY_Replace
    // 如果有多个FomrA，可能需要共用QPixmap.???
    if (!form->doum->dimg.isEmpty()) {
        for (int i = 0; i < 16; i++) {
            dou[i]->setProperty("doustate_id", i);
            dou[i]->installEventFilter(this);
            QPixmap pm = form->doum->dimg.at(0).at(i);
            pm = pm.scaled(200, dou[i]->minimumHeight(),
                           Qt::KeepAspectRatio, Qt::SmoothTransformation);
            dou[i]->setMinimumSize(pm.size());
            dou[i]->setPixmap(pm);
        }
    }

    dstate = form->doum->dstate;
    img.load(QString("./images/%1d/d1.png").arg(form->doum->dn));
//    img.load("1.png");
}

void FormHelpDouState::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
}

void FormHelpDouState::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

bool FormHelpDouState::eventFilter(QObject *watched, QEvent *event)
{
    if (MTools::auth() < 3)
        return false;

    QLabel *lab = (QLabel *)watched;
    if (event->type() == QEvent::MouseButtonPress) {
        lab->setProperty("Pressed", true);
        lab->setStyle(lab->style());
    }
    else if (event->type() == QEvent::MouseButtonRelease) {
        lab->setProperty("Pressed", false);
        lab->setStyle(lab->style());
        showDialog(lab->property("doustate_id").toUInt());
    }
    return  false;
}

void FormHelpDouState::refreshUI()
{
    bool b = MTools::auth() >= 3;
    ui->label->setVisible(b);
    ui->pushButton->setVisible(b);
    ui->pushButton_2->setVisible(b);
}

void FormHelpDouState::onRegUpdated(const MRegAreas &areas)
{
    Q_UNUSED(areas);
}

void FormHelpDouState::showDialog(int i)
{
    int h = dstate.at(i);
    int s = dstate.at(i + 16);
    int l = dstate.at(i + 32);
    bool b = dstate.at(i + 48);
    DialogDouStateAdjust dlg(form);
    dlg.setWindowTitle(lab[i]->text());
    dlg.setContent(img, h, s, l, b);
    if (dlg.exec() == QDialog::Rejected)
        return;

    QPixmap pm = QPixmap::fromImage(dlg.img());
    pm = pm.scaled(dou[i]->minimumSize(),
                   Qt::KeepAspectRatio, Qt::SmoothTransformation);
    dou[i]->setPixmap(pm);

    dstate[i] = dlg.hue();
    dstate[i + 16] = dlg.saturation();
    dstate[i + 32] = dlg.lightness();
    dstate[i + 48] = dlg.isShow0f();
    Doum::saveState(form->doum->dn, dstate);
}

void FormHelpDouState::on_pushButton_clicked()
{
    DlgOKCancel dlg(form);
    dlg.setText(tr("确定要恢复到系统默认的颜色设置吗？"));
    if (dlg.exec() == QDialog::Rejected)
        return;

    Doum::removeState(form->doum->dn);
    dstate = Doum::readState(form->doum->dn);
    QVector<QPixmap> list = Doum::createDouImg(img, dstate);
    for(int i = 0; i < 16; i++) {
        QPixmap pm = list.at(i);
        pm = pm.scaled(dou[i]->minimumSize(),
                       Qt::KeepAspectRatio, Qt::SmoothTransformation);
        dou[i]->setPixmap(pm);
    }
}

void FormHelpDouState::on_pushButton_2_clicked()
{
    DlgOKCancel dlg(form);
    dlg.setText(tr("确定要撤销本次启动之后所做的颜色修改吗？"));
    if (dlg.exec() == QDialog::Rejected)
        return;

    dstate = form->doum->dstate;
    Doum::saveState(form->doum->dn, dstate);
    for (int i = 0; i < 16; i++) {
        QPixmap pm = form->doum->dimg.at(0).at(i);
        pm = pm.scaled(dou[i]->minimumSize(),
                       Qt::KeepAspectRatio, Qt::SmoothTransformation);
        dou[i]->setPixmap(pm);
    }
}
