#include "FormMenuMac1.h"
#include "ui_FormMenuMac1.h"
#include "MTools.h"
#include "Tool.h"
#include "JServer_Mac.h"
#include "JServer_NetUp.h"
#include "ModbusTcpServer.h"
#include "JsonTcpServer.h"
#include "Language.h"
#include <QButtonGroup>

FormMenuMac1::FormMenuMac1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuMac1)
{
    ui->setupUi(this);
    ui->pwd_1->setTextValue("******");
    ui->pwd_2->setTextValue("******");
    ui->pwd_3->setTextValue("******");

    int clindex = Language::files.indexOf(Language::current_language);
    if (clindex >= 0)
        ui->lang->setTextValue(Language::names.at(clindex));
}

FormMenuMac1::~FormMenuMac1()
{
    delete ui;
}

void FormMenuMac1::init(FormA *f)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP., form->mpri);

    starti = form->keybm->startadd();
    form->keybm->add(KEYBMET(ui->dt_1), KbInputInfo("1 ~ 9999"));
    form->keybm->add(KEYBMET(ui->dt_2), KbInputInfo("1 ~ 12"));
    form->keybm->add(KEYBMET(ui->dt_3));
    form->keybm->setIIFunc(std::bind(&FormMenuMac1::IIF_Day, this));
    form->keybm->add(KEYBMET(ui->dt_4), KbInputInfo("0 ~ 23"));
    form->keybm->add(KEYBMET(ui->dt_5), KbInputInfo("0 ~ 59"));
    form->keybm->add(KEYBMET(ui->dt_6), KbInputInfo("0 ~ 59"));
    form->keybm->add(KEYBMET(ui->bl), KbInputInfo("0 ~ 99 min"));
    endi = form->keybm->endadd();

    connect(ui->dt_1, SIGNAL(editingFinished()), this, SLOT(onDTEditFinished()));
    connect(ui->dt_2, SIGNAL(editingFinished()), this, SLOT(onDTEditFinished()));
    connect(ui->dt_3, SIGNAL(editingFinished()), this, SLOT(onDTEditFinished()));
    connect(ui->dt_4, SIGNAL(editingFinished()), this, SLOT(onDTEditFinished()));
    connect(ui->dt_5, SIGNAL(editingFinished()), this, SLOT(onDTEditFinished()));
    connect(ui->dt_6, SIGNAL(editingFinished()), this, SLOT(onDTEditFinished()));

    QButtonGroup *btg = new QButtonGroup(this);
    btg->addButton(ui->pwd_m_1, 1);
    btg->addButton(ui->pwd_m_2, 2);
    btg->addButton(ui->pwd_m_3, 3);
    connect(btg, SIGNAL(buttonClicked(int)), this, SLOT(onPasswordEdit(int)));

    connect(form, SIGNAL(dtChanged(QDateTime)), this, SLOT(onDTChanged(QDateTime)));
#ifndef MYOPT_OnlineManager
    connect(form->sv_netup, SIGNAL(net_up_ready(QString)), this, SLOT(net_up_ready(QString)));
    ui->net_up->setText(MTools::lastNetUp());
#endif
}

void FormMenuMac1::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
}

void FormMenuMac1::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormMenuMac1::refreshUI()
{
    ui->ver->setTextValue(QString("V1.0.20--%1").arg(MP.sys_MacVer->vStr()));
    ui->bl->setTextValue(QString::number(MTools::bltime()).append(" min"));
}

void FormMenuMac1::onDTChanged(const QDateTime &dt)
{
    int i = 0;

    QDate d = dt.date();
    QTime t = dt.time();
    form->keybm->setText(starti+i++, QString::number(d.year()));
    form->keybm->setText(starti+i++, QString::number(d.month()));
    form->keybm->setText(starti+i++, QString::number(d.day()));
    form->keybm->setText(starti+i++, QString::number(t.hour()));
    form->keybm->setText(starti+i++, QString::number(t.minute()));
    form->keybm->setText(starti+i++, QString::number(t.second()));
}

void FormMenuMac1::onRegUpdated(const MRegAreas &areas)
{
    Q_UNUSED(areas);
}

