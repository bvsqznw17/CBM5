#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QStringList>

class Language
{
public:
    Language();

    static int column_count;
    static QString default_language;
    static QString current_language;
    static QStringList names;
    static QStringList files;
    static QString dir;
    static void init(const QString &dir = "./language/");
    static void setCurrentLanguage(const QString &file);
};

#endif // LANGUAGE_H
