#include "FormMenuMac2.h"
#include "ui_FormMenuMac2.h"
#include "MTools.h"
#include "Tool.h"
#include "JServer_Mac.h"
#include "JServer_NetUp.h"
#include "ModbusTcpServer.h"
#include "JsonTcpServer.h"
#include "WiFiCheckThread.h"
#include <QButtonGroup>
#include <QFile>

QStringList trs_net_mode;

FormMenuMac2::FormMenuMac2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuMac2)
{
    if (trs_net_mode.isEmpty())
        trs_net_mode << tr("不能连网")
                     << tr("自动连接互联网")
                     << tr("手动连接互联网")
                        ;

    ui->setupUi(this);

    cmdScan = new QProcess(this);
    cmdConn = new QProcess(this);
    cmdDhcp = new QProcess(this);
    cmdPing = new QProcess(this);
    connect(cmdConn, SIGNAL(finished(int)), this, SLOT(onConnOK()));
    connect(cmdDhcp, SIGNAL(finished(int)), this, SLOT(onDhcpOK()));
    connect(cmdPing, SIGNAL(finished(int)), this, SLOT(onPingOK()));
    loadWifiConfig();

    totimer = new QTimer(this);
    totimer->setInterval(500);
    totimer->setSingleShot(true);
    totimer2 = new QTimer(this);
    totimer2->setInterval(12000);
    totimer2->setSingleShot(true);
    connect(totimer, SIGNAL(timeout()), totimer2, SLOT(start()));
    connect(totimer2, SIGNAL(timeout()), this, SLOT(onConnTimeout()));
    connect(&g_wific, SIGNAL(stateChanged(bool)),
            this, SLOT(onWiFiStateChanged(bool)));
}

FormMenuMac2::~FormMenuMac2()
{
    delete ui;
}

void FormMenuMac2::init(FormA *f)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP., form->mpri);

    starti = form->keybm->startadd();
    form->keybm->add(KEYBMET(ui->net_ip));
    form->keybm->add(KEYBMET(ui->net_nm));
    form->keybm->add(KEYBMET(ui->net_gw));
    form->keybm->add(KEYBMET(ui->net_server_ip));
    form->keybm->add(KEYBMET(ui->net_mode));
    form->keybm->add(KEYBMET(ui->mac_name));
    form->keybm->add(KEYBMET(ui->wifi_ssid));
    form->keybm->add(KEYBMET(ui->wifi_pwd));
    endi = form->keybm->endadd();
}

void FormMenuMac2::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
#ifndef MYOPT_OnlineManager
    ui->wifi_conn->setEnabled(MTools::netMode() != 0);
    ui->wifi_icon->setVisible(MTools::isOpenWifi());
#endif
#ifdef MYOPT_TEST
    static int i = 0;
    g_wific.stateChanged(++i%2);
#endif
}

void FormMenuMac2::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
    if (ui->wifi_lab->text() != tr("连接中..."))
        ui->wifi_lab->setText("");
}

void FormMenuMac2::refreshUI()
{
#ifndef MYOPT_OnlineManager
    ui->net_ip->setTextValue(MTools::ip());
    ui->net_nm->setTextValue(MTools::nm());
    ui->net_gw->setTextValue(MTools::gw());
    ui->net_server_ip->setTextValue(MTools::serverIp());
    ui->net_mode->setTextValue(trs_net_mode.at(MTools::netMode()));
    ui->mac_name->setTextValue(form->sv_mac->getName());
    ui->mac_cpusn->setTextValue(form->sv_mac->getCPUSN());
#endif
}

void FormMenuMac2::onRegUpdated(const MRegAreas &areas)
{
    Q_UNUSED(areas);
}

void FormMenuMac2::on_net_ip_editingFinished()
{
    MTools::setIP(ui->net_ip->textv());
}

void FormMenuMac2::on_net_nm_editingFinished()
{
    MTools::setNM(ui->net_nm->textv());
}

void FormMenuMac2::on_net_gw_editingFinished()
{
    MTools::setGW(ui->net_gw->textv());
}

void FormMenuMac2::on_net_server_ip_editingFinished()
{
#ifndef MYOPT_OnlineManager
    MTools::setServerIp(ui->net_server_ip->textv());
    form->mtcpsc->setIPPort(MTools::serverIp(), MTools::serverPort());
    form->jtcpsc->setIPPort(MTools::serverIp(), MTools::serverPort2());
#endif
}

