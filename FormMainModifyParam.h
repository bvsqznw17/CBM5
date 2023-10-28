#ifndef FORMMAINMODIFYPARAM_H
#define FORMMAINMODIFYPARAM_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMainModifyParam;
}

class FormMainModifyParam : public QWidget
{
    Q_OBJECT

public:
    explicit FormMainModifyParam(QWidget *parent = 0);
    ~FormMainModifyParam();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);
    void onEditFinished();
    void onXZSJClicked();
    void onXZZFClicked();

private:
    Ui::FormMainModifyParam *ui;
    FormA *form;
    MPTRWidget *mptrw;
    int starti;
    int endi;
};

#endif // FORMMAINMODIFYPARAM_H
