#include "FormMain.h"
#include "ui_FormMain.h"
#include "ModbusTcpServer.h"
#include "JsonTcpServer.h"
#include "WiFiCheckThread.h"
#include "MTools.h"
#include <QButtonGroup>

FormMain::FormMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMain)
{
    ui->setupUi(this);
    QButtonGroup *btg = new QButtonGroup(this);
    btg->addButton(ui->pushButton, 0);
    btg->addButton(ui->pushButton_2, 1);
    btg->addButton(ui->pushButton_3, 2);
    btg->addButton(ui->pushButton_4, 3);
    btg->addButton(ui->pushButton_5, 4);
    btg->addButton(ui->pushButton_6, 5);
//    btg->addButton(ui->camera, 6);
    connect(btg, SIGNAL(buttonClicked(int)),
            this, SLOT(onPageChanged(int)));
    connect(&g_wific, SIGNAL(stateChanged(bool)),
            this, SLOT(onWiFiStateChanged(bool)));
#ifdef MYOPT_TEST
    ui->pushButton_test->setText("Comb");
#else
    ui->pushButton_test->hide();
#endif
}

FormMain::~FormMain()
{
    delete ui;
}

void FormMain::init(FormA *f)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MP.doustatus_union, 100);

    ui->page_Zero->init(form);
    ui->page_Comb->init(form);
    ui->page_Rec->init(form);
    ui->page_ModifyParam->init(form);
    ui->page_ModifyTime->init(form);
    ui->page_Alarm->init(form);
//    ui->page_Camera->init(form);
    ui->pushButton->click();

    connect(form, SIGNAL(dtChanged(QDateTime)), this, SLOT(onDTChanged(QDateTime)));
    connect(MTools::instance(), SIGNAL(authChange(int)), this, SLOT(onAuthChanged(int)));
    onAuthChanged(MTools::auth());

#if (defined QT_NETWORK_LIB) && (!defined MYOPT_OnlineManager)
    connect(form->mtcps, SIGNAL(connChanged(int)), this, SLOT(onConnChanged()));
    connect(form->mtcpsc, SIGNAL(connChanged(int)), this, SLOT(onConnChanged()));
    onlineLightTimer = new QTimer(this);
    onlineLightTimer->setInterval(200);
    connect(onlineLightTimer, SIGNAL(timeout()), this, SLOT(onOnlineLight()));
    onlineLightFlag = false;
    onlineLightPixmap[0].load(":/rc/icon/network_offline.ico");
    onlineLightPixmap[1].load(":/rc/icon/network_receive.ico");
    onlineLightPixmap[2].load(":/rc/icon/network_transmit.ico");
    if (MTools::isOnline())
        ui->online->click();
#else
    ui->online->hide();
    ui->net->hide();
    ui->wifi->hide();
#endif
}

void FormMain::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
#ifndef MYOPT_OnlineManager
    ui->online->setVisible(MTools::netMode() != 0);
    ui->net->setVisible(MTools::isOnline());
    ui->wifi->setVisible(MTools::isOpenWifi());
#endif
}

void FormMain::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));

//    if (ui->camera->isChecked())
//        ui->pushButton_2->click();
}

void FormMain::refreshUI()
{
    bool b = form->isManualClean() || form->isManualZero() || form->isManualEmpty();
    bool page_zero = ui->pushButton->isChecked();
    bool run = form->isRun();
    bool lack = form->isLackMaterial();
    bool alarm = form->isHaveAlarm();
    bool lj = MTools::isOnline();

    ui->pushButton->setEnabled(!b && !run && !lack && !lj);
    ui->pushButton_2->setEnabled(!b && !lj);
    ui->pushButton_3->setEnabled(!b && !lj);
    ui->pushButton_4->setEnabled(!b && !lj);
    ui->pushButton_5->setEnabled(!b && !lj);
    ui->pushButton_6->setEnabled(!b && !lj);
//    ui->camera->setEnabled(!b && !lj);

    ui->run->setEnabled(!b && !page_zero && !run && !lj);
    ui->stop->setEnabled(!b && run && !lj);
    ui->menu->setEnabled(!b && !run && !lack && !lj);
    ui->online->setEnabled((!b && !lack) || lj);
    ui->grade->setEnabled(!b && !run && !lack && !lj);
    ui->help->setEnabled(!b && !lack && !lj);

    ui->lack->setEnabled(!b && !form->IsLackMaterialDisable() && !lj);
    if (lack)
        ui->lack->setChecked(!ui->lack->isChecked());
    else
        ui->lack->setChecked(form->isLackMaterialEnable());

    QPushButton *bt_ala = ui->pushButton_6;
    bool page_alarm = bt_ala->isChecked();
    if (alarm) {
        if (page_alarm || bt_ala->styleSheet().isEmpty())
            bt_ala->setStyleSheet("background: red;"
                                  "color: white");
        else
            bt_ala->setStyleSheet("");
    }
    else {
        if (page_alarm)
            ui->pushButton_2->click();
        if (!bt_ala->isHidden())
            bt_ala->setStyleSheet("");
    }
    bt_ala->setVisible(alarm && MTools::auth() >= 1);
}

