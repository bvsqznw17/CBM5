#include "KeyboardInputInfo.h"

KbInputInfo::KbInputInfo()
{
    m_isminus = true;
    m_isdot = true;
    m_minlen = 0;
    m_maxlen = 32767;
    m_type = 0;
}

KbInputInfo::KbInputInfo(const QString &prompt, bool isminus, bool isdot,
                         int minlen, int maxlen, const QString &rangeElse,
                         const QString &value) :
    m_value(value),
    m_prompt(prompt),
    m_rangePair(getRangePair(prompt)),
    m_rangeElse(rangeElse)
{
    m_type = '0';
    m_isminus = isminus;
    m_isdot = isdot;
    m_minlen = minlen;
    m_maxlen = maxlen;
}

KbInputInfo::KbInputInfo(const QString &prompt, const QRegExp &rangeRx,
                         int minlen, int maxlen, const QString &rangeElse,
                         const QString &value) :
    m_value(value),
    m_prompt(prompt),
    m_rangeRx(rangeRx),
    m_rangeElse(rangeElse)
{
    m_type = 'S';
    m_isminus = true;
    m_isdot = true;
    m_minlen = minlen;
    m_maxlen = maxlen;
}

KbInputInfo::KbInputInfo(const QString &range, const QString &value) :
    m_value(value)
{
    m_type = '0';
    m_isminus = true;
    m_isdot = true;
    m_minlen = 0;
    m_maxlen = 65535;
    QStringList list = getRangeList(range);
    if (list.isEmpty())
        return;

    // 数值输入
    if (list.at(0).isEmpty() || list.at(0).at(0) != 'S')
    {
        // 0:范围
        m_prompt = list.at(0);
        m_rangePair = getRangePair(list.at(0));
        if (list.size() <= 1)
        {
            // 智能解析1234
            if (list.at(0).isEmpty())
                return;
            m_isminus = false;
            m_isdot = false;
            m_minlen = 1;
            m_maxlen = 0;
            QStringList strList = list.at(0).split(QRegExp("[,~]"));
            for(int i = 0; i < strList.count(); i++) {
                strList[i].remove(QRegExp("^ *"));
                strList[i].remove(QRegExp("[^\\-\\.0-9].*"));
                m_isminus = m_isminus || strList.at(i).indexOf('-') != -1;
                m_isdot = m_isdot || strList.at(i).indexOf('.') != -1;
                if(m_maxlen < strList.at(i).length())
                    m_maxlen = strList.at(i).length();
            }
            return;
        }

        // 1:负号
        m_isminus = list.at(1).isEmpty() || list.at(1).at(0) != '0';

        if (list.size() <= 2) return;
        // 2:小数点
        m_isdot = list.at(2).isEmpty() || list.at(2).at(0) != '0';

        if (list.size() <= 3) return;
        // 3:最小长度
        m_minlen = list.at(3).toInt();

        if (list.size() <= 4) return;
        // 4:最大长度
        m_maxlen = list.at(4).toInt();

        if (list.size() <= 5) return;
        // 5:其他自定义范围规则
        m_rangeElse = list.at(5);
    }
    // 字符输入
    else
    {
        m_type = 'S';

        if (list.size() <= 1) return;
        // 1:提示信息
        m_prompt = list.at(1);

        if (list.size() <= 2) return;
        // 2:正则表达式
        m_rangeRx = QRegExp(list.at(2));

        if (list.size() <= 3) return;
        // 3:最小长度
        m_minlen = list.at(3).toInt();

        if (list.size() <= 4) return;
        // 4:最大长度
        m_maxlen = list.at(4).toInt();

        if (list.size() <= 5) return;
        // 5:其他自定义范围规则
        m_rangeElse = list.at(5);
    }
}

bool KbInputInfo::isRange(const QString &input)
{
    if (input.size() < m_minlen || input.size() > m_maxlen)
        return false;

    if (m_type == '0')
    {
        if (!m_rangeElse.isEmpty() && !rangeElse(input))
            return false;

        if (m_rangePair.isEmpty())
            return true;

        bool ok;
        double d = input.toDouble(&ok);
        if(ok == false)
            return false;

        QListIterator<QPair<double, double> > it(m_rangePair);
        while (it.hasNext())
        {
            const QPair<double, double> &p = it.next();
            if (d >= p.first && d <= p.second)
                return true;
        }
        return false;
    }
    else
    {
        if (!m_rangeElse.isEmpty() && !rangeElse(input))
            return false;

        if (m_rangeRx.isEmpty())
            return true;

        return m_rangeRx.exactMatch(input);
    }
}

QStringList KbInputInfo::getRangeList(const QString &str, const QChar &sep)
{
    QStringList list;
    int i = 0;
    int index;
    while (i < str.size())
    {
        QString s;
        if (str.at(i) != '\'')
        {
            index = str.indexOf(sep, i);
            s = str.mid(i, index - i);
            i = index < 0 ? str.size() : index + 1;
        }
        else
        {
            index = ++i;
            while(true)
            {
                if (index < str.size())
                {
                    if (str.at(index) != '\'')
                    {
                        index++;
                    }
                    else if (++index < str.size() && str.at(index) == '\'')
                    {
                        s.append(str.mid(i, index - i));
                        i = ++index;
                    }
                    else
                    {
                        s.append(str.mid(i, index - i - 1));
                        i = index;
                        index = str.indexOf(sep, i);
                        s.append(str.mid(i, index - i));
                        i = index < 0 ? str.size() : index + 1;
                        break;
                    }
                }
                else
                {
                    s.append(str.mid(i));
                    i = str.size();
                    break;
                }
            }
        }
        list.append(s);
    }
    return list;
}

QList<QPair<double, double> > KbInputInfo::getRangePair(const QString &str)
{
    QList<QPair<double, double> > list;

    QStringList exps = str.split(',');
    foreach (QString s, exps) {
        double d1, d2;
        bool ok;

        if(s.indexOf("~") != -1) {
            QStringList strList = s.split(QRegExp("~"));
            strList[0].remove(QRegExp("^ *"));
            strList[0].remove(QRegExp("[^\\-\\.0-9].*"));
            strList[1].remove(QRegExp("^ *"));
            strList[1].remove(QRegExp("[^\\-\\.0-9].*"));
            d1 = strList[0].toDouble(&ok);
            if(ok == false) return list;
            d2 = strList[1].toDouble(&ok);
            if(ok == false) return list;
        }
        else {
            s.remove(QRegExp("^ *"));
            s.remove(QRegExp("[^\\-\\.0-9].*"));
            d1 = s.toDouble(&ok);
            if(ok == false) return list;
            d2 = d1;
        }
        list.append(QPair<double, double>(d1, d2));
    }
    return list;
}

bool KbInputInfo::rangeElse(const QString &input)
{
    if (m_rangeElse == "-123") {
        for (int i = 3; i < input.size(); i++) {
            for (int j = 2; j < i; j++) {
                if (input.at(i) == input.at(j))
                    return false;
            }
        }
    }
    return true;
}
