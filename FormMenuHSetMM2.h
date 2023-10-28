#ifndef FORMMENUHSETMM2_H
#define FORMMENUHSETMM2_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuHSetMM2;
}

class FormMenuHSetMM2 : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuHSetMM2(QWidget *parent = 0);
    ~FormMenuHSetMM2();
    void init(FormA *f, E_MotorMode mm);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);
    void onModeClicked(int i);
    void onEditFinished();

    void on_btn_add_1_clicked();

    void on_btn_add_2_clicked();

    void on_btn_del_clicked();

    void on_val_curi_editingFinished();

    void on_gm_clicked(int i);

    void on_btn_copy_clicked();

    void on_btn_test_clicked();

private:
    QVector<int> getMMData();
    void setMMData(const QVector<int> &v);
    void adjustMMData(int mi, int *d, int size);
    inline bool isCorrectMode() {
#ifdef MYOPT_TEST
        return true;
#endif
        return (MP.motor_id->value().toInt() & 0xFF00) == (motorm << 4);
    }

private:
    Ui::FormMenuHSetMM2 *ui;
    FormA *form;
    MPTRWidget *mptrw;
    QButtonGroup *btg;
    E_MotorMode motorm;
    int motormm;
    int motormi;
    int starti;
    int endi;
    QLineEdit *edits[5];
    MParam **mps[5];
};

#endif // FORMMENUHSETMM2_H
