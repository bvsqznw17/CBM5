#ifndef FORMHELPDOUSTATE_H
#define FORMHELPDOUSTATE_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormHelpDouState;
}

class FormHelpDouState : public QWidget
{
    Q_OBJECT

public:
    explicit FormHelpDouState(QWidget *parent = 0);
    ~FormHelpDouState();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);
    void showDialog(int i);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::FormHelpDouState *ui;
    FormA *form;
    MPTRWidget *mptrw;
    QLabel *dou[16];
    QLabel *lab[16];
    QImage img;
    QVector<int> dstate;
};

#endif // FORMHELPDOUSTATE_H
