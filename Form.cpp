#include "Form.h"
#include "ui_Form.h"
#include "MachineComm.h"
#include "MoudbusVirtualSlave.h"
#include "ModbusTcpServer.h"
#include "JsonTcpServer.h"
#include "JServer_Mac.h"
#include "JServer_NetUp.h"
#include "JServer_Rec.h"
#include "TcpComm.h"
#include "WiFiCheckThread.h"
#include "Language.h"
#include "FormA.h"
#include "Thread.h"
#include "MTools.h"
#include "UIDebug.h"
#include <QtSerialPort>
#include <QPainter>
#include <QSettings>
#include <QThread>
#include <QStyleOption>
#include <QStyleFactory>

#ifndef MYOPT_OnlineManager

// 是否显示启动界面
#define SHOW_STARTUP
#ifdef SHOW_STARTUP
#include "DialogStartup.h"
#endif

// 是否使用Moudbus虚拟从机
//#define USE_MoudbusVirtualSlave

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    QTimer::singleShot(1000, this, SLOT(update()));
    Language::init();

    ui->setupUi(this);
    setFixedSize(size());
    move(0, 0);
    setAttribute(Qt::WA_StyledBackground, true);
    #ifdef __ARM_EABI__
    setWindowFlags(Qt::FramelessWindowHint);
    #endif

    // 初始化网络配置
    MTools::initNetConfig();
    if (MTools::netMode() == 0)
        MTools::setIsOpenWifi(false);
    MTools::setIsOnline(MTools::netMode() == 1);

    // 启用界面调试
    uidebug = new UIDebug(this);

    // 开始背光计时
    #ifndef MYOPT_WINDOW
    MTools::startBLTime();
    #endif

    // 使用本地时间
    #ifdef MYOPT_LocalTime
    MTools::startDateTime();
    #endif

    // 开机默认权限
    #ifdef MYOPT_TEST
    MTools::setAuth(3);
    #endif

    #ifdef SHOW_STARTUP
    // 显示启动界面
    //QString simgurl = MTools::getStartupImageUrl();
    DialogStartup *startup0 = new DialogStartup(2, this);
    startup0->setFixedSize(this->size());
    startup0->setWindowFlags(Qt::FramelessWindowHint);
    //startup0->setStyleSheet(QString("background-image: url(%1)").arg(simgurl));
    startup0->move(0, 0);
    Thread *thdLang = new Thread(startup0);
    Thread *thdWifi = new Thread(startup0);
    connect(thdLang, SIGNAL(finished()), startup0, SLOT(progress()));
    connect(thdWifi, SIGNAL(finished()), startup0, SLOT(progress()));
    connect(startup0, SIGNAL(processStarted()), this, SLOT(onInitStarted()));
    connect(startup0, SIGNAL(processFinished()), this, SLOT(onInitFinished()));
    connect(startup0, SIGNAL(processFinished0()), this, SLOT(onInitFinished0()));
    connect(startup0, SIGNAL(processStarted()), thdWifi, SLOT(initWifi()));
    langThd = thdLang;
    mcrtuThd = 0;
    mtcpsThd = 0;
    jtcpsThd = 0;
    tcpCommThd = 0;
    startup = startup0;
    startup->show();
    #else
    // 无启动界面
    onInitStarted();
    Thread *thdLang = new Thread();
    Thread *thdWifi = new Thread();
    langThd = thdLang;
    QImagesManager::def.invertSetEnabled = false;
    thdLang->loadLang();
    thdWifi->initWifi();
    thdLang->wait();
    thdWifi->wait();
    onInitFinished0();
    onInitFinished();
    #endif
}

Form::~Form()
{
    jsonTcpTool.interruptAll = true;
    if (mcrtuThd) {
        delete form;
        #ifdef QT_NETWORK_LIB
        mtcpsThd->quit();
        jtcpsThd->quit();
        tcpCommThd->quit();
        mtcpsThd->wait();
        jtcpsThd->wait();
        tcpCommThd->wait();
        #endif
        delete comm;
        mcrtuThd->quit();
        mcrtuThd->wait();
    }
    delete ui;
}

