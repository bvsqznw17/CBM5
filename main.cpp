#include "MTools.h"
MTools_AppEndExe _AppEndExe;
#include "Form.h"
#include "cbDef.h"
#include <QProcess>
#include <QDebug>

#include <QApplication>

int main(int argc, char *argv[])
{
//    qDebug() << "sizeof(SYSSET)/2" << sizeof(SYSSET)/2;
//    qDebug() << "sizeof(PRMSET)/2" << sizeof(PRMSET)/2;

//    QProcess p;
//    p.start("df", QStringList() << "-m" << "/");
//    p.waitForFinished();
//    QString str = QString::fromUtf8(p.readAllStandardOutput());
//    QStringList list = str.split('\n');

//    for (int i = 0; i < list.size(); i++) {
//        if (list.at(i).endsWith('/')) {
//            QStringList list0 = list.at(i).split(QRegExp(" +"));
//            qDebug() << list0;
//        }
//    }

    QApplication a(argc, argv);
    Form w;
    w.show();
    return a.exec();
}
