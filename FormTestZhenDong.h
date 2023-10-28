#ifndef FORMTESTZHENDONG_H
#define FORMTESTZHENDONG_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormTestZhenDong;
}

class FormTestZhenDong : public QWidget
{
    Q_OBJECT

public:
    explicit FormTestZhenDong(QWidget *parent = 0);
    ~FormTestZhenDong();
    void init(FormA *f);

    int dounum;

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);
    void onReadWeightFns();

    void on_btn_start_clicked();

    void on_btn_stop_clicked();

    void on_btn_cpt_clicked();

    void on_btn_zero_clicked();

    void on_btn_back_clicked();

private:
    Ui::FormTestZhenDong *ui;
    FormA *form;
    MPTRWidget *mptrw;
    MPTReader *mptr;
    int starti;
    int endi;
};

#endif // FORMTESTZHENDONG_H
