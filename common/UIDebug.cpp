#include "UIDebug.h"

UIDebug::UIDebug(QWidget *parent) :
    QWidget(parent)
{
    setFixedSize(0, 0);
#ifdef UIDEBUG
    p = parent;
    initUIDEBUG();
    //ifDEBUG();
    //ifPSCREEN();
#endif
}

void UIDebug::raise()
{
    initUIDEBUG0();
}

#ifdef UIDEBUG
#include "MsgHandlerWrapper.h"
#include <QTextBrowser>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSettings>
#include <QThread>
#include <QDir>

void UIDebug::showEvent(QShowEvent *)
{
    initUIDEBUG0();
}

void UIDebug::initUIDEBUG()
{
    bt_exp_debug = 0;
    bt_ps_pscreen = 0;
}

void UIDebug::initUIDEBUG0()
{
    if (bt_exp_debug) {
        bt_exp_debug->raise();
        w_debug->raise();
    }
    if (bt_ps_pscreen) {
        bt_ps_pscreen->raise();
        bt_ps_pscreensave->raise();
    }
}

void UIDebug::ifDEBUG()
{
    const int w_width = 400;

    QVBoxLayout *vl = new QVBoxLayout();
    vl->setMargin(0);
    vl->setSpacing(0);

    QPushButton *clear = new QPushButton(p);
    clear->setMinimumHeight(40);
    clear->setText("Clear");
    vl->addWidget(clear);

    tb_debug = new QTextBrowser(p);
    tb_debug->document()->setMaximumBlockCount(100);
    tb_debug->setFixedWidth(w_width);
    tb_debug->setStyleSheet("font-size: 12px");
    vl->addWidget(tb_debug);

    bt_exp_debug = new QPushButton(p);
    bt_exp_debug->setGeometry(p->width()-25, (p->height()-100)/2, 25, 100);
    bt_exp_debug->setText("<");

    w_debug = new QWidget(p);
    w_debug->setGeometry(p->width()-w_width, 0, w_width, p->height());
    w_debug->setLayout(vl);
    w_debug->hide();

    connect(MsgHandlerWrapper::instance(), SIGNAL(message(QtMsgType,QString)),
            this, SLOT(handleMsg(QtMsgType,QString)));
    connect(bt_exp_debug, SIGNAL(clicked()), this, SLOT(exp_debug()));
    connect(clear, SIGNAL(clicked()), tb_debug, SLOT(clear()));
}

void UIDebug::ifPSCREEN() {
    bt_ps_pscreen = new QPushButton(p);
    bt_ps_pscreen->setGeometry(0, p->height()/2 - 50, 25, 50);
    bt_ps_pscreen->setText("P");
    connect(bt_ps_pscreen, SIGNAL(clicked()), this, SLOT(ps_pscreen()));

    bt_ps_pscreensave = new QPushButton(p);
    bt_ps_pscreensave->setGeometry(0, p->height()/2, 25, 50);
    bt_ps_pscreensave->setText("=");
    connect(bt_ps_pscreensave, SIGNAL(clicked()), this, SLOT(ps_pscreensave()));
}

void UIDebug::handleMsg(QtMsgType type, QString msg)
{
    if (type == QtDebugMsg)
        tb_debug->setTextColor(QColor(0,0,0));
    else
        tb_debug->setTextColor(QColor(255,0,0));
    tb_debug->append(msg);
}

void UIDebug::exp_debug()
{
    w_debug->setVisible(!w_debug->isVisible());
    if (w_debug->isVisible()) {
        bt_exp_debug->setGeometry(p->width()-25-w_debug->width(), (p->height()-100)/2, 25, 100);
        bt_exp_debug->setText(">");
    }
    else {
        bt_exp_debug->setGeometry(p->width()-25, (p->height()-100)/2, 25, 100);
        bt_exp_debug->setText("<");
    }
}

void UIDebug::ps_pscreen()
{
    bt_ps_pscreen->hide();
    bt_ps_pscreensave->hide();
    QPixmap pm = p->grab();
    bt_ps_pscreen->show();
    bt_ps_pscreensave->show();

    QDir::current().mkdir("PrintScreen");

    QSettings ini("PrintScreen/number.ini", QSettings::IniFormat);
    int number = ini.value("number", 0).toInt();
    bool ok = pm.save(QString("PrintScreen/%1.png")
                      .arg(number, 6, 10, QChar('0')));

    if (ok) {
        ini.setValue("number", number + 1);
        if (bt_ps_pscreen->text() == "P")
            bt_ps_pscreen->setText("S");
        else
            bt_ps_pscreen->setText("P");
    } else
        bt_ps_pscreen->setText("F");
}

void UIDebug::ps_pscreensave()
{
    bool ok = false;

//    for (int i = 0; i < 15 && !ok; i++) {
//        ok = mount("sda1");
//        QThread::sleep(2);
//    }

    if (!ok)
        ok = mount("sda1");
    if (ok) {
        ok = !system("cp -a PrintScreen /mnt/usb");
        system("sync");
        umount("sda1");
    }

    if (ok) {
        system("rm -rf PrintScreen");
        if (bt_ps_pscreensave->text() == "=")
            bt_ps_pscreensave->setText("-");
        else
            bt_ps_pscreensave->setText("=");
    } else
        bt_ps_pscreensave->setText("F");

    system("sync; sync; sync;");
}

bool UIDebug::mount(const QString &dev)
{
    QDir::current().mkpath("/mnt/usb");
    for(int i = 0; i < 2; i++) // 尝试挂载两次
    if (::system(QString("mount /dev/%1 /mnt/usb 1> /dev/null 2> /dev/null")
                 .arg(dev).toLatin1().constData())) {
        ::system(QString("umount /dev/%1 1> /dev/null 2> /dev/null")
                 .arg(dev).toLatin1().constData());
        ::system(QString("umount /mnt/usb 1> /dev/null 2> /dev/null")
                 .toLatin1().constData());
    }
    else
        return true;

    return false;
}

void UIDebug::umount(const QString &dev)
{
    ::system(QString("umount /dev/%1 1> /dev/null 2> /dev/null")
             .arg(dev).toLatin1().constData());
}
#endif
