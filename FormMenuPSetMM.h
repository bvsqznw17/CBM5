#ifndef FORMMENUPSETMM_H
#define FORMMENUPSETMM_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuPSetMM;
}

class FormMenuPSetMM : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuPSetMM(QWidget *parent = 0);
    ~FormMenuPSetMM();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);

private:
    Ui::FormMenuPSetMM *ui;
    FormA *form;
    MPTRWidget *mptrw;
    int starti;
    int endi;
};

#endif // FORMMENUPSETMM_H
