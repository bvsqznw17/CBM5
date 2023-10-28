#ifndef SPRINTER_H
#define SPRINTER_H

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QSerialPort>

extern class SPrinter g_sprinter;
class SPrinter : public QObject
{
    Q_OBJECT
public:
    explicit SPrinter(QObject *parent = nullptr);
    ~SPrinter();

signals:

public:
    void init();
    void printString(const QString &str);

public slots:
    void printString_inv(const QString &str);

private:
    QMutex mutex;
    QThread thd;
    QSerialPort *sp;

};

#endif // SPRINTER_H
