#ifndef FORMMENUHSETSYSPARAM_H
#define FORMMENUHSETSYSPARAM_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuHSetSysParam;
}

class FormMenuHSetSysParam : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuHSetSysParam(QWidget *parent = 0);
    ~FormMenuHSetSysParam();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);

private:
    Ui::FormMenuHSetSysParam *ui;
    FormA *form;
    MPTRWidget *mptrw;
    int starti;
    int endi;
};

#endif // FORMMENUHSETSYSPARAM_H
