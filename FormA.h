#ifndef FORMA_H
#define FORMA_H

#define MAC         (form->mac)
#define MP          (form->mp.m)
#define MMP         (form->mp.mm)

#include <QWidget>
#include <QVariant>
#include <QDateTime>
#include <QDebug>
#include "MachineParamList0.h"
#include "Machine.h"
#include "MPTReader.h"
class ModbusTcpServer;
class ModbusTcpServerC;
class JsonTcpServer;
class JsonTcpServerC;
class JServer_Mac;
class JServer_NetUp;
class JServer_Rec;
class TcpComm;
class RecSave;
class RecDB;

#include "KeyboardManager.h"
#include "CameraJoint.h"
#include "DlgInfo.h"
#include "DlgOKCancel.h"
#include "DlgList.h"
#include "DlgProgress.h"
#include "QLabel2.h"
#include <QButtonGroup>
class QPushButton;
class QCheckBox;
class QLineEdit;
class QLabel;

namespace Ui {
class FormA;
}

class FormA : public QWidget
{
    Q_OBJECT

public:
    int mpri;
    MachineParamList0 mp;
    Machine mac;
    QVariantMap vmap;
    ModbusTcpServer *mtcps;
    ModbusTcpServerC *mtcpsc;
    JsonTcpServer *jtcps;
    JsonTcpServerC *jtcpsc;
    JServer_Mac *sv_mac;
    JServer_NetUp *sv_netup;
    JServer_Rec *sv_rec;
    TcpComm *tcpComm;
    RecSave *recs;
    RecDB *recDB;

    CameraThread *cam_t1;
    CameraThread *cam_t2;
    CameraCalib *cam_c1;
    CameraCalib *cam_c2;
    QVector<float> cam_pts1;
    QVector<float> cam_pts2;
    QVector<float> cam_ptsc1;
    QVector<float> cam_ptsc2;
    Q_SIGNAL void cameraCalibrated();

    class Doum *doum;
    KeyboardManager *keybm;

    explicit FormA(int maddr, MachineComm *comm,
                   const QVariantMap &vmap_ = QVariantMap(),
                   QWidget *parent = 0);
    ~FormA();

private:
    Ui::FormA *ui;
    QString backPageName;

    FormA *form;
    MPTRWidget *mptrw;

    void init();

public slots:
    void showWidget(QString name);
    void backPage();
    void showMain() { showWidget("page_Main"); }
    void showMenu() { showWidget("page_Menu"); }
    void showHelp() { showWidget("page_Help"); }
    void showCameraCalib() { showWidget("page_CameraCalib"); }
    void showSamp(int dounum);
    void showKaiMen(int dounum);
    void showZhenDong(int dounum);

public:
    QDateTime dt();
    void setDT(const QDateTime &dt);
    Q_SLOT void onDtChanged();
    Q_SIGNAL void dtChanged(const QDateTime &dt);

    void connectMPEdit(QLineEdit *edit, const QStringList &trs);
    void refreshMPEdit(QLineEdit *edit, const QStringList &trs);
    Q_SLOT void onMPEditClicked();

private slots:
    void onRecReadFns(int addr, QByteArray ba);
    void onRegUpdated(const MRegAreas &areas);
    void onReadCmdFns(int err, const SignalEcho_zy1 &echo);

public:
    void writeCMD(quint16 cmd, quint16 cmdp);
    inline void writeCMD(quint16 cmdH, quint16 cmdL, quint16 cmdpH, quint16 cmdpL)
    { writeCMD((cmdH<<8)|(cmdL), (cmdpH<<8)|(cmdpL)); }

    bool isReadCmd;
    int waitCmdCnt;
    QDialog *waitCmdDlg;
    int waitCmdFns(int ms, int maxms = -1, bool hide = false);
    int waitCmdFns(int ms, bool hide)
    { return waitCmdFns(ms, -1, hide); }
    Q_SIGNAL void cmdFns(int cnt);

    bool isRun();
    bool isRun(int i);
    bool isClean();
    bool isManualZero();
    bool isManualEmpty();
    bool isManualClean();
    bool isHaveAlarm(int i);
    bool isHaveAlarm();
    bool isLackMaterial();
    bool isLackMaterialEnable();
    bool IsLackMaterialDisable();

    // 总斗数，总组数，每组斗数
    int dn, gn, dng;

    SignalEcho echocmd;
    SignalEchoIds ecIds;
};

