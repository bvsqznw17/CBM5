#ifndef DLGOKCANCEL_H
#define DLGOKCANCEL_H

#include "DlgArm.h"

namespace Ui {
class DlgOKCancel;
}

class DlgOKCancel : public DlgArm
{
    Q_OBJECT

public:
    explicit DlgOKCancel(QWidget *parent = nullptr);
    ~DlgOKCancel();

    void setText(const QString &str);

private:
    Ui::DlgOKCancel *ui;
};

#endif // DLGOKCANCEL_H
