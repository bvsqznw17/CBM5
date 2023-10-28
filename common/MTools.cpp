#include "MTools.h"
#include <QMutex>
#include <QCryptographicHash>
#include <QApplication>
#include <QSettings>
#include <QResource>
#include <QFile>
#include <QWidget>
#include <QProcess>
#include <QImageReader>
#ifdef QT_NETWORK_LIB
#include <QHostAddress>
#endif
#include <QDebug>

int MTools::m_auth = 0;
int MTools::m_langid = INT_MIN;
int MTools::m_bltime = INT_MIN;
int MTools::timerid = 0;
bool MTools::isStartBLTime = false;
bool MTools::isStartDateTime = false;
qint64 MTools::curms;
qint64 MTools::lasttsms;
bool MTools::isBLOn = true;
QString MTools::m_ip;
QString MTools::m_nm;
QString MTools::m_gw;
QString MTools::m_server_ip;
int MTools::m_server_port;
int MTools::m_server_port2;
int MTools::m_net_mode;
bool MTools::m_isOpenWifi;
bool MTools::m_isOnline;

QString MyDll_Dir;
#ifdef __ARM_EABI__
#define MY_SYSTEM_SYNC ::system("sync")
#else
#define MY_SYSTEM_SYNC
#endif

MTools::MTools() : QObject()
{
}

MTools *MTools::instance()
{
    static MTools *_instance = 0;
    static QMutex _mutex;

    if (!_instance) {
        _mutex.lock();
        if (!_instance)
            _instance = new MTools;
        _mutex.unlock();
    }

    return _instance;
}

