#ifndef FORMMENUHSETMM_H
#define FORMMENUHSETMM_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuHSetMM;
}

class FormMenuHSetMM : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuHSetMM(QWidget *parent = 0);
    ~FormMenuHSetMM();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);

private:
    Ui::FormMenuHSetMM *ui;
    FormA *form;
    MPTRWidget *mptrw;
};

#endif // FORMMENUHSETMM_H
