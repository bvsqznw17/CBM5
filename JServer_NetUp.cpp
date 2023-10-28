#include "JServer_NetUp.h"
#include <QCryptographicHash>
#include <QDir>
#include <QProcess>

JServer_NetUp::JServer_NetUp(JsonTcpServer_FuncMap *map, QObject *parent) :
    QObject(parent)
{
    map->insert("net_up", std::bind(&JServer_NetUp::net_up, this, std::placeholders::_1));
}

void JServer_NetUp::net_up_clear()
{
    system("rm -rf /SoftBoot/net_up");
#ifdef __ARM_EABI__
    system("sync");
#endif
}

class Server_NetUp_Unlock {
    QMutex &m;
public:
    inline Server_NetUp_Unlock(QMutex &mutex) : m(mutex) {}
    inline ~Server_NetUp_Unlock() { m.unlock(); }
};

bool JServer_NetUp::net_up(JsonTcpServer_Socket &s)
{
    if (!mutex.tryLock()) {
        s.writeJsonResp(1, "拒绝接收");
        return true;
    }

    Server_NetUp_Unlock unlock(mutex);

    if (!checkDiskSize(1000)) {
        s.writeJsonResp(2, "存储空间不足");
        return true;
    }

    s.writeJsonResp(0, "准备接收");
    QByteArray data, data2;
    JsonTcpTool jtt_file(s.jtt);
    jtt_file.array_maxsize = 500*1000*1000;

    if (!s.jtt.waitForReadArray(s.socket, data, data2))
        return false;
    QString fn = QString::fromUtf8(data2);
    qDebug("recv fileName");
    if (!jtt_file.waitForReadArray(s.socket, data, data2))
        return false;
    QByteArray fileData = data2;
    qDebug("recv fileData");
    if (!s.jtt.waitForReadArray(s.socket, data, data2))
        return false;
    QByteArray fileMd5 = data2;
    qDebug("recv fileMd5");
    if (!checkFileMd5(fn, fileData, s.myRandom, s.key, fileMd5)) {
        s.writeJsonResp(3, "文件校验错误");
        return true;
    }

    QDir dir("/SoftBoot/net_up");
    dir.mkpath(".");
    QFile file(dir.filePath(fn));
    if (file.open(QFile::WriteOnly | QFile::Truncate))
        file.write(fileData);
    if (file.error() != QFile::NoError)
        s.writeJsonResp(-1, "接收失败");
    else {
        file.close();
#ifdef __ARM_EABI__
        system("sync");
#endif
        s.writeJsonResp(0, "接收成功");
        emit net_up_ready(fn);
    }
    return true;
}

void JServer_NetUp::makeFileMd5(
        const QString &fn, const QByteArray &data,
        const QString &rd, const QByteArray &key,
        QByteArray &md5)
{
    QByteArray fn_ba = fn.toUtf8();
    QByteArray rd_ba = rd.toUtf8();
    QByteArray md5_data;
    md5_data.reserve(fn_ba.size() + data.size() + rd_ba.size() + key.size());
    md5_data.append(fn_ba).append(data).append(rd_ba).append(key);
    md5 = QCryptographicHash::hash(md5_data, QCryptographicHash::Md5);
}

bool JServer_NetUp::checkFileMd5(
        const QString &fn, const QByteArray &data,
        const QString &rd, const QByteArray &key,
        const QByteArray &md5)
{
    if (md5.size() != 16)
        return false;
    QByteArray mmd5;
    makeFileMd5(fn, data, rd, key, mmd5);
    return mmd5 == md5;
}

bool JServer_NetUp::checkDiskSize(int m)
{
    QProcess p;
    p.start("df", QStringList() << "-m" << "/");
    p.waitForFinished();
    QString str = QString::fromUtf8(p.readAllStandardOutput());
    QStringList list = str.split('\n');

    for (int i = 0; i < list.size(); i++) {
        if (list.at(i).endsWith('/')) {
            QStringList list0 = list.at(i).split(QRegExp(" +"));
            if (list0.size() > 3)
                return list0.at(3).toInt() >= m;
        }
    }
    return false;
}
