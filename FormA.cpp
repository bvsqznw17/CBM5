#include "FormA.h"
#include "ui_FormA.h"
#include "Tool.h"
#include "MTools.h"
#include "Doum.h"
#include "JsonTcpServer.h"
#include "JServer_NetUp.h"
#include "RecSave.h"
#include "DlgWaiting.h"

FormA::FormA(int maddr, MachineComm *comm,
             const QVariantMap &vmap_,
             QWidget *parent) :
    QWidget(parent),
    mac(maddr, comm, &mp),
    ui(new Ui::FormA)
{
    // 每个页面定时读取参数的间隔时间
    mpri = 500;
    form = this;

    // 斗类型，斗图片
    doum = new Doum;
    doum->init(1, 14);
    dn = doum->dn;
    gn = doum->gn;
    dng = doum->dng;

    // 电脑上测试时，初始化一些参数
#ifdef MYOPT_TEST
    //MP.cmd->setValue(1);
    MP.sys_prm_ids[0]->setValue(1);
    MAC.write(MP.sys_prm_ids[0], 1);
    MP.prm_id->setValue(1);
    MAC.write(MP.prm_id, 1);
    MP.cmb_AlarmDOu[0]->setValue(0x0FFF);
    MAC.write(MP.cmb_AlarmDOu[0], 0x0FFF);
    MP.cmb_RunStatus[0]->setValue(0x0020);
    MAC.write(MP.cmb_RunStatus[0], 0x0020);
    MP.cmb_DouStatus[0]->setValue("\xb\xb");
    MAC.write(MP.cmb_DouStatus[0], "\xb\xb");
    MP.cmb_Weight[0]->setValue(123);
    MAC.write(MP.cmb_Weight[0], 123);
    MP.prm_DWW->setValue(10);
    MAC.write(MP.prm_DWW, 10);
    MP.doustatus[0]->setValue(0xbbbb);
    MAC.write(MP.doustatus[0], 0xbbbb);
    MP.doustatus[12]->setValue(0xbbbb);
    MAC.write(MP.doustatus[12], 0xbbbb);
    for (int i = 0; i < 10; i++) {
        MP.sys_DOU_State[i]->setValue(1);
        MAC.write(MP.sys_DOU_State[i], 1);
    }

    MP.doustatus[14]->setValue(-1);
#endif

    // 提取Map传过来的值
    vmap = vmap_;
#ifndef MYOPT_OnlineManager
    mtcps = (ModbusTcpServer*)(vmap.value("mtcps", 0).value<void*>());
    mtcpsc = (ModbusTcpServerC*)(vmap.value("mtcpsc", 0).value<void*>());
    jtcps = (JsonTcpServer*)(vmap.value("jtcps", 0).value<void*>());
    jtcpsc = (JsonTcpServerC*)(vmap.value("jtcpsc", 0).value<void*>());
    sv_mac = (JServer_Mac*)(vmap.value("sv_mac", 0).value<void*>());
    sv_netup = (JServer_NetUp*)(vmap.value("sv_netup", 0).value<void*>());
    sv_rec = (JServer_Rec*)(vmap.value("sv_rec", 0).value<void*>());
    tcpComm = (TcpComm*)(vmap.value("tcpComm", 0).value<void*>());
    recDB = (RecDB*)(vmap.value("recDB", 0).value<void*>());
#else
    recDB = (RecDB*)(vmap.value("recDB", 0).value<void*>());
#endif

    // 在另一个线程里定时读取并保存记录，同时读取了运行参数，
    // 所以主线程不需要再读运行参数，通过此信号槽共享即可
    recs = new RecSave(this, recDB);
    connect(recs, SIGNAL(readFns(int,QByteArray)),
            this, SLOT(onRecReadFns(int,QByteArray)));

    // 摄像头管理
//    cam_t1 = new CameraThread();
//    cam_t2 = new CameraThread();
    cam_t1->dev = "/dev/video0";
    cam_t2->dev = "/dev/video1";
//    cam_c1 = new CameraCalib();
//    cam_c2 = new CameraCalib();
    cam_c1->cam = cam_t1;
    cam_c2->cam = cam_t2;
//    CameraCalib::loadPoints(cam_pts1, cam_pts2, cam_ptsc1, cam_ptsc2);

    // 键盘管理
    keybm = new KeyboardManager(&mac, this);

    // 发送时间更新信号
    connect(MTools::instance(), SIGNAL(timeChange(QDateTime)),
            this, SLOT(onDtChanged()));

    // 等待命令结束需要的
    isReadCmd = false;
    waitCmdCnt = 0;
    waitCmdDlg = 0;
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);

    // 初始化UI
    ui->setupUi(this);
    setFixedSize(size());
    init();

    // 最后再启动这个线程
    recs->thd.start();

