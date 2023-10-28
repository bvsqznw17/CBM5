#ifndef FORMHELPIOSTATE_H
#define FORMHELPIOSTATE_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormHelpIOState;
}

class FormHelpIOState : public QWidget
{
    Q_OBJECT

public:
    explicit FormHelpIOState(QWidget *parent = 0);
    ~FormHelpIOState();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void refreshStatus();
    void onRegUpdated(const MRegAreas &areas);

private:
    Ui::FormHelpIOState *ui;
    FormA *form;
    MPTRWidget *mptrw;
    const static int in_n = 8;
    QLabel *in_nums[in_n];
    QLabel *in_funs[in_n];
    const static int out_n = 24;
    QLabel *out_nums[out_n];
    QLabel *out_funs[out_n];
};

#endif // FORMHELPIOSTATE_H
