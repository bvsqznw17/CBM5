#ifndef FORMMENUPSETOTHER_H
#define FORMMENUPSETOTHER_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuPSetOther;
}

class FormMenuPSetOther : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuPSetOther(QWidget *parent = 0);
    ~FormMenuPSetOther();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);

private:
    Ui::FormMenuPSetOther *ui;
    FormA *form;
    MPTRWidget *mptrw;
    int starti;
    int endi;
};

#endif // FORMMENUPSETOTHER_H
