#include "Config.h"

#define ConfigDef(C,N,T,V)  key_##C##_##N = #C"/"#N; \
                            C##_##N = conf.value(key_##C##_##N, V).to##T();

Config g_conf;

Config::Config() :
    conf("MainProgramConfig.ini", QSettings::IniFormat)
{
    conf.setIniCodec("UTF-8");
    reload();
}

void Config::reload()
{
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
}

void Config::setValue(const QString &key, const QVariant &value)
{
    conf.setValue(key, value);
#ifdef __ARM_EABI__
    conf.sync();
    system("sync");
#endif
}
