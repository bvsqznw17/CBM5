#ifndef FORMMAINZERO_H
#define FORMMAINZERO_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMainZero;
}

class FormMainZero : public QWidget
{
    Q_OBJECT

public:
    explicit FormMainZero(QWidget *parent = 0);
    ~FormMainZero();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void refreshBtnStatus();
    void onRegUpdated(const MRegAreas &areas);

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void onClean2();

private:
    Ui::FormMainZero *ui;
    FormA *form;
    MPTRWidget *mptrw;
    MPTReader *mptr;
    QVector<QLabel *>dLab;
    QVector<QLabel *>gLab;
    QVector<QLabel *>dwLab;
    QVector<QLabel *>gwLab;
};

#endif // FORMMAINZERO_H
