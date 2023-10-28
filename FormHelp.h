#ifndef FORMHELP_H
#define FORMHELP_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormHelp;
}

class FormHelp : public QWidget
{
    Q_OBJECT

public:
    explicit FormHelp(QWidget *parent = 0);
    ~FormHelp();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);

    void on_menu_clicked();

    void on_nextPage_clicked();

private:
    Ui::FormHelp *ui;
    FormA *form;
    MPTRWidget *mptrw;
};

#endif // FORMHELP_H
