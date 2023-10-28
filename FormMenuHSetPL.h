#ifndef FORMMENUHSETPL_H
#define FORMMENUHSETPL_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuHSetPL;
}

class FormMenuHSetPL : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuHSetPL(QWidget *parent = 0);
    ~FormMenuHSetPL();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);
    void onEditFinished();
    void onXZPLClicked();
    void onXZZYClicked();
    void onDouSelAll();

    void on_btn_test_z_clicked();

    void on_btn_test_x_clicked();

private:
    Ui::FormMenuHSetPL *ui;
    FormA *form;
    MPTRWidget *mptrw;
    int starti;
    int endi;
    QPushButton *dou[MAX_D];
    QButtonGroup *btg;
};

#endif // FORMMENUHSETPL_H
