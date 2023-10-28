#include "FormMenuDebug.h"
#include "ui_FormMenuDebug.h"
#include "Doum.h"
#include <QButtonGroup>

#define Fun_Once        0
#define Fun_CONTINUE    1
#define Fun_ZZJ         2
#define Fun_XZJ         3
#define Fun_HCD         4
#define Fun_CZD         5
#define Fun_JYD         6
#define Fun_JLD         7
#define Fun_DouDis      8
#define Fun_DouWeight   9
#define Fun_TestSamp    10
#define Fun_TestKG      11
#define Fun_TestZD      12
#define Fun_Camera      13

#define WAIT_CMD (cmd == CTL_MANUAL_ONCE || cmd == CTL_MANUAL_CONTINUE || cmd == CTL_MANUAL_ZZJ)
#define WAIT_FUN (fun == Fun_Once || fun == Fun_CONTINUE || fun == Fun_ZZJ)

FormMenuDebug::FormMenuDebug(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuDebug)
{
    ui->setupUi(this);

    fun_btg = new QButtonGroup(this);
    int id = 0;
    fun_btg->addButton(ui->pushButton, id++);
    fun_btg->addButton(ui->pushButton_2, id++);
    fun_btg->addButton(ui->pushButton_3, id++);
    fun_btg->addButton(ui->pushButton_4, id++);
    fun_btg->addButton(ui->pushButton_5, id++);
    fun_btg->addButton(ui->pushButton_6, id++);
    fun_btg->addButton(ui->pushButton_14, id++);
    fun_btg->addButton(ui->pushButton_7, id++);
    fun_btg->addButton(ui->pushButton_8, id++);
    fun_btg->addButton(ui->pushButton_9, id++);
    fun_btg->addButton(ui->pushButton_10, id++);
    fun_btg->addButton(ui->pushButton_11, id++);
    fun_btg->addButton(ui->pushButton_12, id++);
    fun_btg->addButton(ui->pushButton_13, id++);
    fun_btg->addButton(ui->pushButton_hide, -1);
    ui->pushButton_hide->hide();
    connect(fun_btg, SIGNAL(buttonClicked(int)), this, SLOT(onFunClicked(int)));
    fun = -1;
    noHide = false;
    isOpen = false;

    QButtonGroup *btg2 = new QButtonGroup(this);
    btg2->addButton(ui->btn_t_1, 0);
    btg2->addButton(ui->btn_t_2, 1);
    ui->btn_t_1->click();

    showCamera(false);
}

FormMenuDebug::~FormMenuDebug()
{
    delete ui;
    form->mac.echom.destroy(ecIds, this);
}

void FormMenuDebug::init(FormA *f)
{
    form = f;
    mptrw = new MPTRWidget(this, &form->mac, MMP.sys_DOU_State, form->mpri);

    ui->page_Camera->init(form);

    QRect dr = ui->dou_widget->geometry();
    int px = dr.x()+(dr.width()-ui->dou_bg->width())/2;
    int py = dr.y()+(dr.height()-ui->dou_bg->height())/2;
    Doum *dm = form->doum;
    ui->dou_bg->setPixmap(dm->bgimg);
    ui->dou_bg->setStyleSheet("background: white;");
    dLab = dm->genDouWidget(this, px, py);
    gLab = dm->genODouWidget(this, px, py);
    dm->setDouMask(dLab);
    dm->setODouMask(gLab);
    dm->setDouNumber(dLab, this);
    dm->setODouNumber(gLab, this);
    dwLab = dm->genDWeightWidget(this, px, py);
    gwLab = dm->genOWeightWidget(this, px, py);
    curseldn = -1;

    for (int i = 0; i < MAX_D; i++) {
        dstatus[i] = 0;
    }
    for (int i = 0; i < MAX_G1; i++) {
        gstatus[i] = 0;
    }

    form->mac.echom.create(echocmd, ecIds, this, "onReadCmdFns");
    form->mac.echom.create(echorw, ecIds, this, "onReadWeightFns");
    isrcmd = 0;
    isrw = false;
    readWeightTimer = new QTimer(this);
    readWeightTimer->setSingleShot(true);
    connect(readWeightTimer, SIGNAL(timeout()), this, SLOT(onReadWeight()));
}

void FormMenuDebug::waitDebugStop()
{
    int cmd = MP.cmd->value().toUInt();
    if ((isrcmd || WAIT_CMD) && WAIT_FUN)
        waitCmdStop();
}

void FormMenuDebug::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
    refreshDouStatus();

    QAbstractButton *fun_bt_TestZD = fun_btg->button(Fun_TestZD);
    if (MP.sys_VibrationSen->value().toInt()) {
        fun_bt_TestZD->setEnabled(true);
    }
    else {
        fun_bt_TestZD->setEnabled(false);
        if (fun_bt_TestZD->isChecked())
            ui->pushButton_hide->click();
    }
    QAbstractButton *fun_bt_JYD = fun_btg->button(Fun_JYD);
    if (MP.sys_QPDGSel->value().toInt()) {
        fun_bt_JYD->setEnabled(true);
    }
    else {
        fun_bt_JYD->setEnabled(false);
        if (fun_bt_JYD->isChecked())
            ui->pushButton_hide->click();
    }
    QAbstractButton *fun_bt_JLD = fun_btg->button(Fun_JLD);
    if (MP.sys_JLD_state->value().toInt()) {
        fun_bt_JLD->setEnabled(true);
    }
    else {
        fun_bt_JLD->setEnabled(false);
        if (fun_bt_JLD->isChecked())
            ui->pushButton_hide->click();
    }
}

