#include "SPrinter.h"

SPrinter g_sprinter;
SPrinter::SPrinter(QObject *parent) : QObject(parent)
{
    moveToThread(&thd);
    sp = 0;
}

SPrinter::~SPrinter()
{
    thd.quit();
    thd.wait();
}

void SPrinter::init()
{
    QMutexLocker locker(&mutex);

    if (sp) return;

    sp = new QSerialPort("/dev/ttyS3");
    if (sp->open(QIODevice::ReadWrite))
    {
        sp->flush();
        sp->setBaudRate(QSerialPort::Baud9600);
        sp->setDataBits(QSerialPort::Data8);
        sp->setStopBits(QSerialPort::OneStop);
        sp->setParity(QSerialPort::NoParity);
        sp->setFlowControl(QSerialPort::NoFlowControl);
    } else
        qWarning("SerialPort open err");

    sp->moveToThread(&thd);
    connect(&thd, SIGNAL(finished()), sp, SLOT(deleteLater()));
    thd.start();
}

void SPrinter::printString(const QString &str)
{
    if (!sp) init();
    QMetaObject::invokeMethod(this, "printString_inv",
                                    Q_ARG(QString, str));
}

void SPrinter::printString_inv(const QString &str)
{
    sp->write(str.toLatin1().append('\n'));
}
