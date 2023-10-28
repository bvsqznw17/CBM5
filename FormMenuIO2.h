#ifndef FORMMENUIO2_H
#define FORMMENUIO2_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuIO2;
}

class FormMenuIO2 : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuIO2(QWidget *parent = 0);
    ~FormMenuIO2();
    void init(FormA *f, int i = 0);

    static QStringList getOutFunStrsAll() { return out_fun_strs_all; }

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void refreshStatus();
    void onRegUpdated(const MRegAreas &areas);
    void onOutFunClicked(int i);
    void onOutInvClicked(int i);
    void onOutMoniPressed(int i);
    void onOutMoniReleased(int i);

private:
    Ui::FormMenuIO2 *ui;
    FormA *form;
    MPTRWidget *mptrw;
    int index;
    const static int ion = 8;
    QPushButton *out_funs[ion];
    QCheckBox *out_invs[ion];
    QCheckBox *out_states[ion];
    QPushButton *out_monis[ion];
    static QStringList out_fun_strs_all;
    QStringList out_fun_strs;
    QVariantList out_fun_vals;
};

#endif // FORMMENUIO2_H
