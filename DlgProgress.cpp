#include "DlgProgress.h"
#include "ui_DlgProgress.h"
#include <QTimer>
#include <QCloseEvent>

DlgProgress::DlgProgress(QWidget *parent, bool cancel) :
    DlgArm(parent),
    ui(new Ui::DlgProgress),
    isClose(false)
{
    ui->setupUi(this);
    ui->Cancel->setVisible(cancel);
    this->adjustSize();
    this->resize(500, height());
}

DlgProgress::~DlgProgress()
{
    delete ui;
}

QProgressBar *DlgProgress::progressBar()
{
    return ui->progressBar;
}

void DlgProgress::onProgress(int cur, int total)
{
    if (isClose) {
        static bool b = false;
        if (!b) {
            b = true;
            qWarning("DlgProgress::onProgress: isClose");
        }
        return;
    }
    if (cur < 0) {
        isClose = true;
        this->done(cur);
        return;
    }
    if (cur >= total) {
        isClose = true;
        setResult(0);
        QTimer::singleShot(200, this, SLOT(close()));
    }

    ui->progressBar->setMaximum(total);
    ui->progressBar->setValue(cur);
}

void DlgProgress::on_Cancel_clicked()
{
    if (!ui->Cancel->isEnabled())
        return;

    ui->Cancel->setEnabled(false);
    emit cancel();
}

void DlgProgress::closeEvent(QCloseEvent *e)
{
    if (isClose)
        return;

    if (!ui->Cancel->isHidden())
        on_Cancel_clicked();

    e->ignore();
}
