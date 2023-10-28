#include "DialogStartup.h"
#include "ui_DialogStartup.h"

DialogStartup::DialogStartup(int processCount, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogStartup)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);

    timerid = 0;
    interval = 50;
    maximum = 40;
    curvalue = 0;
    countf = 0;
    countt = processCount;
    isProcessStarted = false;
    isProcessFinished = false;
    isProcessFinished0 = false;
}

DialogStartup::~DialogStartup()
{
    delete ui;
}

void DialogStartup::progress()
{
    countf++;
    finishProcess();
}

void DialogStartup::timerEvent(QTimerEvent *)
{
    if (finishProcess())
        return;

    if (curvalue < maximum) {
        curvalue++;
        ui->progressBar->setValue(curvalue);
    }
    else {
        killTimer();
        finishProcess();
    }
}

void DialogStartup::paintEvent(QPaintEvent *e)
{
    QDialog::paintEvent(e);

    if (isProcessStarted)
        return;

    QMetaObject::invokeMethod(this, "startProcess",
                              Qt::QueuedConnection);
}

void DialogStartup::startProcess()
{
    if (isProcessStarted)
        return;

    ui->progressBar->setMaximum(maximum);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);

    isProcessStarted = true;
    emit processStarted();

    timerid = startTimer(interval);
    timerEvent(0);
}

bool DialogStartup::finishProcess()
{
    if (isProcessFinished)
        return true;

    if (!isProcessStarted)
        return false;

    if (!isProcessFinished0 && !timerid) {
        isProcessFinished0 = true;
        emit processFinished0();
    }

    if (!(countf >= countt && !timerid))
        return false;

    killTimer();

    isProcessFinished = true;
    emit processFinished();
    return true;
}

void DialogStartup::killTimer()
{
    if (timerid) {
        QObject::killTimer(timerid);
        timerid = 0;
    }
}