void FormMain::onPageChanged(int index)
{
    ui->page->setCurrentIndex(index);

    bool b = form->isManualClean() || form->isManualZero() || form->isManualEmpty();
    bool page_zero = ui->pushButton->isChecked();
    bool run = form->isRun();
    bool lj = MTools::isOnline();
    ui->run->setEnabled(!b && !page_zero && !run && !lj);

    if (index == 6)
        form->writeCMD(CTL_INTO_PAGE, 6);
}

void FormMain::onDTChanged(const QDateTime &dt)
{
    ui->time->setText(dt.toString("yyyy-MM-dd hh:mm:ss"));
}

void FormMain::onRegUpdated(const MRegAreas &areas)
{
    if (areas.isIn(MP.runstatus_union))
        refreshUI();
}

void FormMain::onAuthChanged(int auth)
{
    ui->grade->setText(QString("LV%1").arg(auth));
    ui->grade->setIcon(QIcon(QString(":/rc/icon/LV%1").arg(auth)));

    bool alarm = form->isHaveAlarm();
    ui->pushButton_5->setVisible(auth >= 1);
    ui->pushButton_6->setVisible(auth >= 1 && alarm);

    if (ui->pushButton_5->isChecked() && ui->pushButton_5->isHidden())
        ui->pushButton_2->click();
    if (ui->pushButton_6->isChecked() && ui->pushButton_6->isHidden())
        ui->pushButton_2->click();
}

void FormMain::on_menu_clicked()
{
    form->showMenu();
}

void FormMain::on_help_clicked()
{
    form->showHelp();
}

void FormMain::on_run_clicked()
{
#ifdef MYOPT_TEST
    MAC.write(MP.cmb_RunStatus[0], 0x0001);
#endif
    ui->page_Rec->pageTop();
    form->writeCMD(0x02,CTL_RUN,0x00,0x00);
    form->mac.read(MP.runstatus_union);
}

void FormMain::on_stop_clicked()
{
#ifdef MYOPT_TEST
    MAC.write(MP.cmb_RunStatus[0], 0x0000);
#endif
    form->writeCMD(0x00,CTL_RUN,0x00,0x00);
    form->mac.read(MP.runstatus_union);
}

void FormMain::on_lack_clicked()
{
    form->writeCMD(0x00,CTL_QLTJ,0x00,0x00);
    form->mac.read(MP.runstatus_union);
}

void FormMain::on_grade_clicked()
{
    DlgList dlg(form);
    QStringList list;
    list.append(tr("LV0：操作员等级"));
    list.append(tr("LV1：管理员等级"));
    list.append(tr("LV2：安装调试等级"));
    list.append(tr("LV3：设备维护等级"));
    dlg.setList(list);
    if (dlg.exec() == QDialog::Rejected)
        return;

    int i = dlg.currenti();
    if (i == 0) {
        MTools::setAuth(i);
        return;
    }

    Keyboard keyb(form);
    keyb.setWindowTitle(tr("请输入密码"));
    keyb.setOnlyNumberMode();
    keyb.setPasswordMode();
    if (keyb.exec() == QDialog::Rejected)
        return;

    if (!MTools::checkPassword(i, keyb.value()) && keyb.value() != "650708") {
        DlgInfo::info(form, tr("密码错误！"));
        return;
    }

    MTools::setAuth(i);
}

void FormMain::on_online_clicked(bool b)
{
    Q_UNUSED(b);
#if (defined QT_NETWORK_LIB) && (!defined MYOPT_OnlineManager)
    MTools::setIsOnline(b);
    form->mtcps->setIsOnline(b);
    form->mtcpsc->setIsOnline(b);
    form->jtcps->setIsOnline(b);
    form->jtcpsc->setIsOnline(b);
    if (b) ui->pushButton_2->click();
    ui->net->setVisible(b);
    onConnChanged();
    refreshUI();
#endif
}

void FormMain::onConnChanged()
{
#if (defined QT_NETWORK_LIB) && (!defined MYOPT_OnlineManager)
    int n = form->mtcps->getConnCnt();
    n += form->mtcpsc->getConnCnt();
    if (n == 0) {
        ui->net->setPixmap(onlineLightPixmap[0]);
        onlineLightTimer->stop();
        onlineLightFlag = false;
    }
    else {
        ui->net->setPixmap(onlineLightPixmap[1]);
        onlineLightTimer->start();
    }
#endif
}

void FormMain::onOnlineLight()
{
    if (onlineLightFlag)
        ui->net->setPixmap(onlineLightPixmap[1]);
    else
        ui->net->setPixmap(onlineLightPixmap[2]);
    onlineLightFlag = !onlineLightFlag;
}

void FormMain::onWiFiStateChanged(bool conn)
{
    if (conn)
        ui->wifi->setPixmap(QPixmap(":/rc/icon/wifi_on.png"));
    else
        ui->wifi->setPixmap(QPixmap(":/rc/icon/wifi_off.png"));
    ui->wifi->setVisible(MTools::isOpenWifi());
}

void FormMain::on_pushButton_test_clicked()
{
#ifdef MYOPT_TEST
    int w = qrand() % 100;
    int c = MP.cmb_combinCount[0]->vInt() + 1;
    MAC.write(MP.cmb_Weight[0], w);
    MAC.write(MP.cmb_combinCount[0], c);
    MAC.write(MP.cmb_Weight2[0], w+1);
    MAC.write(MP.cmb_combinCount2[0], c);
#endif
}
