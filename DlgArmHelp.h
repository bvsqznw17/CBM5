#ifndef DLGARMHELP_H
#define DLGARMHELP_H

#include "DlgArm.h"

namespace Ui {
class DlgArmHelp;
}

class DlgArmHelp : public DlgArm
{
    Q_OBJECT

public:
    explicit DlgArmHelp(QWidget *parent = nullptr);
    ~DlgArmHelp();

    void setText(const QString &str);

private:
    Ui::DlgArmHelp *ui;
};

#endif // DLGARMHELP_H
