#ifndef FORM_H
#define FORM_H

#include <QWidget>
class UIDebug;
class MachineComm;

QT_BEGIN_NAMESPACE
namespace Ui { class Form; }
QT_END_NAMESPACE

#ifndef MYOPT_OnlineManager
class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

private slots:
    void onInitStarted();
    void onInitFinished();
    void onInitFinished0();
    void onSelfCheck();

private:
    Ui::Form *ui;
    QDialog *startup;
    QThread *langThd;
    QThread *mcrtuThd;
    QThread *mtcpsThd;
    QThread *jtcpsThd;
    QThread *tcpCommThd;
    MachineComm *comm;
    QWidget *form;
    UIDebug *uidebug;
};
#else
#include <QTcpSocket>
class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0,
                  const QString &ip = 0,
                  const QVariantMap &port = QVariantMap(),
                  const QString &wd = 0,
                  const QString &language = 0,
                  const QString &dlldir = 0);
    ~Form();

protected:
    virtual void paintEvent(QPaintEvent *);

private slots:
    void onInitStarted();
    void onInitFinished();

private:
    Q_SIGNAL void valChanged();
    Q_SLOT void onSocketConnected();
    Q_SLOT void onSocketDisconnected();

private:
    Ui::Form *ui;
    QDialog *startup;
    QThread *langThd;
    QThread *mctcpThd;
    QThread *mtcpsThd;
    MachineComm *comm;
    QWidget *form;
    UIDebug *uidebug;
};
#endif

#endif // FORM_H
