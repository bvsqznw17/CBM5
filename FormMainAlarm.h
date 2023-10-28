#ifndef FORMMAINALARM_H
#define FORMMAINALARM_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMainAlarm;
}

class FormMainAlarm : public QWidget
{
    Q_OBJECT

public:
    explicit FormMainAlarm(QWidget *parent = 0);
    ~FormMainAlarm();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
    virtual bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);
    void onActionFinished(int err, const SignalEcho_zy1 &echorw);
    void onReadWeight();
    void onFunClicked(int id);
    void onAlarmClear();

private:
    Ui::FormMainAlarm *ui;
    FormA *form;
    MPTRWidget *mptrw;
    QVector<QLabel *>dLab;
    QVector<QLabel *>gLab;
    QVector<QLabel *>dwLab;
    int curseldn;

    SignalEcho echorw;
    SignalEchoIds ecIds;
    bool isrw;
};

#endif // FORMMAINALARM_H