void MTools::setIniValue(const QString &key, const QVariant &v, const QString &fn)
{
    QSettings settings(MyDll_Dir+fn, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    settings.setValue(key, v);
    settings.sync();
    MY_SYSTEM_SYNC;
}

int MTools::auth()
{
    return m_auth;
}

void MTools::setAuth(int auth)
{
    m_auth = auth;
    instance()->authChange(m_auth);
}

void MTools::setPassword(int auth, const QString &pwd)
{
    QString a_pwd(pwd); a_pwd.append("@sg%");
    QString c_pwd(QCryptographicHash::hash(a_pwd.toUtf8(), QCryptographicHash::Md5).toBase64());
    setIniValue(QString("pwd_%1").arg(auth), c_pwd);
}

bool MTools::checkPassword(int auth, const QString &pwd)
{
    QString a_pwd(pwd); a_pwd.append("@sg%");
    QString c_pwd(QCryptographicHash::hash(a_pwd.toUtf8(), QCryptographicHash::Md5).toBase64());
    QSettings settings(MyDll_Dir+"MainProgram.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    QString p = settings.value(QString("pwd_%1").arg(auth)).toString();
    if(p == c_pwd || p.isEmpty()) return true;
    else return false;
}

bool MTools::restartAfterChangeLang = false;

void MTools::loadQSS(QWidget *form)
{
    QString fn;
    QString str = ":/"+MyDll_Dir+"def.qss";
    QString def = "./"+MyDll_Dir+"def.qss";
    if (QFile::exists(str))
        fn = str;
    else
        fn = def;
    QFile qss(fn);
    qss.open(QIODevice::ReadOnly);
    form->setStyleSheet(QString::fromUtf8(qss.readAll()));
    qss.close();
}

void MTools::loadLang()
{
    QString lfn = getLangFileName(curLangID());
    if (!lfn.isEmpty())
        QResource::registerResource(lfn, '/'+MyDll_Dir);
}

void MTools::changeLang(int id)
{
    int oldID = curLangID();
    if(oldID == id)
        return;

    changeLangId(id);

    QResource::unregisterResource(getLangFileName(oldID), '/'+MyDll_Dir);
    QResource::registerResource(getLangFileName(id), '/'+MyDll_Dir);
}

void MTools::changeLangId(int id)
{
    int oldID = curLangID();
    if(oldID == id)
        return;

    setIniValue("lang", id);
    m_langid = id;
}

int MTools::curLangID()
{
    if(m_langid != INT_MIN)
        return m_langid;

    QSettings settings(MyDll_Dir+"MainProgram.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    m_langid = settings.value("lang").toInt();
    return m_langid;
}

QVariantList MTools::getLangList()
{
    QVariantList vl;
    QString str_ = MyDll_Dir+"lang_%1.rcc";
    QString string_ = MyDll_Dir+"language%1.rcc";
    for (int i = 0; i < 100; i++) {
        if (QFile::exists(str_.arg(i)) || QFile::exists(string_.arg(i+1)))
            vl.append(i);
    }
    return vl;
}

QString MTools::getLangFileName(int id)
{
    QString str_ = MyDll_Dir+"lang_%1.rcc";
    QString string_ = MyDll_Dir+"language%1.rcc";
    QString str = str_.arg(id);
    QString string = string_.arg(id + 1);

    if (QFile::exists(str))
        return str;
    if (QFile::exists(string))
        return string;
    return 0;
}

QString MTools::getStartupImageUrl()
{
    int id = curLangID();
    QString str_ = MyDll_Dir+"startup/%1";
    QString string_ = MyDll_Dir+"startup/language%1";
    QString def_ = MyDll_Dir+"startup/def%1";
    QString str = str_.arg(id);
    QString string = string_.arg(id + 1);
    QString def = def_.arg(id);
    if (!QImageReader(str).size().isEmpty())
        return str;
    if (!QImageReader(string).size().isEmpty())
        return string;
    if (!QImageReader(def).size().isEmpty())
        return def;
    return 0;
}

void MTools::startMyTimer()
{
    if(timerid == 0){
        curms = QDateTime::currentMSecsSinceEpoch();
        timerid = instance()->startTimer(50);
    }
}

void MTools::startDateTime()
{
    if(!isStartDateTime) {
        isStartDateTime = true;
        startMyTimer();
        instance()->timeChange(QDateTime::fromMSecsSinceEpoch(curms));
    }
}

void MTools::setDateTime(const QDateTime &dt)
{
    QString cmd = QString("date -s \"%1\"").arg(dt.toString("yyyy-MM-dd hh:mm:ss"));
    #ifdef __ARM_EABI__
    system(cmd.toUtf8().constData());
    system("hwclock -w");
    #else
    qDebug()<<cmd;
    #endif
}

QString MTools::SecToHHMMSS(uint sec)
{
    uint h,m,s;
    s = sec % 60;
    sec /= 60;
    m = sec % 60;
    sec /= 60;
    h = sec;
    return QString("%1:%2:%3")
            .arg(h, 2, 10, QChar('0'))
            .arg(m, 2, 10, QChar('0'))
            .arg(s, 2, 10, QChar('0'));
}

QDateTime MTools::BCDToDateTime(const QByteArray &bcd)
{
    uint data[7];
    if(bcd.length() < 7) return QDateTime();
    for(int i=0; i<7; i++){
        uint d = bcd.at(i);
        data[i] = (d & 0x0F) + ((d >> 4) & 0x0F) * 10;
    }

    int year = data[0] * 100;
    year += data[1];
    int month = data[2];
    int day = data[3];
    int hour = data[4];
    int minute = data[5];
    int second = data[6];

    if(year<1)year=1; else if(year>9999)year=9999;
    if(month<1)month=1; else if(month>12)month=12;
    int dim = QDate(year,month,1).daysInMonth();
    if(day<1)day=1; else if(day>dim)day=dim;
    if(hour<0)hour=0; else if(hour>23)hour=23;
    if(minute<0)minute=0; else if(minute>59)minute=59;
    if(second<0)second=0; else if(second>59)second=59;

    return QDateTime(QDate(year,month,day),QTime(hour,minute,second));
}

QByteArray MTools::DateTimeToBCD(const QDateTime &dt)
{
    uint data[7];
    QByteArray bcd(7, Qt::Uninitialized);
    data[0] = dt.date().year() / 100;
    data[1] = dt.date().year() % 100;
    data[2] = dt.date().month();
    data[3] = dt.date().day();
    data[4] = dt.time().hour();
    data[5] = dt.time().minute();
    data[6] = dt.time().second();
    for(int i=0; i<7; i++){
        uint d = data[i] % 100;
        bcd[i] = (d % 10) | ((d / 10) << 4);
    }
    return bcd;
}

int MTools::bltime()
{
    if(m_bltime != INT_MIN)
        return m_bltime;

    QSettings settings("MainProgram.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    m_bltime = settings.value("bltime").toInt();
    return m_bltime;
}

void MTools::startBLTime()
{
    if(!isStartBLTime) {
        isStartBLTime = true;
        startMyTimer();
        lasttsms = QDateTime::currentMSecsSinceEpoch();
        qApp->installEventFilter(instance());
        blOn();
    }
}

void MTools::setBLTime(const QVariant &minutes, int min, int max)
{
    bool ok;
    int n = minutes.toInt(&ok);
    if(ok == false) return;
    if(n < min || n > max) return;

    setIniValue("bltime", n);

    m_bltime = n;
    instance()->bltimeChange(n);
}

void MTools::blOn()
{
    isBLOn = true;
    #ifdef __ARM_EABI__
    system("echo 255 > /sys/class/backlight/backlight/brightness");
    #else
    //qDebug("echo 255 > /sys/class/backlight/backlight/brightness");
    #endif
}

void MTools::blOff()
{
    isBLOn = false;
    #ifdef __ARM_EABI__
    system("echo 0 > /sys/class/backlight/backlight/brightness");
    #else
    //qDebug("echo 0 > /sys/class/backlight/backlight/brightness");
    #endif
}

void MTools::tsCalib()
{
    #ifdef __ARM_EABI__
    system("weston-touch-calibrator");
    MY_SYSTEM_SYNC;
    #endif
}

void MTools::softwareUpdate()
{
    #ifdef __ARM_EABI__
    if (QFile::exists("/SoftBoot/SoftUpdate"))
        _AppEndExe.cmd = "/SoftBoot/SoftUpdate &";
    else if (QFile::exists("./SoftUpdate"))
        _AppEndExe.cmd = "./SoftUpdate &";
    qApp->quit();
    #endif
}

void MTools::softwareRestart()
{
    #ifdef __ARM_EABI__
    _AppEndExe.cmd = "./MainProgram &";
    qApp->quit();
    #endif
}

void MTools::setLastNetUp(const QString &text)
{
    setIniValue("lastNetUp", text);
}

QString MTools::lastNetUp()
{
    QSettings settings("MainProgram.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    return settings.value("lastNetUp").toString();
}

#ifdef QT_NETWORK_LIB
void MTools::initNetConfig()
{
    QSettings settings("MainProgram.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
#ifdef __ARM_EABI__
    system("ifconfig lo down; ifconfig lo up;");
    system("ifconfig eth0 down; ifconfig eth0 up;");

    // 修改IP地址, MAC地址, 默认网关
    m_ip = settings.value("NET/IP", "192.168.1.111").toString();
    m_nm = settings.value("NET/NETMASK", "255.255.255.0").toString();
    m_gw = settings.value("NET/GW").toString();
    QHostAddress ipaddr(m_ip);
    qsrand(QDateTime::currentMSecsSinceEpoch() +
           ((quint64)ipaddr.toIPv4Address() << 32));

    QString cmd_ip = QString("ifconfig eth0 %1 netmask %2").arg(m_ip, m_nm);
    QString cmd_route = m_gw.isEmpty() ? 0 : QString("route add default gw %1").arg(m_gw);
    QString cmd_mac = QString("ifconfig eth0 hw ether 00:%1:%2:%3:%4:%5")
            .arg(qrand()%0xFF,2,16,QChar('0'))
            .arg(qrand()%0xFF,2,16,QChar('0'))
            .arg(qrand()%0xFF,2,16,QChar('0'))
            .arg(qrand()%0xFF,2,16,QChar('0'))
            .arg(qrand()%0xFF,2,16,QChar('0'));

    system(cmd_ip.toLatin1().constData());
    system(cmd_mac.toLatin1().constData());
    system("ifconfig eth0 down; ifconfig eth0 up;");
    system("route del default 2> /dev/null");
    system(cmd_route.toLatin1().constData());
#endif
    m_server_ip = settings.value("NET/SERVER_IP", "192.168.1.222").toString();
    m_server_port = settings.value("NET/SERVER_PORT", 502).toInt();
    m_server_port2 = settings.value("NET/SERVER_PORT2", 5020).toInt();
    m_net_mode = settings.value("NET/NET_MODE", 2).toInt();
    m_isOpenWifi = settings.value("NET/IsOpenWifi").toBool();
    m_isOnline = settings.value("NET/IsOnline").toBool();
}

bool MTools::setIP(const QString &str)
{
    QHostAddress addr(str);
    if (addr.isNull())
        return false;
#ifdef __ARM_EABI__
    QString cmd = QString("ifconfig eth0 %1").arg(str);
    if (system(cmd.toLatin1().constData()))
        return false;

    system("ifconfig eth0 down; ifconfig eth0 up;");
    system("route del default 2> /dev/null");
    if (!m_gw.isEmpty()) {
        QString cmd = QString("route add default gw %1").arg(m_gw);
        system(cmd.toLatin1().constData());
    }

    setIniValue("NET/IP", str);
#endif
    m_ip = str;
    return true;
}

bool MTools::setNM(const QString &str)
{
    QHostAddress addr(str);
    if (addr.isNull())
        return false;
#ifdef __ARM_EABI__
    QString cmd = QString("ifconfig eth0 netmask %1").arg(str);
    if (system(cmd.toLatin1().constData()))
        return false;

    system("ifconfig eth0 down; ifconfig eth0 up;");
    system("route del default 2> /dev/null");
    if (!m_gw.isEmpty()) {
        QString cmd = QString("route add default gw %1").arg(m_gw);
        system(cmd.toLatin1().constData());
    }

    setIniValue("NET/NETMASK", str);
#endif
    m_nm = str;
    return true;
}

bool MTools::setGW(const QString &str)
{
    QHostAddress addr(str);
    if (addr.isNull() && !str.isEmpty())
        return false;
#ifdef __ARM_EABI__
    if (str == m_gw)
        return true;

    system("route del default 2> /dev/null");
    if (!str.isEmpty()) {
        QString cmd = QString("route add default gw %1").arg(str);
        system(cmd.toLatin1().constData());
    }

    setIniValue("NET/GW", str);
#endif
    m_gw = str;
    return true;
}

void MTools::setServerIp(const QString &str)
{
    setIniValue("NET/SERVER_IP", str);
    m_server_ip = str;
}

void MTools::setServerPort(int i)
{
    setIniValue("NET/SERVER_PORT", i);
    m_server_port = i;
}

void MTools::setServerPort2(int i)
{
    setIniValue("NET/SERVER_PORT2", i);
    m_server_port2 = i;
}

void MTools::setNetMode(int i)
{
    setIniValue("NET/NET_MODE", i);
    m_net_mode = i;
}
#else
void MTools::initNetConfig() {}
bool MTools::setIP(const QString &str) { Q_UNUSED(str); return false; }
bool MTools::setNM(const QString &str) { Q_UNUSED(str); return false; }
bool MTools::setGW(const QString &str) { Q_UNUSED(str); return false; }
#endif

void MTools::setIsOpenWifi(bool b)
{
    setIniValue("NET/IsOpenWifi", b);
    m_isOpenWifi = b;
}

void MTools::setIsOnline(bool b)
{
    setIniValue("NET/IsOnline", b);
    m_isOnline = b;
}

bool MTools::checkDiskSize(int m)
{
    QProcess p;
    p.start("df", QStringList() << "-m" << "/");
    p.waitForFinished();
    QString str = QString::fromUtf8(p.readAllStandardOutput());
    QStringList list = str.split('\n');

    for (int i = 0; i < list.size(); i++) {
        if (list.at(i).endsWith('/')) {
            QStringList list0 = list.at(i).split(QRegExp(" +"));
            if (list0.size() > 3)
                return list0.at(3).toInt() >= m;
        }
    }
    return false;
}

bool MTools::eventFilter(QObject *, QEvent *ev)
{
    if(isStartBLTime && (
                ev->type() == QEvent::MouseButtonPress ||
                ev->type() == QEvent::KeyPress)){
        MTools::lasttsms = QDateTime::currentMSecsSinceEpoch();
        if(isBLOn == false){
            blOn();
            #ifdef __ARM_EABI__
            return true;
            #endif
        }
    }
    return false;
}

void MTools::timerEvent(QTimerEvent *ev)
{
    if(ev->timerId() == timerid){
        QDateTime dt = QDateTime::currentDateTime();
        curms = dt.toMSecsSinceEpoch();
        if(isStartDateTime)
            instance()->timeChange(dt);
        if(isStartBLTime && bltime() > 0 && isBLOn && curms - lasttsms >= bltime() * 60000) {
            blOff();
        }
    }
}

MTools_AppEndExe::MTools_AppEndExe()
{
}

MTools_AppEndExe::~MTools_AppEndExe()
{
    if (!cmd.isEmpty())
        system(cmd.constData());
}
