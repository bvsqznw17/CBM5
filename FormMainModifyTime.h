#ifndef FORMMAINMODIFYTIME_H
#define FORMMAINMODIFYTIME_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMainModifyTime;
}

class FormMainModifyTime : public QWidget
{
    Q_OBJECT

public:
    explicit FormMainModifyTime(QWidget *parent = 0);
    ~FormMainModifyTime();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);

private:
    Ui::FormMainModifyTime *ui;
    FormA *form;
    MPTRWidget *mptrw;
    int starti;
    int endi;
};

#endif // FORMMAINMODIFYTIME_H
