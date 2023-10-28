#include "Tool.h"
#include <QSettings>
#include <QProcess>

Tool g_tool_init;

Tool::Tool()
{
    Tool::initTimeChaZhi();
}

qint64 Tool::time_chazhi = 0;

void Tool::initTimeChaZhi()
{
#ifdef __ARM_EABI__
    QSettings settings("/MainProgram_FenLi/settings.ini", QSettings::IniFormat);
#else
    QSettings settings("./settings.ini", QSettings::IniFormat);
#endif
    time_chazhi = settings.value("TimeChaZhi").toLongLong();
}

void Tool::setTimeChaZhi(qint64 chahzi)
{
#ifdef __ARM_EABI__
    QSettings settings("/MainProgram_FenLi/settings.ini", QSettings::IniFormat);
#else
    QSettings settings("./settings.ini", QSettings::IniFormat);
#endif
    time_chazhi = chahzi;
    settings.setValue("TimeChaZhi", chahzi);
#ifdef __ARM_EABI__
    settings.sync();
    system("sync");
#endif
}

void Tool::setDateTime(const QDateTime &dt)
{
//    Q_UNUSED(dt);
//#ifdef __ARM_EABI__
    QDateTime cdt = QDateTime::currentDateTime();
    qint64 chazhi = cdt.msecsTo(dt);
    setTimeChaZhi(chazhi);
//#endif
}

QDateTime Tool::currentDateTime()
{
    QDateTime cdt = QDateTime::currentDateTime();
//#ifdef __ARM_EABI__
    cdt = cdt.addMSecs(time_chazhi);
//#endif
    return cdt;
}

QDate Tool::date(const QString &str)
{
    return QDate::fromString(str, Qt::ISODate);
}

QTime Tool::time(const QString &str)
{
    return QTime::fromString(str, Qt::ISODate);
}

QDateTime Tool::dt(const QString &str)
{
    return QDateTime::fromString(str, Qt::ISODate);
}

QString Tool::dateToStr(const QDate &d)
{
    if (d.isNull())
        return "0000-00-00";
    else
        return d.toString("yyyy-MM-dd");
}

QString Tool::dateToStr(const QString &str)
{
    return dateToStr(QDate::fromString(str, Qt::ISODate));
}

QString Tool::timeToStr(const QTime &t)
{
    if (t.isNull())
        return "00:00:00";
    else
        return t.toString("hh:mm:ss");
}

QString Tool::timeToStr(const QString &str)
{
    return timeToStr(QTime::fromString(str, Qt::ISODate));
}

QString Tool::dtToStr(const QDateTime &dt)
{
    return QString("%1 %2").arg(dateToStr(dt.date()), timeToStr(dt.time()));
}

QString Tool::dtToStr(const QString &str)
{
    return dtToStr(QDateTime::fromString(str, Qt::ISODate));
}

QString Tool::fromXString(const QString &str)
{
    QByteArray s;
    s.reserve(str.size());
    for (int i = 0; i < str.size(); i++) {
        QChar c = str.at(i);
        if (c == '\\' && i+1 < str.size()) {
            QChar c1 = str.at(i+1);
            if (c1 == 'x') {
                ushort num = 0;
                int j = i+2;
                for (; j < str.size(); j++) {
                    ushort c2 = str.at(j).unicode();
                    if (c2 >= '0' && c2 <= '9')
                        c2 -= '0';
                    else if (c2 >= 'A' && c2 <= 'F')
                        c2 -= 'A'-10;
                    else if (c2 >= 'a' && c2 <= 'f')
                        c2 -= 'a'-10;
                    else
                        break;
                    num = num * 16 + c2;
                }
                s.append((char)num);
                i = j-1;
                continue;
            }
            if (c1 == '\\') {
                s.append(QString(QChar(c1)).toUtf8());
                i++;
                continue;
            }
        }
        s.append(QString(QChar(c)).toUtf8());
    }
    return QString::fromUtf8(s);
}

int Tool::y1c2(qreal a, qreal b, qreal c, qreal *x1, qreal *x2)
{
    qreal delta = b*b - 4*a*c;
    if (delta > 0) {
        *x1 = (-b + sqrt(delta)) / 2;
        *x2 = (-b - sqrt(delta)) / 2;
        return 2;
    }
    else if (delta == 0) {
        *x1 = (-b + sqrt(delta)) / 2;
        return 1;
    }
    else
        return 0;
}

QString Tool::getAFCStr(int n)
{
    switch (n) {
    case 1:
        return "AFCT";
    case 2:
        return "AFCI";
    case 3:
        return "AFCR";
    default:
        return "";
    }
}