void FormMenuMac2::on_net_mode_editingFinished()
{
    DlgList dlg(form);
    dlg.setWindowTitle(ui->net_mode->text());
    dlg.setList(trs_net_mode);
    if (dlg.exec() == QDialog::Accepted) {
        int i = dlg.currenti();
        if (i == 0) {
            if(cmdConn->state() != QProcess::NotRunning) return;
            if(cmdDhcp->state() != QProcess::NotRunning) return;
            if(cmdPing->state() != QProcess::NotRunning) return;
            if(totimer->isActive() || totimer2->isActive()) return;
            if (MTools::isOpenWifi()) {
                on_wifi_disconn_clicked();
                ui->wifi_lab->setText("");
            }
            MTools::setIsOnline(false);
            form->mtcps->setIsOnline(false);
            form->mtcpsc->setIsOnline(false);
            form->jtcps->setIsOnline(false);
            form->jtcpsc->setIsOnline(false);
        }
        MTools::setNetMode(i);
        ui->net_mode->setTextValue(trs_net_mode.at(i));
        ui->wifi_conn->setEnabled(i != 0);
    }
}

void FormMenuMac2::on_mac_name_editingFinished()
{
#ifndef MYOPT_OnlineManager
    form->sv_mac->setName(ui->mac_name->textv());
#endif
}

void FormMenuMac2::on_wifi_scan_clicked()
{
    if(cmdScan->state() != QProcess::NotRunning) return;
    if(cmdConn->state() != QProcess::NotRunning) return;
    if(cmdDhcp->state() != QProcess::NotRunning) return;
    if(totimer->isActive() || totimer2->isActive()) return;

    for (int i = 0; i < 3; i++) {
        cmdScan->start("iwlist", QStringList() << "wlan0" << "scanning");
        cmdScan->waitForFinished();
        if (cmdScan->exitCode() == 0)
            break;
        QThread::msleep(200);
    }

    QStringList wifiAddrList;
    QStringList wifiNameList;
    QList<int> wifiSignalList;
    QList<bool> wifiEncrList;

    QString outStr = QString::fromUtf8(cmdScan->readAllStandardOutput());
#ifdef MYOPT_TEST
    QFile file("wifi_scan");
    file.open(QFile::ReadOnly);
    outStr = QString::fromUtf8(file.readAll());
#endif
    QStringList list0 = outStr.split("Cell");
    QString str;
    int start;
    int end;

    // 解析扫描信息
    for (int k = 0; k < list0.size(); k++) {
        QStringList list = list0.at(k).split('\n');
        QString addr;
        QString ssid;
        int signal = 0;
        bool encr = false;

        for (int i = 0; i < list.size(); i++) {
            str = list.at(i).trimmed();
            start = str.indexOf("Address", 0, Qt::CaseInsensitive);
            if (start >= 0) {
                addr = str.right(17);
            }
            start = str.indexOf("SSID:\"", 0, Qt::CaseInsensitive);
            if (start >= 0) {
                start += strlen("SSID:\"");
                end = str.lastIndexOf("\"", -1, Qt::CaseInsensitive);
                ssid = str.mid(start, end - start);
            }
            start = str.indexOf("Signal level=", 0, Qt::CaseInsensitive);
            if (start >= 0) {
                start += strlen("Signal level=");
                end = str.indexOf("dBm", start, Qt::CaseInsensitive);
                signal = str.mid(start, end - start).toInt();
            }
            start = str.indexOf("Encryption", 0, Qt::CaseInsensitive);
            if (start >= 0) {
                QString encr_onoff = str.right(3);
                if(encr_onoff != "off") encr = true;
            }
        }
        if (ssid.isEmpty())
            continue;
        wifiAddrList.append(addr);
        wifiNameList.append(Tool::fromXString(ssid));
        wifiSignalList.append(signal);
        wifiEncrList.append(encr);
    }

    if(wifiNameList.isEmpty()) {
        DlgInfo::info(form, tr("没有搜索到WiFi"));
        return;
    }

    // 根据信号强度排序
    for(int i = 1; i < wifiSignalList.size(); i++) {
        int signal = wifiSignalList.at(i);
        bool encr = wifiEncrList.at(i);
        QString ssid = wifiNameList.at(i);
        QString addr = wifiAddrList.at(i);
        int j;
        for(j = i - 1; j >= 0 && signal > wifiSignalList.at(j); j--) {
            wifiSignalList[j + 1] = wifiSignalList.at(j);
            wifiEncrList[j + 1] = wifiEncrList.at(j);
            wifiNameList[j + 1] = wifiNameList.at(j);
            wifiAddrList[j + 1] = wifiAddrList.at(j);
        }
        wifiSignalList[j + 1] = signal;
        wifiEncrList[j + 1] = encr;
        wifiNameList[j + 1] = ssid;
        wifiAddrList[j + 1] = addr;
    }

    DlgList dlg(form);
    dlg.setList(wifiNameList);
    if (dlg.exec() == QDialog::Accepted)
        ui->wifi_ssid->setTextValue(dlg.currents());
}

