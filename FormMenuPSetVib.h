#ifndef FORMMENUPSETVIB_H
#define FORMMENUPSETVIB_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuPSetVib;
}

class FormMenuPSetVib : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuPSetVib(QWidget *parent = 0);
    ~FormMenuPSetVib();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);
    void onXZSJClicked();
    void onXZZFClicked();

private:
    Ui::FormMenuPSetVib *ui;
    FormA *form;
    MPTRWidget *mptrw;
    int starti;
    int endi;
};

#endif // FORMMENUPSETVIB_H
