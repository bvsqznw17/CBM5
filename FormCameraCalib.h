#ifndef FORMCAMERACALIB_H
#define FORMCAMERACALIB_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormCameraCalib;
}

class FormCameraCalib : public QWidget
{
    Q_OBJECT

public:
    explicit FormCameraCalib(QWidget *parent = 0);
    ~FormCameraCalib();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);
    void onCameraCalibrated();
    void on_ezh_1_clicked(bool checked);
    void on_ezh_2_clicked(bool checked);
    void on_yz_1_editingFinished();
    void on_yz_2_editingFinished();
    void on_yzjian_1_clicked();
    void on_yzjian_2_clicked();
    void on_yzjia_1_clicked();
    void on_yzjia_2_clicked();
    void on_calib_1_clicked();
    void on_calib_2_clicked();

private:
    void edityz1(int v);
    void edityz2(int v);

private:
    Ui::FormCameraCalib *ui;
    FormA *form;
    MPTRWidget *mptrw;
    bool isOpen;
};

#endif // FORMCAMERACALIB_H
