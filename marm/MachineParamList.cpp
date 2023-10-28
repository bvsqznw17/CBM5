#include "MachineParamList.h"
#include <qmath.h>
#include <QStringList>
#include <QVector>

MachineParamList::MachineParamList(int count)
{
    m_size = count * 2;
    m_data = new char[m_size];
    memset(m_data, 0, m_size);
}

MachineParamList::~MachineParamList()
{
    delete[] m_data;
}

void MachineParamList::setData(const MRegAreas &regAreas, const QList<QByteArray> &datas)
{
    QListIterator<QByteArray> datai(datas);
    foreach (const MRegArea &area, regAreas.v) {
        if (!datai.hasNext()) break;
        const QByteArray &data = datai.next();
        int index = area.addr * 2;
        int len = area.count * 2;
        if (index >= 0 && len > 0 && index + len <= m_size && len == data.length())
            memcpy(m_data + index, data.constData(), len);
    }
}

MParam::MParam(MachineParamList *owner)
{
    m_owner = owner;
}

MParam::~MParam() {
}

void MParam::setValue(const QVariant &value) const
{
    QByteArray data = regFormat(value);
    MRegArea area(getAddr(), getCount());
    m_owner->setData(QList<MRegArea>() << area, QList<QByteArray>() << data);
}

void MParam::setValue(const QVariant &value, int i) const
{
    QVariantList list = this->value().toList();
    list[i] = value;
    setValue(list);
}

QByteArray MParam::regFormat(const QVariant &value) const
{
    int len = getCount() * 2;
    QByteArray ba(len, Qt::Uninitialized);
    char *data = ba.data();
    int type = getType();

    switch (type) {
    case U16:
    case S16: {
        int val = QVariant(value.toDouble() * pow(10, getPrec())).toInt();
        setByte(data, (quint16)val);
        break;
    }
    case U32:
    case S32: {
        int val = QVariant(value.toDouble() * pow(10, getPrec())).toInt();
        setByte(data, (quint32)val);
        break;
    }
    case U64:
    case S64: {
        qint64 val = value.toLongLong();
        setByte(data, (quint64)val);
        break;
    }
    case F32: {
        float val = value.toFloat();
        setByte(data, val);
        break;
    }
    case F64: {
        double val = value.toDouble();
        setByte(data, val);
        break;
    }
    case Chars:
    case Bytes: {
        QByteArray val;
        if (type == Chars) val = value.toString().toUtf8(); // 字符串是UTF-8编码
        else val = value.toByteArray();
        int olen = val.length();
        val.resize(len);
        if (len - olen > 0)
            memset(val.data() + olen, 0, len - olen);
        for (int i = 0; i < len; i += 2) {
            data[i] = val.at(i+1);
            data[i+1] = val.at(i);
        }
        break;
    }
    default:
    {
        int pos = 0;
        QVariantList list = (value.type() == QVariant::List) ?
                    value.toList() : QVariantList() << value;
        switch (type) {
        case U8s: case S8s:
            for (int i = 0; i < list.size() && pos < len; i++, pos+=2) {
                data[pos+1] = QVariant(list.at(i++).toDouble() * pow(10, getPrec())).toInt();
                data[pos] = i < list.size() ?
                            QVariant(list.at(i).toDouble() * pow(10, getPrec())).toInt() : 0;
            }
            break;
        case U16s: case S16s:
            for (int i = 0; i < list.size() && pos < len; i++, pos+=2) {
                int val = QVariant(list.at(i).toDouble() * pow(10, getPrec())).toInt();
                setByte(data+pos, (quint16)val);
            }
            break;
        case U32s: case S32s:
            for (int i = 0; i < list.size() && pos < len; i++, pos+=4) {
                int val = QVariant(list.at(i).toDouble() * pow(10, getPrec())).toInt();
                setByte(data+pos, (quint32)val);
            }
            break;
        case U64s: case S64s:
            for (int i = 0; i < list.size() && pos < len; i++, pos+=8) {
                qint64 val = list.at(i).toLongLong();
                setByte(data+pos, (quint64)val);
            }
            break;
        case F32s:
            for (int i = 0; i < list.size() && pos < len; i++, pos+=4) {
                float val = list.at(i).toFloat();
                setByte(data+pos, val);
            }
            break;
        case F64s:
            for (int i = 0; i < list.size() && pos < len; i++, pos+=8) {
                double val = list.at(i).toDouble();
                setByte(data+pos, val);
            }
            break;
        }
        if (pos < len)
            memset(data+pos, 0, len - pos);
    }
    }

    return ba;
}

