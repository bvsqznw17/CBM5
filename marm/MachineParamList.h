#ifndef MACHINEPARAMLIST_H
#define MACHINEPARAMLIST_H

// "MachineParamList"和"MParam"，一般通过指针传递给其他对象使用
// 所以在销毁"MachineParamList"和"MParam"前，要先销毁可能正在使用的对象

#include <QVariant>
#include <QStringList>
class MParam;
class MRegAreas;

// 机器参数表，下位机寄存器的本地缓存
class MachineParamList
{
public:
    MachineParamList(int count);
    ~MachineParamList();

    inline int size() const { return m_size; } // 寄存器缓存大小（寄存器个数的两倍）
    inline char *data() const { return m_data; } // 寄存器缓存数据

    void setData(const MRegAreas &regAreas, const QList<QByteArray> &datas); // 修改缓存

    // 规定要先将firstReadMpslist里的参数按顺序读取完，才能进行其他读写操作
    QList<QList<const MParam *> > firstReadMpslist;

private:
    char *m_data;
    int m_size;
};

// 机器参数，包含参数的各种信息
class MParam
{
private:
    MachineParamList *m_owner;

public:
    enum Type { U16, S16, U32, S32, U64, S64, F32, F64, Chars, Bytes,
              U8s, S8s, U16s, S16s, U32s, S32s, U64s, S64s, F32s, F64s};

    MParam(MachineParamList *owner);
    virtual ~MParam();

    // 属于哪个“机器参数表”，该参数的值存储在哪里
    inline MachineParamList *owner() const { return m_owner; }

    virtual bool isValid() const = 0; // 是否有效
    virtual int getAddr() const = 0; // 寄存器起始地址
    virtual int getCount() const = 0; // 寄存器个数
    virtual int getRDelay() const = 0; // 最大读取延时
    virtual int getWDelay() const = 0; // 最大写入延时
    virtual int getType() const = 0; // 数据类型
    virtual int getPrec() const = 0; // 小数位数
    virtual QString getUStr() const = 0; // 单位
    virtual QString getRange() const = 0; // 取值范围

    // 修改该参数的值，修改相应寄存器缓存
    void setValue(const QVariant &value) const;
    void setValuenp(const QVariant &value) const { setValue(vP(value)); }
    void setValue(const QVariant &value, int i) const;
    void setValuenp(const QVariant &value, int i) const { setValue(vP(value), i); }

    // 把value转成寄存器格式
    QByteArray regFormat(const QVariant &value) const;

    // 该参数的值，没有乘精度系数（精度系数=pow(10,-getPrec())）
    QVariant valuenp() const;
    // 该参数的值，乘了精度系数
    QVariant value() const;
    // 空格加单位（没有单位便不加空格）
    QString getUStr_() const;

    inline QString vStr() const { return vStr(value()); }
    inline QString vStru() const { return vStru(value()); }
    inline QString vStrU() const { return vStrU(value()); }

    inline QStringList vStrList() const { return vStrList(value().toList()); }
    inline QStringList vStruList() const { return vStruList(value().toList()); }
    inline QStringList vStrUList() const { return vStrUList(value().toList()); }

    // 把value转成字符串，保留相应小数位
    virtual QString vStr(const QVariant &value) const;
    // 把string转换为值
    virtual QVariant sVal(const QString &string) const;
    // 把value转成字符串，保留相应小数位，加单位
    inline QString vStru(const QVariant &value) const
    { return vStr(value).append(getUStr()); }
    // 把value转成字符串，保留相应小数位，加空格加单位
    inline QString vStrU(const QVariant &value) const
    { return vStr(value).append(getUStr_()); }

    virtual QStringList vStrList(const QVariantList &value) const;
    virtual QVariantList sValList(const QStringList &string) const;
    QStringList vStruList(const QVariantList &value) const;
    QStringList vStrUList(const QVariantList &value) const;

    QVariant vP(const QVariant &value) const; // 把value乘精度系数
    inline QString vPS(const QVariant &value) const { return vStr(vP(value)); }
    inline QString vPSu(const QVariant &value) const { return vStru(vP(value)); }
    inline QString vPSU(const QVariant &value) const { return vStrU(vP(value)); }

    inline int vInt() { return value().toInt(); }
    inline uint vUInt() { return value().toUInt(); }
    inline int vIntNp() { return valuenp().toInt(); }
    inline uint vUIntNp() { return valuenp().toUInt(); }
    inline qlonglong vLLong() { return value().toLongLong(); }
    inline qulonglong vULLong() { return value().toULongLong(); }
    inline float vFloat() { return value().toFloat(); }
    inline double vDouble() { return value().toDouble(); }
    inline QByteArray vByteArray() { return value().toByteArray(); }
    inline QString vString() { return value().toString(); }
    inline QVariantList vList() { return value().toList(); }

    static void setByte(char *data, quint16 v);
    static void setByte(char *data, quint32 v);
    static void setByte(char *data, quint64 v);
    static void setByte(char *data, float v);
    static void setByte(char *data, double v);
    static void getByte2(const char *src, char *dst);
    static void getByte4(const char *src, char *dst);
    static void getByte8(const char *src, char *dst);
};

// 寄存器读写信息
struct MRegRWInfo {
    int addr; // 寄存器起始地址
    int count; // 寄存器个数
    int rdelay; // 最大读取延时
    int wdelay; // 最大写入延时
    QByteArray data; // 要写入的数据

    MRegRWInfo() {}
    MRegRWInfo(int addr_, int count_, int rdelay_, int wdelay_,
               QByteArray data_ = QByteArray()) :
        addr(addr_), count(count_),
        rdelay(rdelay_), wdelay(wdelay_),
        data(data_) {}

    void span(MParam *m1, MParam *m2, int rd = -1, int wd = -1);
};

bool lessThan(const MRegRWInfo &m1, const MRegRWInfo &m2);

// 寄存器区域
struct MRegArea
{
    int addr;
    int count;
    MRegArea() {}
    MRegArea(int addr_, int count_) {
        addr = addr_;
        count = count_;
    }
    void span(MParam *m1, MParam *m2) {
        addr = m1->getAddr();
        count = m2->getAddr() + m2->getCount() - addr;
    }
};

// 多块寄存器区域
class MRegAreas
{
public:
    QList<MRegArea> v;
    MRegAreas() {}
    MRegAreas(int addr, int count) { v.append(MRegArea(addr, count)); }
    MRegAreas(const MRegArea &ra) { v.append(ra); }
    MRegAreas(const QList<MRegArea> &list) : v(list) {}

    // v是否与该寄存器区域相交
    bool isIn(const MParam *mp) const;
    bool isIn(const MRegRWInfo &info) const;
    bool isIn(const QList<const MParam *> &mps) const;
    bool isIn(const QList<MRegRWInfo> &list) const;
    bool isIn(const MRegAreas &regAreas) const;

    // v是否包含该寄存器区域
    bool isCon(const MParam *mp) const;
    bool isCon(const MRegRWInfo &info) const;
    bool isCon(const QList<const MParam *> &mps) const;
    bool isCon(const QList<MRegRWInfo> &list) const;
    bool isCon(const MRegAreas &regAreas) const;
};

#endif // MACHINEPARAMLIST_H
