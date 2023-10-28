#ifndef FORMMENUCALIB_H
#define FORMMENUCALIB_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuCalib;
}

class FormMenuCalib : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuCalib(QWidget *parent = 0);
    ~FormMenuCalib();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
    virtual bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void refreshUI();
    void refreshDouWeight();
    void onRegUpdated(const MRegAreas &areas);

    void on_zero_clicked();
    void on_full_clicked();
    void on_c_all_clicked(bool checked);

    void onZDSenClicked(int i);
    void onZDSenLongPreesed();

private:
    void setEnabledZero(bool b);
    void selAllDou(bool b);

private:
    Ui::FormMenuCalib *ui;
    FormA *form;
    MPTRWidget *mptrw;
    int starti;
    int endi;
    QVector<QLabel *>dLab;
    QVector<QLabel *>gLab;
    QVector<QLabel *>dwLab;
    int curseldn;
    bool selall;
    QButtonGroup *btg_zd;
    QTimer *timer_zd;
};

#endif // FORMMENUCALIB_H
