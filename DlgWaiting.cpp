#include "DlgWaiting.h"
#include "ui_DlgWaiting.h"
#include <QTimer>

DlgWaiting::DlgWaiting(QWidget *parent, int ms, int maxms) :
    DlgArm(parent),
    ui(new Ui::DlgWaiting)
{
    ui->setupUi(this);
    this->adjustSize();
    this->resize(500, height());

    if (maxms == -1)
        maxms = ms;

    m_cur = 0;
    m_max = maxms/50;
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(ms/50);
    ui->progressBar->setValue(0);
    QTimer *timer = new QTimer(this);
    timer->setInterval(50);
    timer->start();
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

DlgWaiting::~DlgWaiting()
{
    delete ui;
}

QProgressBar *DlgWaiting::progressBar()
{
    return ui->progressBar;
}

void DlgWaiting::onTimeout()
{
    m_cur++;
    if (m_cur <= ui->progressBar->maximum())
        ui->progressBar->setValue(m_cur);
    else if (m_cur >= m_max)
        close();
}
