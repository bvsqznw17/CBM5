#ifndef FORMMAIN_H
#define FORMMAIN_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMain;
}

class FormMain : public QWidget
{
    Q_OBJECT

public:
    explicit FormMain(QWidget *parent = 0);
    ~FormMain();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onPageChanged(int index);
    void onDTChanged(const QDateTime &dt);
    void onRegUpdated(const MRegAreas &areas);
    void onAuthChanged(int auth);

    void on_menu_clicked();

    void on_help_clicked();

    void on_run_clicked();

    void on_stop_clicked();

    void on_lack_clicked();

    void on_grade_clicked();

    void on_online_clicked(bool b);
    void onConnChanged();
    void onOnlineLight();
    void onWiFiStateChanged(bool conn);

    void on_pushButton_test_clicked();

private:
    Ui::FormMain *ui;
    FormA *form;
    MPTRWidget *mptrw;

    QTimer *onlineLightTimer;
    bool onlineLightFlag;
    QPixmap onlineLightPixmap[3];
};

#endif // FORMMAIN_H
