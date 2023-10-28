#ifndef FORMMENUPLIST_H
#define FORMMENUPLIST_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuPList;
}

class FormMenuPList : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuPList(QWidget *parent = 0);
    ~FormMenuPList();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);

    void on_lineup_clicked();

    void on_linedown_clicked();

    void on_pageup_clicked();

    void on_pagedown_clicked();

    void on_btn_sel_clicked();

    void on_btn_copy_clicked();

    void on_btn_del_clicked();

private:
    Ui::FormMenuPList *ui;
    FormA *form;
    MPTRWidget *mptrw;
};

#endif // FORMMENUPLIST_H
