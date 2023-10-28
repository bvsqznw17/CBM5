#ifndef DIALOGDEBUGMM_H
#define DIALOGDEBUGMM_H

#include "DlgArm.h"
#include "FormA.h"
class QListWidgetItem;

namespace Ui {
class DialogDebugMM;
}

class DialogDebugMM : public DlgArm
{
    Q_OBJECT

public:
    explicit DialogDebugMM(QWidget *parent = nullptr);
    ~DialogDebugMM();
    void init(FormA *f, int mm);

private slots:
    void on_listWidget_itemPressed(QListWidgetItem *item);

private:
    Ui::DialogDebugMM *ui;
    FormA *form;
    int mmode;
};

#endif // DIALOGDEBUGMM_H
