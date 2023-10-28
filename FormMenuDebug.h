#ifndef FORMMENUDEBUG_H
#define FORMMENUDEBUG_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuDebug;
}

class FormMenuDebug : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuDebug(QWidget *parent = 0);
    ~FormMenuDebug();
    void init(FormA *f);

    void waitDebugStop();

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
    virtual bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void refreshUI();
    void refreshDouStatus();
    void onRegUpdated(const MRegAreas &areas);
    void onFunClicked(int id);
    void onReadCmdFns(int err, const SignalEcho_zy1 &echorw);
    void onReadWeightFns(int err, const SignalEcho_zy1 &echorw);
    void onReadWeight();

    void on_btn_t_1_clicked();
    void on_btn_t_2_clicked();
    void on_cam_calib_clicked();

private:
    void waitCmdStop();
    void showCamera(bool b = true);

signals:
    void cmdStoped();

private:
    Ui::FormMenuDebug *ui;
    FormA *form;
    MPTRWidget *mptrw;
    QVector<QLabel *>dLab;
    QVector<QLabel *>gLab;
    QVector<QLabel *>dwLab;
    QVector<QLabel *>gwLab;
    int dstatus[MAX_D];
    int gstatus[MAX_G1];
    int curseldn;
    int fun;
    QButtonGroup *fun_btg;
    bool noHide;
    bool isOpen;

    SignalEcho echocmd;
    SignalEcho echorw;
    SignalEchoIds ecIds;
    int isrcmd;
    bool isrw;
    QTimer *readWeightTimer;
};

#endif // FORMMENUDEBUG_H