QVariant MParam::valuenp() const
{
    const char *src = m_owner->data() + getAddr() * 2;
    int len = getCount() * 2;
    char data0[8];
    char *data = data0;
    int type = getType();

    switch (type) {
    case U16:
        getByte2(src, data);
        return *(quint16*)data;
    case S16:
        getByte2(src, data);
        return *(qint16*)data;
    case U32:
        getByte4(src, data);
        return *(quint32*)data;
    case S32:
        getByte4(src, data);
        return *(qint32*)data;
    case U64:
        getByte8(src, data);
        return *(quint64*)data;
    case S64:
        getByte8(src, data);
        return *(qint64*)data;
    case F32:
        getByte4(src, data);
        return *(float*)data;
    case F64:
        getByte8(src, data);
        return *(double*)data;
    case Chars: // 字符串将作为字节数组返回
    case Bytes: {
        char data[len];
        for (int i = 0; i < len; i += 2) {
            data[i] = src[i+1];
            data[i+1] = src[i];
        }
        return QByteArray(data, len);
    }
    default:
    {
        QVariantList list;
        switch (type) {
        case U8s:
            list.reserve(len);
            for (int i = 0; i < len; i+=2) {
                list.append((quint8)src[i+1]);
                list.append((quint8)src[i]);
            }
            return list;
        case S8s:
            list.reserve(len);
            for (int i = 0; i < len; i+=2) {
                list.append((qint8)src[i+1]);
                list.append((qint8)src[i]);
            }
            return list;
        case U16s:
            list.reserve(len / 2);
            for (int i = 0; i < len; i+=2) {
                getByte2(src + i, data);
                list.append(*(quint16*)data);
            }
            return list;
        case S16s:
            list.reserve(len / 2);
            for (int i = 0; i < len; i+=2) {
                getByte2(src + i, data);
                list.append(*(qint16*)data);
            }
            return list;
        case U32s:
            list.reserve(len / 4);
            for (int i = 0; i < len; i+=4) {
                getByte4(src + i, data);
                list.append(*(quint32*)data);
            }
            return list;
        case S32s:
            list.reserve(len / 4);
            for (int i = 0; i < len; i+=4) {
                getByte4(src + i, data);
                list.append(*(qint32*)data);
            }
            return list;
        case U64s:
            list.reserve(len / 8);
            for (int i = 0; i < len; i+=8) {
                getByte8(src + i, data);
                list.append(*(quint64*)data);
            }
            return list;
        case S64s:
            list.reserve(len / 8);
            for (int i = 0; i < len; i+=8) {
                getByte8(src + i, data);
                list.append(*(qint64*)data);
            }
            return list;
        case F32s:
            list.reserve(len / 4);
            for (int i = 0; i < len; i+=4) {
                getByte4(src + i, data);
                list.append(*(float*)data);
            }
            return list;
        case F64s:
            list.reserve(len / 8);
            for (int i = 0; i < len; i+=8) {
                getByte8(src + i, data);
                list.append(*(double*)data);
            }
            return list;
        }
    }
    }
    return QVariant();
}

