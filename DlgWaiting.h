#ifndef DLGWAITING_H
#define DLGWAITING_H

#include "DlgArm.h"
#include <QProgressBar>

namespace Ui {
class DlgWaiting;
}

class DlgWaiting : public DlgArm
{
    Q_OBJECT

public:
    explicit DlgWaiting(QWidget *parent, int ms, int maxms = -1);
    ~DlgWaiting();

    QProgressBar *progressBar();

private slots:
    void onTimeout();

private:
    Ui::DlgWaiting *ui;
    int m_cur;
    int m_max;
};

#endif // DLGWAITING_H
