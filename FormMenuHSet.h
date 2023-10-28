#ifndef FORMMENUHSET_H
#define FORMMENUHSET_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuHSet;
}

class FormMenuHSet : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuHSet(QWidget *parent = 0);
    ~FormMenuHSet();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);

private:
    Ui::FormMenuHSet *ui;
    FormA *form;
    MPTRWidget *mptrw;
};

#endif // FORMMENUHSET_H
