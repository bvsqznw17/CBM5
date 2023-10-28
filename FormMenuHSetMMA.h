#ifndef FORMMENUHSETMMA_H
#define FORMMENUHSETMMA_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuHSetMMA;
}

class FormMenuHSetMMA : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuHSetMMA(QWidget *parent = 0);
    ~FormMenuHSetMMA();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);

private:
    Ui::FormMenuHSetMMA *ui;
    FormA *form;
    MPTRWidget *mptrw;
};

#endif // FORMMENUHSETMMA_H