QVariant MParam::value() const
{
    const char *src = m_owner->data() + getAddr() * 2;
    int len = getCount() * 2;
    char data0[8];
    char *data = data0;
    int type = getType();

    switch (type) {
    case U16:
        getByte2(src, data);
        return (double)*(quint16*)data * pow(10, -getPrec());
    case S16:
        getByte2(src, data);
        return (double)*(qint16*)data * pow(10, -getPrec());
    case U32:
        getByte4(src, data);
        return (double)*(quint32*)data * pow(10, -getPrec());
    case S32:
        getByte4(src, data);
        return (double)*(qint32*)data * pow(10, -getPrec());
    case U64: // 64位整型不能乘精度系数
        getByte8(src, data);
        return *(quint64*)data;
    case S64:
        getByte8(src, data);
        return *(qint64*)data;
    case F32: // 浮点型无需乘精度系数
        getByte4(src, data);
        return *(float*)data;
    case F64:
        getByte8(src, data);
        return *(double*)data;
    case Chars: { // 字符串是UTF-8编码
        char data[len];
        for (int i = 0; i < len; i += 2) {
            data[i] = src[i+1];
            data[i+1] = src[i];
            if (data[i] == 0) return QString::fromUtf8(data, i);
            if (data[i + 1] == 0) return QString::fromUtf8(data, i + 1);
        }
        return QString::fromUtf8(data, len);
    }
    case Bytes: {
        char data[len];
        for (int i = 0; i < len; i += 2) {
            data[i] = src[i+1];
            data[i+1] = src[i];
        }
        return QByteArray(data, len);
    }
    default:
    {
        QVariantList list;
        switch (type) {
        case U8s:
            list.reserve(len);
            for (int i = 0; i < len; i+=2) {
                list.append((quint8)src[i+1] * pow(10, -getPrec()));
                list.append((quint8)src[i] * pow(10, -getPrec()));
            }
            return list;
        case S8s:
            list.reserve(len);
            for (int i = 0; i < len; i+=2) {
                list.append((qint8)src[i+1] * pow(10, -getPrec()));
                list.append((qint8)src[i] * pow(10, -getPrec()));
            }
            return list;
        case U16s:
            list.reserve(len / 2);
            for (int i = 0; i < len; i+=2) {
                getByte2(src + i, data);
                list.append(*(quint16*)data * pow(10, -getPrec()));
            }
            return list;
        case S16s:
            list.reserve(len / 2);
            for (int i = 0; i < len; i+=2) {
                getByte2(src + i, data);
                list.append(*(qint16*)data * pow(10, -getPrec()));
            }
            return list;
        case U32s:
            list.reserve(len / 4);
            for (int i = 0; i < len; i+=4) {
                getByte4(src + i, data);
                list.append(*(quint32*)data * pow(10, -getPrec()));
            }
            return list;
        case S32s:
            list.reserve(len / 4);
            for (int i = 0; i < len; i+=4) {
                getByte4(src + i, data);
                list.append(*(qint32*)data * pow(10, -getPrec()));
            }
            return list;
        case U64s:
            list.reserve(len / 8);
            for (int i = 0; i < len; i+=8) {
                getByte8(src + i, data);
                list.append(*(quint64*)data * pow(10, -getPrec()));
            }
            return list;
        case S64s:
            list.reserve(len / 8);
            for (int i = 0; i < len; i+=8) {
                getByte8(src + i, data);
                list.append(*(qint64*)data);
            }
            return list;
        case F32s:
            list.reserve(len / 4);
            for (int i = 0; i < len; i+=4) {
                getByte4(src + i, data);
                list.append(*(float*)data);
            }
            return list;
        case F64s:
            list.reserve(len / 8);
            for (int i = 0; i < len; i+=8) {
                getByte8(src + i, data);
                list.append(*(double*)data);
            }
            return list;
        }
    }
    }
    return QVariant();
}

QString MParam::getUStr_() const
{
    QString ustr = getUStr();
    if (!ustr.isEmpty())
        ustr = QString(" ").append(ustr);
    return ustr;
}

QString MParam::vStr(const QVariant &value) const
{
    int type = getType();
    switch (type) {
    case U16: case S16: case U32: case S32: case F32: case F64:
        return QString::number(value.toDouble(), 'f', getPrec());
    case U64:
        return QString::number(value.toULongLong());
    case S64:
        return QString::number(value.toLongLong());
    case Bytes: // 字节数组将转为十六进制大写字符串
        return value.toByteArray().toHex().toUpper();
    case Chars:
        return value.toString();
    default:
        QString str;
        QVariantList list;
        if (!(value.type() == QVariant::List))
            list << value;
        else {
            list = value.toList();
            if (list.isEmpty())
                return "";
        }
        switch (type) {
        case U8s: case S8s: case U16s: case S16s:
        case U32s: case S32s: case F32s: case F64s:
            str = QString::number(list.at(0).toDouble(), 'f', getPrec());
            for (int i = 1; i < list.size(); i++)
                str.append(QChar(',')).append(QString::number(list.at(i).toDouble(), 'f', getPrec()));
            return str;
        case U64s:
            str = QString::number(list.at(0).toULongLong());
            for (int i = 1; i < list.size(); i++)
                str.append(QChar(',')).append(QString::number(list.at(i).toULongLong()));
            return str;
        case S64s:
            str = QString::number(list.at(0).toLongLong());
            for (int i = 1; i < list.size(); i++)
                str.append(QChar(',')).append(QString::number(list.at(i).toLongLong()));
            return str;
        default:
            return "";
        }
    }
}

