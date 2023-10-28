#ifndef FORMMENUHSETSORTOR_H
#define FORMMENUHSETSORTOR_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuHSetSortor;
}

class FormMenuHSetSortor : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuHSetSortor(QWidget *parent = 0);
    ~FormMenuHSetSortor();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);

private:
    Ui::FormMenuHSetSortor *ui;
    FormA *form;
    MPTRWidget *mptrw;
};

#endif // FORMMENUHSETSORTOR_H