void FormMenuMac2::on_wifi_conn_clicked()
{
    if(cmdConn->state() != QProcess::NotRunning) return;
    if(cmdDhcp->state() != QProcess::NotRunning) return;
    if(cmdPing->state() != QProcess::NotRunning) return;
    if(totimer->isActive() || totimer2->isActive()) return;

    QString ssid = ui->wifi_ssid->textv();
    QString pwd = ui->wifi_pwd->textv();
    if(ssid.isEmpty()) return;

#ifdef __ARM_EABI__
    ::system("killall wpa_supplicant");
    ::system("killall udhcpc");
    ::system("killall hostapd");
    ::system("killall udhcpd");

    ::system("ifconfig wlan0 down");
    ::system("ifconfig wlan0 up");
#endif
    if (!pwd.isEmpty()) {
        QProcess wpa_passphrase;
        wpa_passphrase.setStandardOutputFile("wifi.conf");
        wpa_passphrase.start("wpa_passphrase", QStringList() << ssid << pwd);
        wpa_passphrase.waitForFinished();
        if (wpa_passphrase.exitCode())
            return;
    }
    else {
        QString str("network={\n"
                    "    ssid=\"%1\"\n"
                    "    #psk=\"\"\n"
                    "    key_mgmt=NONE\n"
                    "}\n");
        str = str.arg(ssid);
        QFile file("wifi.conf");
        if (file.open(QFile::WriteOnly | QFile::Truncate)) {
            file.write(str.toUtf8());
            file.close();
        }
    }

#ifdef __ARM_EABI__
    cmdConn->start("wpa_supplicant", QStringList() << "-D" << "wext" << "-B" << "-i" << "wlan0" << "-c" << "wifi.conf");
#endif

    MTools::setIsOpenWifi(true);
    ui->wifi_icon->setVisible(true);
    ui->wifi_lab->setText(tr("连接中..."));
    totimer->start();
}

void FormMenuMac2::on_wifi_disconn_clicked()
{
    if(cmdConn->state() != QProcess::NotRunning) return;
    if(cmdDhcp->state() != QProcess::NotRunning) return;
    if(cmdPing->state() != QProcess::NotRunning) return;
    if(totimer->isActive() || totimer2->isActive()) return;

#ifdef __ARM_EABI__
    ::system("killall wpa_supplicant");
    ::system("killall udhcpc");
    ::system("killall hostapd");
    ::system("killall udhcpd");

    ::system("ifconfig wlan0 down");
    ::system("ifconfig wlan0 up");
#endif
    MTools::setIsOpenWifi(false);
    ui->wifi_icon->setVisible(false);
    ui->wifi_lab->setText(tr("连接已断开"));
}

void FormMenuMac2::onConnOK()
{
    ::system("killall udhcpc");
    cmdDhcp->start("udhcpc", QStringList() << "-b" << "-i" << "wlan0" << "-R");
}

void FormMenuMac2::onDhcpOK()
{
    //cmdPing->start("ping", QStringList() << "" << "-c3");
    //QTimer::singleShot(1000, this, SLOT(onPingOK()));
}

void FormMenuMac2::onPingOK()
{
    if (::system("iwconfig 2> /dev/null | grep -i Not-Associated 1> /dev/null")) {
        ui->wifi_lab->setText(tr("连接成功"));
    }
    else {
        ui->wifi_lab->setText(tr("连接失败"));
    }
}

void FormMenuMac2::onConnTimeout()
{
    ui->wifi_lab->setText(tr("连接失败"));
}

void FormMenuMac2::onWiFiStateChanged(bool conn)
{
    if (conn) {
        ui->wifi_icon->setPixmap(QPixmap(":/rc/icon/wifi_on.png"));
        if (ui->wifi_lab->text() == tr("连接中...") && !totimer->isActive()) {
            ui->wifi_lab->setText(tr("连接成功"));
            totimer2->stop();
        }
        else if (ui->wifi_lab->text() == tr("连接失败"))
                 ui->wifi_lab->setText("");
    }
    else {
        ui->wifi_icon->setPixmap(QPixmap(":/rc/icon/wifi_off.png"));
    }
    ui->wifi_icon->setVisible(MTools::isOpenWifi());
}

void FormMenuMac2::loadWifiConfig()
{
    QFile file("wifi.conf");
    if (!file.open(QIODevice::ReadOnly))
        return;
    QString wificonf = QString::fromUtf8(file.readAll());
    int index, index2;

    index = wificonf.indexOf("ssid=", 0, Qt::CaseInsensitive);
    if(index >= 0) {
        for(index2 = index + 6; index2 < wificonf.size() && wificonf.at(index2) != '\n'; index2++);
        ui->wifi_ssid->setTextValue(wificonf.mid(index + 6, index2 - 1 - (index + 6)));
    } else
        ui->wifi_ssid->setTextValue("");

    index = wificonf.indexOf("#psk=", 0, Qt::CaseInsensitive);
    if(index >= 0) {
        for(index2 = index + 6; index2 < wificonf.size() && wificonf.at(index2) != '\n'; index2++);
        ui->wifi_pwd->setTextValue(wificonf.mid(index + 6, index2 - 1 - (index + 6)));
    }else
        ui->wifi_pwd->setTextValue("");
}
