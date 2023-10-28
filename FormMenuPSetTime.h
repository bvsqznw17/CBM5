#ifndef FORMMENUPSETTIME_H
#define FORMMENUPSETTIME_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuPSetTime;
}

class FormMenuPSetTime : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuPSetTime(QWidget *parent = 0);
    ~FormMenuPSetTime();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);

private:
    Ui::FormMenuPSetTime *ui;
    FormA *form;
    MPTRWidget *mptrw;
    int starti;
    int endi;
};

#endif // FORMMENUPSETTIME_H