//    qDebug() << MMP.Sys.size();
//    qDebug() << MMP.Prm.size();
//    qDebug() << MMP.Motor.size();
//    qDebug() << MMP.PrmList.size();
//    qDebug() << MMP.Mod.size();
//    qDebug() << MMP.PrmList.at(0).addr << MMP.PrmList.at(0).count;
//    qDebug() << MMP.PrmList.at(1).addr << MMP.PrmList.at(1).count;

}

FormA::~FormA()
{
    foreach (const QObject *obj, children())
        delete obj;
    delete recs;
    delete recDB;
    delete ui;
    form->mac.echom.destroy(ecIds, this);
}

void FormA::init()
{
//    mptrw = new MPTRWidget(this, &form->mac, MMP.Main, 50);

    ui->page->setCurrentWidget(ui->page_Main);
    ui->page_Main->init(form);
    ui->page_Menu->init(form);
    ui->page_Help->init(form);
    ui->page_Samp->init(form);
    ui->page_KaiMen->init(form);
    ui->page_ZhenDong->init(form);
//    ui->page_CameraCalib->init(form);
}

void FormA::showWidget(QString name)
{
    backPageName = ui->page->currentWidget()->objectName();
    ui->page->setCurrentWidget(findChild<QWidget*>(name));
}

void FormA::backPage()
{
    ui->page->setCurrentWidget(findChild<QWidget*>(backPageName));
}

void FormA::showSamp(int dounum)
{
    ui->page_Samp->dounum = dounum;
    showWidget("page_Samp");
}

void FormA::showKaiMen(int dounum)
{
    ui->page_KaiMen->dounum = dounum;
    showWidget("page_KaiMen");
}

void FormA::showZhenDong(int dounum)
{
    ui->page_ZhenDong->dounum = dounum;
    showWidget("page_ZhenDong");
}

QDateTime FormA::dt()
{
    return Tool::currentDateTime();
}

void FormA::setDT(const QDateTime &dt)
{
    if (dt.isValid())
        Tool::setDateTime(dt);
}

void FormA::onDtChanged()
{
    emit dtChanged(Tool::currentDateTime());
}

#include <QLineEdit>

void FormA::connectMPEdit(QLineEdit *edit, const QStringList &trs)
{
    connect(edit, SIGNAL(editingFinished()), this, SLOT(onMPEditClicked()));
    edit->setProperty("trs", trs);
    edit->setReadOnly(true);
}

void FormA::refreshMPEdit(QLineEdit *edit, const QStringList &trs)
{
    if (edit->isReadOnly()) {
        int i = form->keybm->indexOf(edit);
        uint v = form->keybm->mp(i)->valuenp().toUInt();
        if (v < (uint)trs.size())
            edit->setText(trs.at(v));
    }
}

void FormA::onMPEditClicked()
{
    QObject *edit = sender();
    QStringList list = edit->property("trs").toStringList();
    int i = form->keybm->indexOf(edit);
    const KeyboardManager::Group &g = form->keybm->group(i);
    DlgList dlg(form);
    dlg.setWindowTitle(g.title);
    dlg.setHelpText(g.help);
    dlg.setList(list);
    if (dlg.exec() == QDialog::Accepted) {
        int v = dlg.currenti();
        MAC.write(g.mp, v);
        MAC.read(g.mp);
    }
}

void FormA::onRecReadFns(int addr, QByteArray ba)
{
    MRegAreas ras(addr, ba.size()/2);
    mp.setData(ras, QList<QByteArray>() << ba);
    QMetaObject::invokeMethod(&mac, "regUpdated",
                              Q_ARG(MRegAreas, ras));
}

