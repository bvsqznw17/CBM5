#ifndef FORMTESTDOUKAIMEN_H
#define FORMTESTDOUKAIMEN_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormTestDouKaiMen;
}

class FormTestDouKaiMen : public QWidget
{
    Q_OBJECT

public:
    explicit FormTestDouKaiMen(QWidget *parent = 0);
    ~FormTestDouKaiMen();
    void init(FormA *f);

    int dounum;

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);

    void on_btn_ag_jian_clicked();

    void on_btn_ag_jia_clicked();

    void on_btn_zero_clicked();

    void on_btn_test_clicked();

private:
    Ui::FormTestDouKaiMen *ui;
    FormA *form;
    MPTRWidget *mptrw;
    QButtonGroup *btg;
};

#endif // FORMTESTDOUKAIMEN_H
