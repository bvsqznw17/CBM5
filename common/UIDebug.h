#ifndef UIDEBUG_H
#define UIDEBUG_H
#define UIDEBUG

#include <QWidget>

class UIDebug : public QWidget
{
    Q_OBJECT
public:
    explicit UIDebug(QWidget *parent);
    void raise();

#ifdef UIDEBUG
private:
    QWidget *p;
    class QTextBrowser *tb_debug;
    class QWidget *w_debug;
    class QPushButton *bt_exp_debug;
    class QPushButton *bt_ps_pscreen;
    class QPushButton *bt_ps_pscreensave;
    void showEvent(QShowEvent *);
    void initUIDEBUG();
    void initUIDEBUG0();
    void ifDEBUG();
    void ifPSCREEN();
private slots:
    void handleMsg(QtMsgType type, QString msg);
    void exp_debug();
    void ps_pscreen();
    void ps_pscreensave();
private:
    static bool mount(const QString &dev) ;
    static void umount(const QString &dev) ;
#endif
};

#endif // UIDEBUG_H
