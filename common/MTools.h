#ifndef MTOOLS_H
#define MTOOLS_H

#include <QObject>
#include <QVariant>
#include <QDateTime>

extern QString MyDll_Dir;

class MTools : public QObject
{
    Q_OBJECT
public:
    MTools();
    static MTools *instance();
    static void setIniValue(const QString &key, const QVariant &v,
                            const QString &fn = "MainProgram.ini");

    // 权限密码设置
    static int auth();
    static void setAuth(int auth);
    static void setPassword(int auth, const QString &pwd);
    static bool checkPassword(int auth, const QString &pwd);

    // 语言设置
    static bool restartAfterChangeLang;
    static void loadQSS(QWidget *form);
    static void loadLang();
    static void changeLang(int id);
    static void changeLangId(int id);
    static int curLangID();
    static QVariantList getLangList();
    static QString getLangFileName(int id);
    static QString getStartupImageUrl();

    // 时间设置和BCD码时间转换
    static void startMyTimer();
    static void startDateTime();
    static void setDateTime(const QDateTime &time);
    static QString SecToHHMMSS(uint sec);
    static QDateTime BCDToDateTime(const QByteArray &bcd);
    static QByteArray DateTimeToBCD(const QDateTime &dt);

    // 屏幕设置
    static int bltime();
    static void startBLTime();
    static void setBLTime(const QVariant &minutes, int min=0, int max=99);
    static void blOn();
    static void blOff();
    static void tsCalib();

    // 软件升级
    static void softwareUpdate();
    static void softwareRestart();
    static void setLastNetUp(const QString &text);
    static QString lastNetUp();

    // 网络设置
    static void initNetConfig();
    static bool setIP(const QString &str);
    static bool setNM(const QString &str);
    static bool setGW(const QString &str);
    static inline QString ip() { return m_ip; }
    static inline QString nm() { return m_nm; }
    static inline QString gw() { return m_gw; }
    static void setServerIp(const QString &str);
    static void setServerPort(int i);
    static void setServerPort2(int i);
    static void setNetMode(int i);
    static inline QString serverIp() { return m_server_ip; }
    static inline int serverPort() { return m_server_port; }
    static inline int serverPort2() { return m_server_port2; }
    static inline int netMode() { return m_net_mode; }
    static bool isOpenWifi() { return m_isOpenWifi; }
    static bool isOnline() { return m_isOnline; }
    static void setIsOpenWifi(bool b);
    static void setIsOnline(bool b);

    // 其他
    static bool checkDiskSize(int m);

private:
    static int m_auth;
    static int m_langid;
    static int m_bltime;
    static int timerid;
    static bool isStartDateTime;
    static bool isStartBLTime;
    static qint64 curms;
    static qint64 lasttsms;
    static bool isBLOn;
    static QString m_ip;
    static QString m_nm;
    static QString m_gw;
    static QString m_server_ip;
    static int m_server_port;
    static int m_server_port2;
    static int m_net_mode;
    static bool m_isOpenWifi;
    static bool m_isOnline;

signals:
    void authChange(int auth);
    void bltimeChange(int min);
    void timeChange(const QDateTime &time);

protected:
    virtual bool eventFilter(QObject *, QEvent *);
    virtual void timerEvent(QTimerEvent *);
};

class MTools_AppEndExe {
public:
    MTools_AppEndExe();
    ~MTools_AppEndExe();
    QByteArray cmd;
};
extern MTools_AppEndExe _AppEndExe;

#endif // MTOOLS_H
