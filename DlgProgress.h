#ifndef DLGPROGRESS_H
#define DLGPROGRESS_H

#include "DlgArm.h"
#include <QProgressBar>

namespace Ui {
class DlgProgress;
}

class DlgProgress : public DlgArm
{
    Q_OBJECT

public:
    explicit DlgProgress(QWidget *parent = nullptr, bool cancel = false);
    ~DlgProgress();

    QProgressBar *progressBar();

signals:
    void cancel();

public slots:
    void onProgress(int cur, int total);

private slots:
    void on_Cancel_clicked();

private:
    void closeEvent(QCloseEvent *e);

private:
    Ui::DlgProgress *ui;
    bool isClose;
};

#endif // DLGPROGRESS_H
