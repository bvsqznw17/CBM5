#include "PrintThread.h"
#include "Printer.h"
#include <QPainter>

PrintThread::PrintThread(QObject *parent) : QThread(parent)
{

}

QMutex PrintThread::mutex;
void PrintThread::printString(const QString &str)
{
    if (!mutex.tryLock())
        return;
    PrintThread *thd = new PrintThread;
    connect(thd, SIGNAL(finished()), thd, SLOT(deleteLater()));
    thd->m_str = str;
    thd->start();
}

void PrintThread::run()
{
    QFont font;
    font.setPixelSize(40);
    QFontMetrics fm(font);
    int fmw = fm.horizontalAdvance(m_str);
    int fmh = fm.height();
    QImage img(fmw, fmh, QImage::Format_Mono);
    img.setColorTable(QVector<QRgb>() << qRgb(255,255,255) << qRgb(0,0,0));
    img.fill(0);
    QPainter p(&img);
    p.setPen(qRgb(0,0,0));
    p.setFont(font);
    p.drawText(img.rect(), m_str);
    g_printer.print(img);
#ifdef MYOPT_TEST
    img.save("print.png");
#endif
    mutex.unlock();
}
