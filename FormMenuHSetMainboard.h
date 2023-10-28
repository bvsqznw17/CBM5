#ifndef FORMMENUHSETMAINBOARD_H
#define FORMMENUHSETMAINBOARD_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuHSetMainboard;
}

class FormMenuHSetMainboard : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuHSetMainboard(QWidget *parent = 0);
    ~FormMenuHSetMainboard();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);
    void onWriteCmd();
    void onCmdFns(int cnt);
    void onReadFns(int err, const SignalEcho_zy1 &echo);

    void on_pushButton_clicked();

private:
    Ui::FormMenuHSetMainboard *ui;
    FormA *form;
    MPTRWidget *mptr;
    static const int num = 46;
    QLabel *ver[num];
    QPushButton *state[num];

    SignalEcho echo;
    SignalEchoIds ecIds;
};

#endif // FORMMENUHSETMAINBOARD_H
