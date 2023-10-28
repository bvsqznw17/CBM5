#ifndef KEYBOARDINPUTINFO_H
#define KEYBOARDINPUTINFO_H

#include <QStringList>
#include <QPair>
#include <QRegExp>

class KbInputInfo
{
public:
    inline const QString &value() const { return m_value; }
    inline void setValue(const QString &v) { m_value = v; }
    inline const QString &prompt() const { return m_prompt; }
    inline bool isminus() const { return m_isminus; }
    inline bool isdot() const { return m_isdot; }
    inline int minlen() const { return m_minlen; }
    inline int maxlen() const { return m_maxlen; }
    inline char type() const { return m_type; }

    KbInputInfo();
    KbInputInfo(const QString &prompt, bool isminus, bool isdot,
                int minlen, int maxlen, const QString &rangeElse = 0,
                const QString &value = 0);
    KbInputInfo(const QString &prompt, const QRegExp &rangeRx,
                int minlen, int maxlen, const QString &rangeElse = 0,
                const QString &value = 0);

    KbInputInfo(const QString &range, const QString &value = 0);

    bool isRange(const QString &input);

    static QStringList getRangeList(const QString &str, const QChar &sep='`');
    static QList<QPair<double, double> > getRangePair(const QString &str);

private:
    bool rangeElse(const QString &input);

private:
    QString m_value;
    QString m_prompt;
    bool m_isminus;
    bool m_isdot;
    int m_minlen;
    int m_maxlen;
    char m_type;

    QList<QPair<double, double> > m_rangePair;
    QRegExp m_rangeRx;
    QString m_rangeElse;
};

#endif // KEYBOARDINPUTINFO_H