void FormMenuDebug::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));

    if (fun == Fun_Camera || fun == Fun_DouWeight)
        if (!noHide)
            ui->pushButton_hide->click();
    noHide = false;
    curseldn = -1;
}


bool FormMenuDebug::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        if (fun < 0)
            return false;
        int cmd = MP.cmd->value().toUInt();
        if ((isrcmd || cmd) && fun != Fun_DouWeight) {
            if (WAIT_CMD && WAIT_FUN)
                waitCmdStop();
            return false;
        }
        QLabel *lab = (QLabel *)watched;
        int n = Doum::getDouNumber(lab);
        if (n > 0 && n <= MAX_D && fun != Fun_JLD && fun != Fun_DouDis &&
                !MP.sys_DOU_State[n]->value().toInt())
            return false;
        if (n > 0 && n <= MAX_D) {
            if (fun != Fun_DouDis && dstatus[n-1] == 15)
                return false;
        }
        else if (n != 0) {
            if (gstatus[n-'A'] == 15)
                return false;
        }

        switch (fun) {
        case Fun_Once:
            form->writeCMD(CTL_MANUAL_ONCE, n);
            break;
        case Fun_CONTINUE:
            form->writeCMD(CTL_MANUAL_CONTINUE, n);
            break;
        case Fun_ZZJ:
            form->writeCMD(CTL_MANUAL_ZZJ, n);
            break;
        case Fun_XZJ:
            form->writeCMD(CTL_MANUAL_XZJ, n);
            break;
        case Fun_HCD:
            form->writeCMD(CTL_MANUAL_HCD, n);
            break;
        case Fun_CZD:
            form->writeCMD(CTL_MANUAL_CZD, n);
            break;
        case Fun_JYD:
            form->writeCMD(CTL_MANUAL_JYD, n);
            break;
        case Fun_JLD:
            form->writeCMD(CTL_MANUAL_JLD, n);
            break;
        case Fun_DouDis:
            form->writeCMD(CTL_DOU_PROHIBIT, n);
            break;
        case Fun_DouWeight:/*
            if (curseldn == n) {
                form->writeCMD(CTL_SEZ, n);
                return false;
            }
            curseldn = n;
            refreshUI();
            refreshDouStatus();*/
            curseldn = n;
            form->writeCMD(CTL_SEZ, n);
            refreshDouStatus();
            return false;
        case Fun_TestSamp:
            form->showSamp(n);
            return false;
        case Fun_TestKG:
            form->showKaiMen(n);
            return false;
        case Fun_TestZD:
            form->showZhenDong(n);
            return false;
        default:
            return false;
        }
        isrcmd++;
        form->mac.read(MP.cmd, echocmd);

        curseldn = n;
        refreshDouStatus();
    }
    return false;
}

void FormMenuDebug::refreshUI()
{
    int doustatuscnt = (form->dn + 1) / 2;
    for (int i = 0; i < dwLab.size(); i++) {
        if (/*i+1 == curseldn && */fun == Fun_DouWeight) {
            int v = MP.doustatus[i + doustatuscnt]->valuenp().toInt();
            if (v != 0x7FFE) {
                dwLab.at(i)->setEnabled(true);
                dwLab.at(i)->setText(MP.doustatus[0]->vPS(v));
            }
            else {
                dwLab.at(i)->setEnabled(false);
                dwLab.at(i)->setText("ERR");
            }
            dwLab.at(i)->show();
        }
        else
            dwLab.at(i)->hide();
    }
    for (int i = 0; i < gwLab.size(); i++) {
        if (/*i+'A' == curseldn && */fun == Fun_DouWeight &&
                MP.sys_LWJC_FS->value().toInt() == 1) {
            int v = MP.doustatus[i + doustatuscnt + form->dn]->valuenp().toInt();
            if (v != 0x7FFE) {
                gwLab.at(i)->setEnabled(true);
                gwLab.at(i)->setText(MP.prm_ZZJ_ULW->vPS(v));
            }
            else {
                gwLab.at(i)->setEnabled(false);
                gwLab.at(i)->setText("ERR");
            }
            gwLab.at(i)->show();
        }
        else
            gwLab.at(i)->hide();
    }
}

