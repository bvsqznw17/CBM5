#ifndef FORMMAINCOMB_H
#define FORMMAINCOMB_H

#include <QWidget>

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMainComb;
}

class FormMainComb : public QWidget
{
    Q_OBJECT

public:
    explicit FormMainComb(QWidget *parent = 0);
    ~FormMainComb();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void refreshDouWeight();
    void onRegUpdated(const MRegAreas &areas);
    void onInTWeightChanged(int w);

private:
    Ui::FormMainComb *ui;
    FormA *form;
    MPTRWidget *mptrw;
    QVector<QLabel *>dLab;
    QVector<QLabel *>gLab;
    QVector<QLabel *>dwLab;
    QVector<QLabel *>gwLab;
    QVector<int> dstate;
    QLabel *vals[MAX_G][13];
    int inTWeight;
};

#endif // FORMMAINCOMB_H
