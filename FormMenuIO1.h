#ifndef FORMMENUIO1_H
#define FORMMENUIO1_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuIO1;
}

class FormMenuIO1 : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuIO1(QWidget *parent = 0);
    ~FormMenuIO1();
    void init(FormA *f, int i = 0);

    static QStringList getInFunStrsAll() { return in_fun_strs_all; }

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void refreshStatus();
    void onRegUpdated(const MRegAreas &areas);
    void onInFunClicked(int i);
    void onInInvClicked(int i);

private:
    Ui::FormMenuIO1 *ui;
    FormA *form;
    MPTRWidget *mptrw;
    int index;
    const static int ion = 8;
    QPushButton *in_funs[ion];
    QCheckBox *in_invs[ion];
    QCheckBox *in_states[ion];
    static QStringList in_fun_strs_all;
    QStringList in_fun_strs;
    QVariantList in_fun_vals;
};

#endif // FORMMENUIO1_H
