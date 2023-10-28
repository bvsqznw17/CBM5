#ifndef FORMMENUHSETMMA1_H
#define FORMMENUHSETMMA1_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuHSetMMA1;
}

class FormMenuHSetMMA1 : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuHSetMMA1(QWidget *parent = 0);
    ~FormMenuHSetMMA1();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);

private:
    Ui::FormMenuHSetMMA1 *ui;
    FormA *form;
    MPTRWidget *mptrw;
};

#endif // FORMMENUHSETMMA1_H
