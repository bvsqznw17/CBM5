#include "JServer_Mac.h"
#include "MTools.h"
#include <QSettings>
#include <QFile>

JServer_Mac::JServer_Mac(JsonTcpServer_FuncMap *map, QObject *parent) : QObject(parent)
{
    map->insert("mac_info", std::bind(&JServer_Mac::mac_info, this, std::placeholders::_1));
    map->insert("heart_beat", std::bind(&JServer_Mac::heart_beat, this, std::placeholders::_1));

    QSettings settings("MainProgram.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    m_name = settings.value("MAC/name").toString();
    m_cpusn = readCPUSN();
}

void JServer_Mac::setName(const QString &str)
{
    QMutexLocker locker(&m_mutex);
    m_name = str;
    MTools::setIniValue("MAC/name", str);
}

bool JServer_Mac::mac_info(JsonTcpServer_Socket &s)
{
    m_mutex.lock();
    QJsonObject jo;
    jo.insert("type", "CBM5");
    jo.insert("uuid", uuid);
    jo.insert("name", m_name);
    jo.insert("cpusn", m_cpusn);
    m_mutex.unlock();

    s.writeJsonResp(0, "", jo);
    return true;
}

bool JServer_Mac::heart_beat(JsonTcpServer_Socket &s)
{
    s.writeJsonResp(0, "");
    return true;
}

QString JServer_Mac::readCPUSN()
{
#ifdef __ARM_EABI__
    QFile file("/proc/cpuinfo");
    file.open(QFile::ReadOnly);
    QByteArray ba = file.readAll();
    file.close();

    QStringList list = QString::fromUtf8(ba).split('\n');
    for (int i = 0; i < list.size(); i++) {
        const QString &str = list.at(i);
        if (str.startsWith("Serial", Qt::CaseInsensitive)) {
            QStringList list2 = str.split(':');
            if (list2.size() > 1)
                return list2.at(1).trimmed();
        }
    }
#endif
    return "unknown";
}