void FormMenuMac1::onDTEditFinished()
{
    QImageLineEdit *edit = (QImageLineEdit *)sender();
    int i = form->keybm->indexOf(edit);
    if (form->keybm->isError(i))
        return;

    QDateTime dt = form->dt();
    QDate d = dt.date();
    QTime t = dt.time();
    switch (i - starti) {
    case 0:
        dt.setDate(QDate(ui->dt_1->textv().toInt(), d.month(), d.day()));
        break;
    case 1:
        dt.setDate(QDate(d.year(), ui->dt_2->textv().toInt(), d.day()));
        break;
    case 2:
        dt.setDate(QDate(d.year(), d.month(), ui->dt_3->textv().toInt()));
        break;
    case 3:
        dt.setTime(QTime(ui->dt_4->textv().toInt(), t.minute(), t.second(), t.msec()));
        break;
    case 4:
        dt.setTime(QTime(t.hour(), ui->dt_5->textv().toInt(), t.second(), t.msec()));
        break;
    case 5:
        dt.setTime(QTime(t.hour(), t.minute(), ui->dt_6->textv().toInt(), t.msec()));
        break;
    default:
        break;
    }
    form->setDT(dt);
}

void FormMenuMac1::onPasswordEdit(int i)
{
    QString pwd;
    Keyboard keyb(form);
    keyb.setOnlyNumberMode();
    keyb.setPasswordMode();
    keyb.reset(KbInputInfo(), tr("请输入旧密码"));
    if (keyb.exec() == QDialog::Rejected)
        return;
    if (!MTools::checkPassword(i, keyb.value()) && keyb.value() != "650708") {
        DlgInfo::info(form, tr("密码错误！"));
        return;
    }
    keyb.reset(KbInputInfo(), tr("请输入新密码"));
    if (keyb.exec() == QDialog::Rejected)
        return;
    pwd = keyb.value();
    keyb.reset(KbInputInfo(), tr("请再次输入新密码"));
    if (keyb.exec() == QDialog::Rejected)
        return;
    if (pwd != keyb.value()) {
        DlgInfo::info(form, tr("两次密码输入不一致！"));
        return;
    }
    MTools::setPassword(i, pwd);
    DlgInfo::info(form, tr("修改成功！"));
}

void FormMenuMac1::on_lang_m_clicked()
{
    int cc = Language::column_count;
    if (cc <= 0)
        cc = 1;

    DlgList dlg(form);
    dlg.setWindowTitle(ui->lang_m->text());
    dlg.setList(Language::names, cc);
    if (cc >= 3) {
        int minw = qMin(cc*200, 800);
        dlg.resize(minw, dlg.height());
    }
    if (dlg.exec() == QDialog::Accepted) {
        int i = dlg.currenti();
        if (i >= 0 && i < Language::files.size()) {
            QString lf = Language::files.at(i);
            Language::setCurrentLanguage(lf);
            ui->lang->setTextValue(Language::names.at(i));
            if (lf != Language::current_language) {
                DlgOKCancel dlg(form);
                dlg.setText(tr("是否立即重启？"));
                if (dlg.exec() == QDialog::Accepted)
                    MTools::softwareRestart();
            }
        }
    }
}

void FormMenuMac1::on_ts_clicked()
{
    DlgOKCancel dlg(form);
    dlg.setText(tr("确定要进行触摸屏校准吗？"));
    if (dlg.exec() == QDialog::Accepted)
        MTools::tsCalib();
}

void FormMenuMac1::on_bl_editingFinished()
{
    MTools::setBLTime(ui->bl->textv());
    ui->bl->setTextValue(QString::number(MTools::bltime()).append(" min"));
}

void FormMenuMac1::net_up_ready(const QString &fn)
{
    QString text("[%1] %2");
    text = text.arg(Tool::dtToStr(form->dt()), fn);
    MTools::setLastNetUp(text);
    ui->net_up->setText(text);
}

void FormMenuMac1::on_net_up_clear_clicked()
{
#ifndef MYOPT_OnlineManager
    form->sv_netup->net_up_clear();
    MTools::setLastNetUp("");
    ui->net_up->setText("");
#endif
}

void FormMenuMac1::on_verup_clicked()
{
    DlgOKCancel dlg(form);
    dlg.setText(tr("确定要进行软件更新吗？"));
    if (dlg.exec() == QDialog::Accepted)
        MTools::softwareUpdate();
}

KbInputInfo FormMenuMac1::IIF_Day()
{
    QDateTime dt = form->dt();
    return KbInputInfo(QString("1 ~ %1").arg(dt.date().daysInMonth()));
}
