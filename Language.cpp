#include "Language.h"
#include <QFile>
#include <QSettings>
#include <QTextStream>
#include <QTranslator>
#include <QApplication>

Language::Language()
{
}

int Language::column_count = 0;
QString Language::default_language;
QString Language::current_language;
QStringList Language::names;
QStringList Language::files;
QString Language::dir;

void Language::init(const QString &dir)
{
    Language::dir = dir;
    QFile file(dir + "language_configure.txt");
    file.open(QFile::ReadOnly);
    QTextStream s(&file);
    s.setCodec("UTF-8");

    while (!s.atEnd()) {
        QString str = s.readLine();
        if (str.isEmpty() || str.at(0) == '#')
            continue;

        QStringList split = str.split(':');
        if (split.size() != 2)
            continue;

        QString str1 = split.at(0).trimmed();
        QString str2 = split.at(1).trimmed();
        if (str1 == "column_count")
            column_count = str2.toInt();
        else if (str1 == "default")
            default_language = str2;
        else {
            names.append(str1);
            files.append(str2);
        }
    }

    QSettings settings(dir + "language.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    QString langFile = settings.value("language").toString();
    if (langFile.isEmpty() || files.indexOf(langFile) < 0) {
        langFile = default_language;
        if (langFile.isEmpty() && !files.isEmpty())
            langFile = files.at(0);
    }

    QTranslator *translator = new QTranslator(qApp);
    translator->load(dir + langFile);
    qApp->installTranslator(translator);

    current_language = langFile;
}

void Language::setCurrentLanguage(const QString &file)
{
    QSettings settings(dir + "language.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    settings.setValue("language", file);
    settings.sync();
#ifdef __ARM_EABI__
    system("sync");
#endif
}