void Form::onInitStarted()
{
    // 创建串口对象
#ifndef USE_MoudbusVirtualSlave
    QSettings settings("serialPort.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    #ifdef __ARM_EABI__
    QString sp0Name = settings.value("portName", "/dev/ttyS7").toString(); //ttyS7,ttyS3
    #elif unix
    QString sp0Name = "/dev/ttyS1";
    #endif
    #ifdef unix
    QSerialPort *io;
    io = new QSerialPort(sp0Name);
    if (io->open(QIODevice::ReadWrite))
    {
        io->flush();
        io->setBaudRate((QSerialPort::BaudRate)settings.value("baudRate", QSerialPort::Baud115200).toInt());
        io->setDataBits((QSerialPort::DataBits)settings.value("dataBits", QSerialPort::Data8).toInt());
        io->setParity((QSerialPort::Parity)settings.value("parity", QSerialPort::NoParity).toInt());
        io->setStopBits((QSerialPort::StopBits)settings.value("stopBits", QSerialPort::TwoStop).toInt());
        io->setFlowControl(QSerialPort::NoFlowControl);
    } else
        qWarning("SerialPort open err");
    #else
    MoudbusVirtualSlave *io = new MoudbusVirtualSlave(this);
    io->open(QIODevice::ReadWrite);
    #endif
#else
    MoudbusVirtualSlave *io = new MoudbusVirtualSlave(this);
    io->open(QIODevice::ReadWrite);
#endif

    // 创建机器对象
    MachineCommRTU *mcrtu = new MachineCommRTU(io);
    mcrtuThd = new QThread(this);
    mcrtuThd->start();
    mcrtu->moveToThread(mcrtuThd);
    connect(mcrtuThd, SIGNAL(finished()), mcrtu, SLOT(deleteLater()));
    comm = new MachineCommQueue(mcrtu);

    // 创建ModbusTcpServer
    ModbusTcpServer *mtcps = 0;
    ModbusTcpServerC *mtcpsc = 0;
    #ifdef QT_NETWORK_LIB
    mtcpsThd = new QThread(this);
    mtcpsThd->start();
    mtcps = new ModbusTcpServer(502, comm);
    mtcps->moveToThread(mtcpsThd);
    connect(mtcpsThd, SIGNAL(finished()), mtcps, SLOT(deleteLater()));
    mtcpsc = new ModbusTcpServerC(MTools::serverIp(), MTools::serverPort(), comm);
    mtcpsc->moveToThread(mtcpsThd);
    connect(mtcpsThd, SIGNAL(finished()), mtcpsc, SLOT(deleteLater()));
    QByteArray uuid = QUuid::createUuid().toByteArray();
    mtcps->uuid = mtcpsc->uuid = uuid;
    #endif

    // 创建JsonTcpServer
    JsonTcpServer *jtcps = 0;
    JsonTcpServerC *jtcpsc = 0;
    #ifdef QT_NETWORK_LIB
    jtcpsThd = new QThread(this);
    jtcpsThd->start();
    jtcps = new JsonTcpServer(5020);
    jtcps->moveToThread(jtcpsThd);
    connect(jtcpsThd, SIGNAL(finished()), jtcps, SLOT(deleteLater()));
    jtcpsc = new JsonTcpServerC(MTools::serverIp(), MTools::serverPort2());
    jtcpsc->moveToThread(jtcpsThd);
    connect(jtcpsThd, SIGNAL(finished()), jtcpsc, SLOT(deleteLater()));
    #endif

    // 创建数据库
    RecDB *recDB = new RecDB("rec.db");

    // 创建Server
    JServer_Mac *sv_mac = 0;
    JServer_NetUp *sv_netup = 0;
    JServer_Rec *sv_rec = 0;
    #ifdef QT_NETWORK_LIB
    JsonTcpServer_FuncMap jts_fm;
    sv_mac = new JServer_Mac(&jts_fm, this);
    sv_netup = new JServer_NetUp(&jts_fm, this);
    sv_rec = new JServer_Rec(&jts_fm, recDB, this);
    jtcps->setFuncMap(jts_fm);
    jtcpsc->setFuncMap(jts_fm);
    sv_mac->uuid = uuid;
    #endif

    // 创建TcpComm
    TcpComm *tcpComm = 0;
    #ifdef QT_NETWORK_LIB
    tcpCommThd = new QThread(this);
    tcpCommThd->start();
    tcpComm = new TcpComm(15020);
    tcpComm->moveToThread(tcpCommThd);
    connect(tcpCommThd, SIGNAL(finished()), tcpComm, SLOT(deleteLater()));
    #endif

    QVariantMap vmap;
    QVariant var;
    var.setValue<void*>(mtcps); vmap.insert("mtcps", var);
    var.setValue<void*>(mtcpsc); vmap.insert("mtcpsc", var);
    var.setValue<void*>(jtcps); vmap.insert("jtcps", var);
    var.setValue<void*>(jtcpsc); vmap.insert("jtcpsc", var);
    var.setValue<void*>(sv_mac); vmap.insert("sv_mac", var);
    var.setValue<void*>(sv_netup); vmap.insert("sv_netup", var);
    var.setValue<void*>(sv_rec); vmap.insert("sv_rec", var);
    var.setValue<void*>(tcpComm); vmap.insert("tcpComm", var);
    var.setValue<void*>(recDB); vmap.insert("recDB", var);

    FormA *forma = new FormA(1, comm, vmap);
    form = forma;
    #ifdef SHOW_STARTUP
    form->hide();
    #endif

    #ifdef QT_NETWORK_LIB
    tcpComm->setIsOnline(true);
    #endif

    #ifdef __ARM_EABI__
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    #endif

    #ifdef SHOW_STARTUP
    QImagesManager::def.invertSetEnabled = false;
    ((Thread*)langThd)->loadLang();
    #endif

    /*
    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 5020);
    jsonTcpTool.waitForConnected(&socket);
    QByteArray data;
    jsonTcpTool.waitForReadJson(&socket, data);
    jsonTcpTool.writeJsonReq(&socket, jtcps->secretKey,
                              QString::fromUtf8(data.constData()), "",
                              "net_up");
    data.clear();
    jsonTcpTool.waitForReadJson(&socket, data);
    jsonTcpTool.writeArray(&socket, "123.up");
    jsonTcpTool.writeArray(&socket, "data");
    QByteArray fileMd5;
    Server_NetUp::makeFileMd5( "123.up", "data", g_st->myRandom, g_st->key, fileMd5);
    jsonTcpTool.writeArray(&socket, fileMd5);
    data.clear();
    jsonTcpTool.waitForReadJson(&socket, data);
    jsonTcpTool.waitForDisconnected(&socket);
    */
}

void Form::onInitFinished()
{
    // 使用加载的语言图片
    QImagesManager::def.replace(((Thread*)langThd)->lang_imgs);
    langThd->deleteLater();

    // 关闭启动界面显示主界面
    #ifdef SHOW_STARTUP
    startup->deleteLater();
    #endif
    form->setParent(this);
    form->show();
    uidebug->raise();
    QTimer::singleShot(2000, this, SLOT(onSelfCheck()));
}

void Form::onInitFinished0()
{
    // 加载样式表
    MTools::loadQSS(this);
}

void Form::onSelfCheck()
{
#ifdef __ARM_EABI__
    if (!MTools::checkDiskSize(100)) {
        DlgInfo dlg(this);
        dlg.setText("There is insufficient storage space");
        dlg.exec();
    }
#endif
}

#else
Form::Form(QWidget *parent,
           const QString &ip,
           const QVariantMap &port,
           const QString &wd,
           const QString &language,
           const QString &dlldir) :
    QWidget(parent),
    ui(new Ui::Form)
{
    this->setProperty("ip", ip);
    this->setProperty("port", port);
    this->setProperty("wd", wd);
    if (!dlldir.isEmpty() &&
            dlldir.at(dlldir.size() - 1) != '/' &&
            dlldir.at(dlldir.size() - 1) != '\\')
        MyDll_Dir = dlldir + '/';
    else
        MyDll_Dir = dlldir;
    QImagesManager::resourceRoot = MyDll_Dir;

    ui->setupUi(this);
    setFixedSize(size());

    // 使用本地时间
    #ifdef MYOPT_LocalTime
    MTools::startDateTime();
    #endif
    // 开机默认权限
    #ifndef MYOPT_TEST
    MTools::setAuth(3);
    #endif

    langThd = 0;
    static bool isLoad = false;
    if (!isLoad) {
        isLoad = true;
        QSettings langset(MyDll_Dir+"language.ini", QSettings::IniFormat);
        langset.setIniCodec("utf-8");
        QVariant langNum = langset.value(QString("Number/%1").arg(language));
        if (langNum.canConvert(QVariant::Int))
            MTools::changeLangId(langNum.toInt());
        Thread *thread = new Thread();
        langThd = thread;
        QImagesManager::def.invertSetEnabled = false;
        thread->loadLang();
        thread->wait();
    }
    onInitStarted();
    onInitFinished();
}

Form::~Form()
{
    if (mctcpThd) {
        delete form;
        delete comm;
        mctcpThd->quit();
        mctcpThd->wait();
    }
    delete ui;
}

void Form::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void Form::onInitStarted()
{
    // 创建通信对象
    QTcpSocket *s = new QTcpSocket;
    QString ip = property("ip").toString();
    uint port = property("port").toMap()
            .value("tcpPort").toMap()
            .value("502", 502).toUInt();
    s->setProperty("ip", ip);
    s->setProperty("port", port);

    // 创建机器对象
    MachineCommTcp *mctcp = new MachineCommTcp(s);
    connect(mctcp, SIGNAL(socketConnected()),
            this, SLOT(onSocketConnected()));
    connect(mctcp, SIGNAL(socketDisconnected()),
            this, SLOT(onSocketDisconnected()));
    mctcpThd = new QThread(this);
    mctcpThd->start();
    mctcp->moveToThread(mctcpThd);
    connect(mctcpThd, SIGNAL(finished()), mctcp, SLOT(deleteLater()));
    comm = new MachineCommQueue(mctcp);

    FormA *forma = new FormA(1, comm);
    form = forma;
    static QWindowsStyle *ws = new QWindowsStyle;
    form->setStyle(ws);
}

void Form::onInitFinished()
{
    // 加载样式表
    MTools::loadQSS(this);

    // 使用加载的语言图片
    if (langThd) {
        QImagesManager::def.replace(((Thread*)langThd)->lang_imgs);
        langThd->deleteLater();
    }

    form->setParent(this);
    form->show();
}

void Form::onSocketConnected()
{
    setProperty("val_connected", true);
    emit valChanged();
}

void Form::onSocketDisconnected()
{
    setProperty("val_connected", false);
    emit valChanged();
}
#endif
