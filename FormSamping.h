#ifndef FORMSAMPING_H
#define FORMSAMPING_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormSamping;
}

class FormSamping : public QWidget
{
    Q_OBJECT

public:
    explicit FormSamping(QWidget *parent = 0);
    ~FormSamping();
    void init(FormA *f);

    int dounum;

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void refreshDouWeight();
    void onRegUpdated(const MRegAreas &areas);
    void onReadSampDataFns(int err, const SignalEcho_zy1 &echo);

    void on_btn_zero_clicked();

    void on_btn_test_clicked();

private:
    Ui::FormSamping *ui;
    FormA *form;
    MPTRWidget *mptrw;
    SignalEcho echo;
    SignalEchoIds ecIds;
    int starti;
    int endi;
};

#endif // FORMSAMPING_H
