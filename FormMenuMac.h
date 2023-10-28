#ifndef FORMMENUMAC_H
#define FORMMENUMAC_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuMac;
}

class FormMenuMac : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuMac(QWidget *parent = 0);
    ~FormMenuMac();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);

private:
    Ui::FormMenuMac *ui;
    FormA *form;
    MPTRWidget *mptrw;
};

#endif // FORMMENUMAC_H