void FormA::onRegUpdated(const MRegAreas &areas)
{
    Q_UNUSED(areas)
    if (!isReadCmd && waitCmdDlg && !MP.cmd->value().toUInt()) {
        if (waitCmdCnt == 0)
            waitCmdDlg->accept();
        else {
            emit cmdFns(waitCmdCnt--);
            isReadCmd = true;
            MAC.read(MP.cmd, echocmd);
        }
    }
}

void FormA::onReadCmdFns(int , const SignalEcho_zy1 &echo)
{
    MAC.echom.finish(echo);
    if (echo == echocmd) {
        isReadCmd = false;
        if (!isReadCmd && waitCmdDlg && !MP.cmd->value().toUInt()) {
            if (waitCmdCnt == 0)
                waitCmdDlg->accept();
            else {
                emit cmdFns(waitCmdCnt--);
                isReadCmd = true;
                MAC.read(MP.cmd, echocmd);
            }
        }
    }
}

void FormA::writeCMD(quint16 cmd, quint16 cmdp)
{
    QList<const MParam *>mps;
    QVariantList vals;
    mps.reserve(2);
    mps.append(MP.cmd_param);
    mps.append(MP.cmd);
    vals.reserve(2);
    vals.append(cmdp);
    vals.append(cmd);
    mac.write(mps, vals);
}

int FormA::waitCmdFns(int ms, int maxms, bool hide)
{
#ifdef MYOPT_TEST
    ms = maxms = 1000;
#endif
    if (!echocmd.isValid())
        form->mac.echom.create(echocmd, ecIds, this, "onReadCmdFns");
    isReadCmd = true;
    MAC.read(MP.cmd, echocmd);
    if (!hide)
        waitCmdDlg = new DlgWaiting(form, ms, maxms);
    else {
        waitCmdDlg = new QDialog();
        waitCmdDlg->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
        waitCmdDlg->setFixedSize(1, 1);
        QTimer::singleShot(3000, waitCmdDlg, SLOT(reject()));
    }
    int r = waitCmdDlg->exec();
    delete waitCmdDlg;
    waitCmdDlg = 0;
    waitCmdCnt = 0;
    form->mac.clearAction(echocmd, ecIds);
    isReadCmd = false;
    return r;
}

bool FormA::isRun()
{
    for (int i = 0; i < gn; i++)
        if (MP.cmb_RunStatus[i]->value().toInt() & 0x01)
            return true;
    return false;
}

bool FormA::isRun(int i)
{
    return MP.cmb_RunStatus[i]->value().toInt() & 0x01;
}

bool FormA::isClean()
{
    uint flag = MP.sys_bQXFlag->value().toUInt();
    return (flag & 0xFFF0) == 0xAAA0;
}

bool FormA::isManualZero()
{
    for (int i = 0; i < gn; i++)
        if (MP.cmb_RunStatus[i]->value().toInt() & 0x0100)
            return true;
    return false;
}

bool FormA::isManualEmpty()
{
    for (int i = 0; i < gn; i++)
        if (MP.cmb_RunStatus[i]->value().toInt() & 0x0200)
            return true;
    return false;
}

bool FormA::isManualClean()
{
    for (int i = 0; i < gn; i++)
        if (MP.cmb_RunStatus[i]->value().toInt() & 0x0400)
            return true;
    return false;
}

bool FormA::isHaveAlarm(int i)
{
    int g = i / dng;
    int n = i % dng;
    uint a = MP.cmb_AlarmDOu[g]->value().toUInt();
    return (a >> n) & 0x01;
}

bool FormA::isHaveAlarm()
{
    for (int i = 0; i < gn; i++)
        if (MP.cmb_AlarmDOu[i]->value().toInt())
            return true;
    return false;
}

bool FormA::isLackMaterial()
{
    for (int i = 0; i < gn; i++)
        if (MP.cmb_RunStatus[i]->value().toInt() & 0x40)
            return true;
    return false;
}

bool FormA::isLackMaterialEnable()
{
    for (int i = 0; i < gn; i++)
        if (MP.cmb_RunStatus[i]->value().toInt() & 0x20)
            return true;
    return false;
}

bool FormA::IsLackMaterialDisable()
{
    return MP.prm_ZZJ_QLTJ->value().toInt() == 0;
}
