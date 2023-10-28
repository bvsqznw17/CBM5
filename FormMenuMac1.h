#ifndef FORMMENUMAC1_H
#define FORMMENUMAC1_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuMac1;
}

class FormMenuMac1 : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuMac1(QWidget *parent = 0);
    ~FormMenuMac1();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onDTChanged(const QDateTime &dt);
    void onRegUpdated(const MRegAreas &areas);

    void onDTEditFinished();
    void onPasswordEdit(int i);

    void on_lang_m_clicked();
    void on_ts_clicked();
    void on_bl_editingFinished();

    void net_up_ready(const QString &fn);
    void on_net_up_clear_clicked();
    void on_verup_clicked();

private:
    KbInputInfo IIF_Day();

private:
    Ui::FormMenuMac1 *ui;
    FormA *form;
    MPTRWidget *mptrw;
    int starti;
    int endi;
};

#endif // FORMMENUMAC1_H
