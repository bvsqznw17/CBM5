#ifndef FORMMENUPSETSPECIFY_H
#define FORMMENUPSETSPECIFY_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuPSetSpecify;
}

class FormMenuPSetSpecify : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuPSetSpecify(QWidget *parent = 0);
    ~FormMenuPSetSpecify();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);
    void onZDDClicked(int i);
    void onDDZHClicked(int i);
    void onCQQPClicked(int i);

private:
    Ui::FormMenuPSetSpecify *ui;
    FormA *form;
    MPTRWidget *mptrw;
    int starti;
    int endi;
    const static int zddn = 7;
    QPushButton *zdd[zddn];
    QCheckBox *ddzh[zddn];
    QCheckBox *cqqp[zddn];
    MParam *mps_zdd[zddn];
    quint32 old_zdd[zddn];
};

#endif // FORMMENUPSETSPECIFY_H
