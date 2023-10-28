#ifndef FORMMENU_H
#define FORMMENU_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenu;
}

class FormMenu : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenu(QWidget *parent = 0);
    ~FormMenu();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);
    void onAuthChanged(int auth);

    void onButtonClicked(int id);
    void on_home_clicked();

private:
    Ui::FormMenu *ui;
    FormA *form;
    MPTRWidget *mptrw;
};

#endif // FORMMENU_H