QVariant MParam::sVal(const QString &string) const
{
    int type = getType();
    switch (type) {
    case U16: case S16: case U32: case S32:
    case U64: case S64: case F32: case F64:
    case Chars:
        return string;
    case Bytes:
        return QByteArray::fromHex(string.toLatin1());
    case U8s: case S8s: case U16s: case S16s:
    case U32s: case S32s: case F32s: case F64s: {
        QVariantList list;
        QStringList slist = string.split(',');
        foreach (const QString &str, slist)
            list.append(str);
        return list;
    }
    }
    return QVariant();
}

QStringList MParam::vStrList(const QVariantList &value) const
{
    QStringList list;
    list.reserve(value.size());
    foreach (const QVariant &v, value) {
        list.append(vStr(v));
    }
    return list;
}

QVariantList MParam::sValList(const QStringList &string) const
{
    QVariantList list;
    list.reserve(string.size());
    foreach (const QString &s, string) {
        list.append(sVal(s));
    }
    return list;
}

QStringList MParam::vStruList(const QVariantList &value) const
{
    QStringList list;
    list.reserve(value.size());
    foreach (const QVariant &v, value) {
        list.append(vStru(v));
    }
    return list;
}

QStringList MParam::vStrUList(const QVariantList &value) const
{
    QStringList list;
    list.reserve(value.size());
    foreach (const QVariant &v, value) {
        list.append(vStrU(v));
    }
    return list;
}

QVariant MParam::vP(const QVariant &value) const
{
    switch (getType()) {
    case U16: case S16: case U32: case S32:
        // 64位整型不能乘精度系数，浮点型无需乘精度系数
        return value.toDouble() * pow(10, -getPrec());
    case U8s: case S8s: case U16s: case S16s: case U32s: case S32s: {
        if (!(value.type() == QVariant::List))
            return value.toDouble() * pow(10, -getPrec());
        QVariantList list = value.toList();
        if (list.isEmpty())
            return value;
        QVariantList rlist;
        for (int i = 1; i < list.size(); i++) {
            rlist.append(list.at(i).toDouble() * pow(10, -getPrec()));
        }
        return rlist;
    }
    case Chars: {
        // valuenp()是获取不乘精度系数的值，那里是将字符串当作字节数组，
        // 所以这里应该是将字节数组转字符串，字符串是UTF-8编码
        QByteArray ba = value.toByteArray();
        int len = 0;
        while (len < ba.size() && ba.at(len))
            len++;
        return QString::fromUtf8(ba.constData(), len);
        }
    default:
        return value;
    }
}

void MParam::setByte(char *data, quint16 v)
{
    data[0] = v >> 8;
    data[1] = v >> 0;
}

void MParam::setByte(char *data, quint32 v)
{
    setByte(&data[2], (quint16)(v >> 16));
    setByte(&data[0], (quint16)(v >> 0));
}

void MParam::setByte(char *data, quint64 v)
{
    setByte(&data[6], (quint16)(v >> 48));
    setByte(&data[4], (quint16)(v >> 32));
    setByte(&data[2], (quint16)(v >> 16));
    setByte(&data[0], (quint16)(v >> 0));
}

void MParam::setByte(char *data, float v)
{
    char *p = (char*)&v;
    quint32 i = *(quint32*)p;
    setByte(data, i);
}

void MParam::setByte(char *data, double v)
{
    char *p = (char*)&v;
    quint64 i = *(quint64*)p;
    setByte(data, i);
}

void MParam::getByte2(const char *src, char *dst)
{
    quint16 v;
    v  = (quint16)src[0] << 8 & 0xFF00;
    v |= (quint16)src[1] << 0 & 0xFF;
    *(quint16*)dst = v;
}

void MParam::getByte4(const char *src, char *dst)
{
    quint16 v1;
    quint16 v2;
    getByte2(src + 0, (char*)&v1);
    getByte2(src + 2, (char*)&v2);
    quint32 v;
    v  = (quint32)v2 << 16 & 0xFFFF0000;
    v |= (quint32)v1 <<  0 & 0xFFFF;
    *(quint32*)dst = v;
}

void MParam::getByte8(const char *src, char *dst)
{
    quint16 v1;
    quint16 v2;
    quint16 v3;
    quint16 v4;
    getByte2(src + 0, (char*)&v1);
    getByte2(src + 2, (char*)&v2);
    getByte2(src + 4, (char*)&v3);
    getByte2(src + 6, (char*)&v4);
    quint64 v;
    v  = (quint64)v4 << 48 & 0xFFFF000000000000ULL;
    v |= (quint64)v3 << 32 & 0xFFFF00000000ULL;
    v |= (quint64)v2 << 16 & 0xFFFF0000ULL;
    v |= (quint64)v1 <<  0 & 0xFFFFULL;
    *(quint64*)dst = v;
}

