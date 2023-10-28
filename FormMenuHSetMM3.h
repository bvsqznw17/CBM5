#ifndef FORMMENUHSETMM2_H
#define FORMMENUHSETMM2_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuHSetMM2;
}

class FormMenuHSetMM2 : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuHSetMM2(QWidget *parent = 0);
    ~FormMenuHSetMM2();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);

private:
    Ui::FormMenuHSetMM2 *ui;
    FormA *form;
    MPTRWidget *mptrw;
};

#endif // FORMMENUHSETMM2_H
