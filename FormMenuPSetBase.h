#ifndef FORMMENUPSETBASE_H
#define FORMMENUPSETBASE_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuPSetBase;
}

class FormMenuPSetBase : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuPSetBase(QWidget *parent = 0);
    ~FormMenuPSetBase();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);

private:
    Ui::FormMenuPSetBase *ui;
    FormA *form;
    MPTRWidget *mptrw;
    int starti;
    int endi;
};

#endif // FORMMENUPSETBASE_H
