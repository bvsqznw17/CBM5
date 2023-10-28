#ifndef TOOL_H
#define TOOL_H

#include <QString>
#include <QDateTime>
#include <math.h>

class Tool
{
public:
    Tool();

    static qint64 time_chazhi;
    static void initTimeChaZhi();
    static void setTimeChaZhi(qint64 chahzi);
    static void setDateTime(const QDateTime &dt);
    static QDateTime currentDateTime();
    static inline QDate currentDate() { return currentDateTime().date(); }
    static inline QTime currentTime() { return currentDateTime().time(); }

    static QDate date(const QString &str);
    static QTime time(const QString &str);
    static QDateTime dt(const QString &str);
    static QString dateToStr(const QDate &d);
    static QString dateToStr(const QString &str);
    static QString timeToStr(const QTime &t);
    static QString timeToStr(const QString &str);
    static QString dtToStr(const QDateTime &dt);
    static QString dtToStr(const QString &str);

    static QString fromXString(const QString &str);

    static int y1c2(qreal a, qreal b, qreal c, qreal *x1, qreal *x2);

    inline static QString getDotStr(int n, int dot)
    { return QString::number(n * pow(10, -dot), 'f', dot); }
    static QString getAFCStr(int n);
};

#endif // TOOL_H
