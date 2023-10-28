#ifndef DLGINFO_H
#define DLGINFO_H

#include "DlgArm.h"

namespace Ui {
class DlgInfo;
}

class DlgInfo : public DlgArm
{
    Q_OBJECT

public:
    explicit DlgInfo(QWidget *parent = nullptr);
    ~DlgInfo();

    void setText(const QString &str);

    static void info(QWidget *p, const QString &str);

private:
    Ui::DlgInfo *ui;
};

#endif // DLGINFO_H