void FormMenuDebug::refreshDouStatus()
{
    for (int i = 0; i < dLab.size(); i++) {
        int s = dstatus[i];
        if (curseldn == i+1)
            s = 7;
        else if (s != 15 && fun != Fun_DouWeight &&
                 (curseldn == 0 || i/form->dng == curseldn-'A'))
            s = 7;

        if (MP.sys_DOU_State[i+1]->value().toInt() ||
                fun == Fun_JLD || curseldn == i+1) {
            dLab.at(i)->setPixmap(form->doum->dimg.at(i).at(s));
        }
        else
            dLab.at(i)->setPixmap(form->doum->dimg.at(i).at(15));
    }
    for (int i = 0; i < gLab.size(); i++) {
        int s = gstatus[i];
        if (curseldn == i+'A')
            s = 7;
        else if (s != 15 && (curseldn == 0))
            s = 7;
        gLab.at(i)->setPixmap(form->doum->gimg.at(i).at(s));
    }

    update();
}

void FormMenuDebug::onRegUpdated(const MRegAreas &areas)
{
    if (areas.isIn(MP.doustatus_union))
        refreshUI();

    if (areas.isIn(MP.cmd)) {
        // cmd执行完成(会多次调用)
        if (!isrcmd && !MP.cmd->value().toUInt()) {
            emit cmdStoped();
            if (fun != Fun_DouWeight) {
                curseldn = -1; // 初始化选中
                refreshDouStatus();
            }
        }
    }
}

void FormMenuDebug::onFunClicked(int id)
{
    int cmd = MP.cmd->value().toUInt();
    if ((isrcmd || WAIT_CMD) && WAIT_FUN)
        waitCmdStop();
    fun = id;
    curseldn = -1;

    if (fun == Fun_DouWeight && !isrw) {
        isrw = true;
        onReadWeight();
    }
    else if (fun != Fun_DouWeight && isrw) {
        isrw = false;
        readWeightTimer->stop();
        form->mac.clearAction(echorw, ecIds);
        refreshUI();
    }

    int ds = 0;
    int gs = 0;

    switch (fun) {
    case Fun_ZZJ:
    case Fun_JLD:
        ds = 15;
        break;
    case Fun_DouWeight:
        if (MP.sys_LWJC_FS->value().toInt() == 1)
            break;
        gs = 15;
        break;
    case Fun_DouDis:
    case Fun_TestSamp:
    case Fun_TestKG:
    case Fun_TestZD:
        gs = 15;
        break;
    case Fun_Camera:
        showCamera();
        return;
    default:
        break;
    }
    for (int i = 0; i < MAX_D; i++) {
        dstatus[i] = ds;
    }
    for (int i = 0; i < MAX_G1; i++) {
        gstatus[i] = gs;
    }
    if (fun == Fun_JLD) {
        dstatus[0] = 12;
        dstatus[1] = 12;
    }
    showCamera(false);
    refreshDouStatus();
}

void FormMenuDebug::onReadCmdFns(int , const SignalEcho_zy1 &echo)
{
    form->mac.echom.finish(echo);
    if (echo == echocmd) {
        isrcmd--;
        if (!isrcmd && !MP.cmd->value().toUInt())
            emit cmdStoped();
    }
}

void FormMenuDebug::onReadWeightFns(int , const SignalEcho_zy1 &echo)
{
    form->mac.echom.finish(echo);
    if (echo == echorw)
        readWeightTimer->start(100);
}

void FormMenuDebug::onReadWeight()
{
    if (curseldn >= 0)
        form->writeCMD(CTL_MSV, curseldn);
    form->mac.read(MP.doustatus_union, echorw);
}

#include "DlgWaiting.h"
void FormMenuDebug::waitCmdStop()
{
    if (!form->mac.isconnect())
        return;

    isrcmd++;
    form->writeCMD(CTL_MANUAL_STOP, 0);
#ifdef MYOPT_TEST
    form->writeCMD(0, 0);
#endif
    form->mac.read(MP.cmd, echocmd);

//    DlgWaiting dlg(form, 3000);
//    connect(this, SIGNAL(cmdStoped()), &dlg, SLOT(close()));
//    dlg.exec();
    QDialog dlg;
    dlg.setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    dlg.setFixedSize(1, 1);
    connect(this, SIGNAL(cmdStoped()), &dlg, SLOT(close()));
    QTimer::singleShot(3000, &dlg, SLOT(close()));
    dlg.exec();
}

void FormMenuDebug::showCamera(bool b)
{
    ui->page_Camera->setVisible(b);
    if (b)
        ui->page_Camera->raise();

    if (b && !isOpen) {
        isOpen = true;
        form->cam_t1->open();
        form->cam_t2->open();
    }

    if (!b && isOpen) {
        isOpen = false;
        form->cam_t1->close();
        form->cam_t2->close();
    }
}

void FormMenuDebug::on_btn_t_1_clicked()
{
    if (ui->stackedWidget->currentIndex() == 0)
        return;
    ui->pushButton_hide->click();
    ui->stackedWidget->setCurrentIndex(0);
}

void FormMenuDebug::on_btn_t_2_clicked()
{
    if (ui->stackedWidget->currentIndex() == 1)
        return;
    ui->pushButton_hide->click();
    ui->stackedWidget->setCurrentIndex(1);
}

void FormMenuDebug::on_cam_calib_clicked()
{
    noHide = true;
    form->showCameraCalib();
}