void MRegRWInfo::span(MParam *m1, MParam *m2, int rd, int wd)
{
    addr = m1->getAddr();
    count = m2->getAddr() + m2->getCount() - addr;
    if (rd < 0) {
        int i1 = m1->getRDelay();
        int i2 = m2->getRDelay();
        rdelay = i1 > i2 ? i1 : i2;
    }
    else
        rdelay = rd;
    if (wd < 0) {
        int i1 = m1->getWDelay();
        int i2 = m2->getWDelay();
        wdelay = i1 > i2 ? i1 : i2;
    }
    else
        wdelay = wd;
}

bool lessThan(const MRegRWInfo &m1, const MRegRWInfo &m2) {
    return (m1.addr < m2.addr);
}

// 不考虑count为负的情况
#define MRegAreas_IsIn_For(Addr, Count) {\
int m1_addr = Addr;\
int m1_count = Count;\
foreach (const MRegArea &area, v) {\
    int m2_addr = area.addr;\
    int m2_count = area.count;\
    if (m1_addr >= m2_addr && m1_addr < m2_addr + m2_count)\
        return true;\
    if (m2_addr >= m1_addr && m2_addr < m1_addr + m1_count)\
        return true;\
}\
}\
return false;

bool MRegAreas::isIn(const MParam *mp) const
{
    MRegAreas_IsIn_For(mp->getAddr(), mp->getCount());
}

bool MRegAreas::isIn(const MRegRWInfo &m) const
{
    MRegAreas_IsIn_For(m.addr, m.count);
}

bool MRegAreas::isIn(const QList<const MParam *> &mps) const
{
    foreach (const MParam *mp, mps)
        MRegAreas_IsIn_For(mp->getAddr(), mp->getCount());
}

bool MRegAreas::isIn(const QList<MRegRWInfo> &list) const
{
    foreach (const MRegRWInfo &m, list)
        MRegAreas_IsIn_For(m.addr, m.count);
}

bool MRegAreas::isIn(const MRegAreas &regAreas) const
{
    foreach (const MRegArea &m, regAreas.v)
        MRegAreas_IsIn_For(m.addr, m.count);
}

#undef MRegAreas_IsIn_For

// 不考虑addr大于addr2的情况
#define MRegAreas_IsCon_For(Addr, Count) {\
int m1_addr = Addr;\
int m1_count = Count;\
QVector<QPair<int, int> > list, list0;\
list.append(QPair<int, int>(m1_addr, m1_addr + m1_count));\
list0.reserve(list.size() + 1);\
foreach (const MRegArea &area, v) {\
    int m2_addr = area.addr;\
    int m2_addr2 = area.addr + area.count;\
    for (int i = 0; i < list.size(); i++) {\
        const QPair<int, int> &m = list.at(i);\
        if (m2_addr > m.first)\
            list0.append(QPair<int, int>(m.first, qMin(m.second, m2_addr)));\
        if (m2_addr2 < m.second)\
            list0.append(QPair<int, int>(qMax(m.first, m2_addr2), m.second));\
    }\
    list = list0;\
    if (list.isEmpty())\
        break;\
    list0.clear();\
    list0.reserve(list.size() + 1);\
}\
if (!list.isEmpty())\
    return false;\
}\
return true;

bool MRegAreas::isCon(const MParam *mp) const
{
    MRegAreas_IsCon_For(mp->getAddr(), mp->getCount());
}

bool MRegAreas::isCon(const MRegRWInfo &m) const
{
    MRegAreas_IsCon_For(m.addr, m.count);
}

bool MRegAreas::isCon(const QList<const MParam *> &mps) const
{
    foreach (const MParam *mp, mps)
        MRegAreas_IsCon_For(mp->getAddr(), mp->getCount());
}

bool MRegAreas::isCon(const QList<MRegRWInfo> &list) const
{
    foreach (const MRegRWInfo &m, list)
        MRegAreas_IsCon_For(m.addr, m.count);
}

bool MRegAreas::isCon(const MRegAreas &regAreas) const
{
    foreach (const MRegArea &m, regAreas.v)
        MRegAreas_IsCon_For(m.addr, m.count);
}

#undef MRegAreas_IsCon_For