#define MAX_D       20  // 最多斗数量
#define MAX_G       1   // 最多斗分组
#define MAX_DG      21  // 斗数量和斗分组之和
#define MAX_DPG     20  // 每组最多斗数量
#define MAX_G1      (MAX_G + 1)         // 斗分组加一
#define MAX_DG1     (MAX_DG + 1)        // 斗数量和斗分组之和加一
#define MAX_DGG1    (MAX_DG + MAX_G1)   // 单层斗**之和
#define MAX_DDG     (MAX_G + MAX_D*2)   // 双层斗**之和

enum ctrlno{CTL_NONE=0,
    //CTL_RESET,
    CTL_RUN                 = 1,    //运行 commandH=0,停止;commandH=2,运行;commandH=4,清除报警
    CTL_MANUAL_ONCE         = 2,    //指定的一列斗运行一次 commandH斗编号
    CTL_MANUAL_CONTINUE     = 3,    //指定的一列斗连续循环运行
    CTL_MANUAL_ZZJ          = 4,    //指定主振机运行一次
    CTL_MANUAL_XZJ          = 5,    //指定的线振机运行一次
    CTL_MANUAL_HCD          = 6,    //指定的进料斗运行一次
    CTL_MANUAL_CZD          = 7,    //指定的称重斗运行一次
    CTL_MANUAL_JYD          = 8,    //指定的记忆斗运行一次
    CTL_MANUAL_JLD          = 9,    //指定集料斗运行一次
    CTL_DOU_PROHIBIT        = 10,   //禁止指定的斗
    CTL_DOU_TEST            = 11,   //称重斗进行采样测试
    CTL_DOU_CALIZERO        = 12,   //传感器置零
    CTL_DOU_CALIFULL        = 13,   //传感器满度
    CTL_SAVE_SYSTEM         = 14,   //保存系统参数
    CTL_SAVE_PROGRAM        = 15,   //保存程序参数
    CTL_MANUAL_ZERO         = 16,   //斗置零
    CTL_MANUAL_EMPTY        = 17,   //斗清空
    CTL_MANUAL_CLEAN        = 18,   //斗清洗
    CTL_MANUAL_STOP         = 19,   //停止当前动作
    CTL_PRMLIST             = 20, 	//读程序列表,CmdPara起始程序号   新增加
    CTL_PRM_WRITE           = 21,   //写程序参数commandH=’A'-'D'，加载当前的程序号;commandH=0,CmdPara=1-94指定程序号 新增加
    CTL_PRM_READ            = 22,   //读程序参数commandH=’A'-'D'，加载当前的程序号;commandH=0,CmdPara=1-99指定程序号
    CTL_PRM_SELECT          = 23,   //选择程序号
    CTL_PRM_DELETE          = 24,   //删除指定的程序号
    CTL_PRM_COPY            = 25,   //复制到指定的程序号
    CTL_MOTOR_SEARCH        = 26,   //查询马达模式
    CTL_MOTOR_SELECT        = 27, 	//选择马达模式
    CTL_MOTOR_COPY          = 28, 	//复制马达模式
    CTL_RECOARD_SEARCH      = 29,	//查询记录
    CTL_RECOARD_CLEAN       = 30, 	//清除记录
    CTL_MOTOR_TEST          = 31,   //马达模式测试
    CTL_MSV                 = 32,   //更新斗状态和斗重量
    CTL_SEZ                 = 33, 	//清零
    CTL_QLTJ                = 34,   //缺料暂停
    CTL_PRMSAVE             = 35,   //参数保存
    CTL_PRM_READ1           = 36,   //参数
    CTL_TEST_IOOUT          = 37,   //端口模拟
    CTL_TEST_DOUKAIMEN      = 38,   //斗开门测试,commandH=斗号,命令参数=角度
    CTL_TEST_ZHENDONG       = 39,   //振动测试
    CTL_READ_MKSTATUS       = 40,   //查询模块状态
    CTL_INTO_PAGE           = 41,   //进入页面
    CTL_IN_TWEIGHT          = 42,   //外部输入目标重量
};

typedef enum tag_E_MotorMode
{
    MM_UNDEFINE     = -1,	//未定义
    MM_BUFFER       = 0x00,		//缓冲斗马达模式
    MM_WEIGHT       = 0x10,		//称重斗马达模式
    MM_JYD          = 0x20,		//记忆斗斗马达模式
    MM_COLLECT      = 0x30,		//集料斗马达模式
    MM_MAIN_VIB     = 0x40		//主振马达模式
}E_MotorMode;

#endif // FORMA_H
