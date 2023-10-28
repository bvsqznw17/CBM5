#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>
#include <QStringList>

#define ConfigDef(C,N,T,V)  ConfigDef_##T C##_##N; \
                            QString key_##C##_##N;

typedef bool ConfigDef_Bool;
typedef int ConfigDef_Int;
typedef long long ConfigDef_LongLong;
typedef float ConfigDef_Flaot;
typedef double ConfigDef_Double;
typedef QString ConfigDef_String;
typedef QStringList ConfigDef_StringList;
typedef QByteArray ConfigDef_ByteArray;

#define Net_Role_Server 0
#define Net_Role_Client 1

class Config
{
public:
    ConfigDef(Printer, IsHuiTui, Bool, false);   // 是否启动回退
    ConfigDef(Printer, HuiDu, Int, 100);         // 打印灰度
    ConfigDef(Printer, PageType, Int, 1);        // 纸张类型
    ConfigDef(Printer, StartPos, Int, 0);        // 打印初始位置
    ConfigDef(Printer, LabLength, Int, 0);       // 标签长度
    ConfigDef(Printer, LabInterval, Int, 2);     // 标签间隙
    ConfigDef(Printer, PenYinDir, Int, 0);       // 喷印方向
    ConfigDef(Printer, HorOffset, Int, 0);       // 水平位移
    ConfigDef(Printer, VerOffset, Int, 0);       // 垂直方向位移

    ConfigDef(PrinterSel, M1, String, "");   // 打印机选择，对应工作模式
    ConfigDef(PrinterSel, M2, String, "");
    ConfigDef(PrinterSel, M3, String, "");
    ConfigDef(PrinterSel, M1_Info, ByteArray, "");
    ConfigDef(PrinterSel, M2_Info, ByteArray, "");
    ConfigDef(PrinterSel, M3_Info, ByteArray, "");

public:
    Config();
    QSettings conf;
    void reload();
    void setValue(const QString &key, const QVariant &value);
};

extern Config g_conf;

#define g_conf_SetValue(C,N,V)  { if (g_conf.C##_##N != V) { \
                                    g_conf.C##_##N = V; \
                                    g_conf.setValue(g_conf.key_##C##_##N, g_conf.C##_##N); \
                                    } }

#undef ConfigDef
#endif // CONFIG_H
