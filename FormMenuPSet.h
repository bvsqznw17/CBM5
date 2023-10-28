#ifndef FORMMENUPSET_H
#define FORMMENUPSET_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuPSet;
}

class FormMenuPSet : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuPSet(QWidget *parent = 0);
    ~FormMenuPSet();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);

private:
    Ui::FormMenuPSet *ui;
    FormA *form;
    MPTRWidget *mptrw;
};

#endif // FORMMENUPSET_H
