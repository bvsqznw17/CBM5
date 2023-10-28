#ifndef USBDEVICE_H
#define USBDEVICE_H

#include <QStringList>

#ifdef Q_OS_WIN
#include <windows.h>
typedef HANDLE USBDevice_HANDLE;
#define USBDevice_INVALID_HANDLE_VALUE INVALID_HANDLE_VALUE
#endif
#ifdef Q_OS_UNIX
#include <libusb.h>
typedef libusb_device_handle* USBDevice_HANDLE;
#define USBDevice_INVALID_HANDLE_VALUE 0
#endif

class USBDevice_Info;

class USBDevice
{
public:
    USBDevice(const USBDevice_Info &info, int timeoutms = 200);
    ~USBDevice();

    void setTimeoutms(int timeoutms);
    bool write(const char *data, int size, int *len);
    bool read(char *data, int size, int *len);

private:
    Q_DISABLE_COPY(USBDevice)
    quint16 ep;
    USBDevice_HANDLE handle;
    int toms;
    int ifn;
};

#include <QDataStream>
enum USBDevice_Type
{
    USBDevice_Type_Unknown = 0,
    USBDevice_Type_Printer
};

class USBDevice_Info
{
public:
    QString desc;
    QString sn;
    quint16 vid;
    quint16 pid;
    quint16 ep;
    USBDevice_Type type;
    USBDevice_HANDLE handle;
    USBDevice_Info() {
        vid = pid = ep = 0;
        type = USBDevice_Type_Unknown;
        handle = USBDevice_INVALID_HANDLE_VALUE;
    }
    QString getVidStr() const {
        return QString("%1").arg(vid, 4, 16, QChar('0')).toUpper();
    }
    QString getPidStr() const {
        return QString("%1").arg(pid, 4, 16, QChar('0')).toUpper();
    }
    QString getName() const {
        return QString("%1 %2").arg((!desc.isEmpty() ? desc :
                                     (QString("VID_%1&PID_%2")
                                      .arg(getVidStr(), getPidStr()))),
                                    sn);
    }
    QByteArray getSaveByteArray() const {
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out << desc << sn << vid << pid;
        return data;
    }
    bool setSaveByteArray(const QByteArray &data) {
        QDataStream in(data);
        QString d, s; quint16 v, p;
        in >> d >> s >> v >> p;
        if (in.status() == QDataStream::Ok) {
            desc = d; sn = s; vid = v; pid = p;
            return true;
        }
        return false;
    }
};

class USBDevice_Scanner
{
public:
    explicit USBDevice_Scanner();
    ~USBDevice_Scanner();

    // 根据filter获取当前设备列表
    // type=0: 获取全部，不获取handle
    // type=1: 获取全部，获取handle
    // type=2: 只获取第一个，获取handle
    // type=3: 获取指定info的一个，获取handle
    // handle要close
    QList<USBDevice_Info> getList(const QList<int> &filter = QList<int>(), int type = 0, const USBDevice_Info &info = USBDevice_Info());

private:
    int getEndPoint(const QList<int> &filter, quint16 vid, quint16 pid);

private:
    Q_DISABLE_COPY(USBDevice_Scanner)
#ifdef Q_OS_UNIX
    libusb_context *ctx;
#endif
};

#endif // USBDEVICE_H
