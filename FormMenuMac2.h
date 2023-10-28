#ifndef FORMMENUMAC2_H
#define FORMMENUMAC2_H

#include <QWidget>
#include <QProcess>
#include "FormA.h"

namespace Ui {
class FormMenuMac2;
}

class FormMenuMac2 : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuMac2(QWidget *parent = 0);
    ~FormMenuMac2();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);

    void on_net_ip_editingFinished();
    void on_net_nm_editingFinished();
    void on_net_gw_editingFinished();
    void on_net_server_ip_editingFinished();
    void on_net_mode_editingFinished();
    void on_mac_name_editingFinished();
    void on_wifi_scan_clicked();
    void on_wifi_conn_clicked();
    void on_wifi_disconn_clicked();

    void onConnOK();
    void onDhcpOK();
    void onPingOK();
    void onConnTimeout();
    void onWiFiStateChanged(bool conn);

private:
    void loadWifiConfig();

private:
    Ui::FormMenuMac2 *ui;
    FormA *form;
    MPTRWidget *mptrw;
    int starti;
    int endi;

    QProcess* cmdScan;
    QProcess* cmdConn;
    QProcess* cmdDhcp;
    QProcess* cmdPing;
    QTimer *totimer;
    QTimer *totimer2;
};

#endif // FORMMENUMAC2_H
