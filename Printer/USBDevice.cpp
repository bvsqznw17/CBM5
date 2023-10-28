#include "USBDevice.h"

#ifdef Q_OS_WIN
#include <windows.h>
#include <winbase.h>
#include <string>
#include <IOSTREAM>
#include <winioctl.h>
#include <setupapi.h>
#include <stdio.h>

USBDevice::USBDevice(const USBDevice_Info &info, int timeoutms)
{
    ep = info.ep;
    handle = info.handle;

    if (handle == USBDevice_INVALID_HANDLE_VALUE)
        return;

    setTimeoutms(timeoutms);
}

USBDevice::~USBDevice()
{
    if (handle == USBDevice_INVALID_HANDLE_VALUE)
        return;

    CloseHandle(handle);
}

void USBDevice::setTimeoutms(int timeoutms)
{
    if (handle == USBDevice_INVALID_HANDLE_VALUE)
        return;

    if (toms != timeoutms) {
        toms = timeoutms;
        COMMTIMEOUTS tmouts;
        tmouts.ReadIntervalTimeout = timeoutms;
        tmouts.ReadTotalTimeoutMultiplier = timeoutms;
        tmouts.ReadTotalTimeoutConstant = timeoutms;
        tmouts.WriteTotalTimeoutConstant = timeoutms;
        tmouts.WriteTotalTimeoutMultiplier = timeoutms;
        SetCommTimeouts(handle, &tmouts);
    }
}

bool USBDevice::write(const char *data, int size, int *len)
{
    if (handle == USBDevice_INVALID_HANDLE_VALUE)
        return false;

    DWORD l;
    BOOL b = WriteFile(handle, data, size, &l, NULL);
    if (len) *len = l;
    return b;
}

bool USBDevice::read(char *data, int size, int *len)
{
    if (handle == USBDevice_INVALID_HANDLE_VALUE)
        return false;

    DWORD l;
    BOOL b = ReadFile(handle, data, size, &l, NULL);
    if (len) *len = l;
    return b;
}

// SetupDiGetInterfaceDeviceDetail所需要的输出长度，定义足够大
#define INTERFACE_DETAIL_SIZE 1024

USBDevice_Scanner g_usbdev_scanner;

USBDevice_Scanner::USBDevice_Scanner()
{
}

USBDevice_Scanner::~USBDevice_Scanner()
{
}

QList<USBDevice_Info> USBDevice_Scanner::getList(const QList<int> &filter, int type, const USBDevice_Info &info)
{
    QList<USBDevice_Info> list;

    const GUID USB_GUID = {0xa5dcbf10, 0x6530, 0x11d2, {0x90, 0x1f, 0x00, 0xc0, 0x4f, 0xb9, 0x51, 0xed}};
    LPGUID lpGuid = (LPGUID)&USB_GUID;
    HDEVINFO hDevInfoSet;
    SP_DEVINFO_DATA spDevInfoData;
    SP_DEVICE_INTERFACE_DATA ifData;
    PSP_DEVICE_INTERFACE_DETAIL_DATA pDetail;
    int nTotle;
    BOOL bResult;
    WCHAR buf[MAX_PATH];

    // 取得一个该GUID相关的设备信息集句柄
    hDevInfoSet = ::SetupDiGetClassDevs(lpGuid, // class GUID
                                        NULL,   // 无关键字
                                        NULL,   // 不指定父窗口句柄
                                        DIGCF_PRESENT | DIGCF_DEVICEINTERFACE); // 目前存在的设备
    if (hDevInfoSet == INVALID_HANDLE_VALUE)
        return list;

    // 申请设备接口数据空间
    pDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)::GlobalAlloc(LMEM_ZEROINIT, INTERFACE_DETAIL_SIZE);
    pDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
    nTotle = -1;
    bResult = TRUE;

    // 设备序号=0,1,2... 逐一测试设备接口，到失败为止
    while (1) {
        USBDevice_Info info0;
        nTotle++;
        spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

        // 枚举符合该GUID的设备接口
        bResult = ::SetupDiEnumDeviceInfo(
                    hDevInfoSet,        // 设备信息集句柄
                    (ULONG)nTotle,      // 设备信息集里的设备序号
                    &spDevInfoData);    // 设备接口信息
        if (!bResult)
            break;

        // 得到设备实例的唯一标识ID(由注册表路径组成)
        bResult = SetupDiGetDeviceInstanceId(hDevInfoSet, &spDevInfoData,
                                             buf, sizeof(buf), NULL);
        if (!bResult)
            continue;
        else {
            // 解析VID&PID和序列号
            QStringList sl = QString::fromWCharArray(buf).split('\\');
            if (sl.size() >= 3) {
                info0.vid = sl.at(1).mid(4, 4).toShort(0, 16);
                info0.pid = sl.at(1).mid(13, 4).toShort(0, 16);
                info0.sn = sl.at(2);
                int ep = getEndPoint(filter, info0.vid, info0.pid);
                if (ep == -1)
                    continue;
                info0.ep = ep;
                if (type == 3 && (info0.vid != info.vid || info0.pid != info.pid || info0.sn != info.sn))
                    continue;
            }
        }
        // 得到设备类型
        /*
        SetupDiGetDeviceRegistryProperty(hDevInfoSet, &spDevInfoData,
                                         SPDRP_DEVTYPE, NULL,
                                         (PBYTE)buf, sizeof(buf), NULL);
        FILE_DEVICE_PRINTER;
        DWORD type = *((DWORD*)buf);
        */
        // 得到设备描述
        buf[0] = 0;
        SetupDiGetDeviceRegistryProperty(hDevInfoSet, &spDevInfoData,
                                         SPDRP_DEVICEDESC, NULL,
                                         (PBYTE)buf, sizeof(buf), NULL);
        info0.desc = QString::fromWCharArray(buf);
        if (type == 3 && (info0.desc != info.desc))
            continue;

        if (type == 0) {
            info0.handle = USBDevice_INVALID_HANDLE_VALUE;
            list.append(info0);
            continue;
        }

        // 枚舉符合該GUID的設備接口
        ifData.cbSize = sizeof(ifData);
        bResult = ::SetupDiEnumDeviceInterfaces(
                    hDevInfoSet,     // 設備信息集句柄
                    NULL,            // 不需額外的設備描述
                    lpGuid,          // GUID
                    (ULONG)nTotle,   // 設備信息集里的設備序號
                    &ifData);        // 設備接口信息

        // 取得该设备接口的细节(设备路径)
        bResult = SetupDiGetInterfaceDeviceDetail(
                    hDevInfoSet,    // 设备信息集句柄
                    &ifData,        // 设备接口信息
                    pDetail,        // 设备接口细节(设备路径)
                    INTERFACE_DETAIL_SIZE,    // 输出缓冲区大小
                    NULL,           // 不需计算输出缓冲区大小(直接用设定值)
                    NULL);          // 不需额外的设备描述

        info0.handle = CreateFile(pDetail->DevicePath, GENERIC_READ | GENERIC_WRITE,
                                  0, NULL,
                                  OPEN_EXISTING,
                                  FILE_ATTRIBUTE_NORMAL, NULL);
        if (info0.handle == USBDevice_INVALID_HANDLE_VALUE)
            continue;

        list.append(info0);
        if (type == 1)
            continue;
        break;
    }

    // 释放设备接口数据空间
    ::GlobalFree(pDetail);
    // 关闭设备信息集句柄
    ::SetupDiDestroyDeviceInfoList(hDevInfoSet);

    return list;
}
#endif

#ifdef Q_OS_UNIX
#include <QDebug>

USBDevice::USBDevice(const USBDevice_Info &info, int timeoutms)
{
    ep = info.ep;
    handle = info.handle;

    if (handle == USBDevice_INVALID_HANDLE_VALUE)
        return;

    setTimeoutms(timeoutms);

    ifn = 0;
    if ((info.vid == 0x20D0 && info.pid == 0x7008) ||
            (info.vid == 0x20D0 && info.pid == 0x7009))
        ifn = 1;

    if (libusb_kernel_driver_active(handle, ifn) == 1)
        libusb_detach_kernel_driver(handle, ifn);

    if (libusb_claim_interface(handle, ifn))
        qDebug("libusb_claim_interface err");
}

USBDevice::~USBDevice()
{
    if (handle == USBDevice_INVALID_HANDLE_VALUE)
        return;

    if (libusb_release_interface(handle, ifn))
        qDebug("libusb_release_interface err");

    libusb_close(handle);
}

void USBDevice::setTimeoutms(int timeoutms)
{
    if (handle == USBDevice_INVALID_HANDLE_VALUE)
        return;

    toms = timeoutms;
}

bool USBDevice::write(const char *data, int size, int *len)
{
    if (handle == USBDevice_INVALID_HANDLE_VALUE)
        return false;

    int l;
    bool b = libusb_bulk_transfer(handle, ep, (uchar*)data, size, &l, toms);
    if (len) *len = l;
    return !b;
}

bool USBDevice::read(char *data, int size, int *len)
{
    if (handle == USBDevice_INVALID_HANDLE_VALUE)
        return false;

    int l;
    bool b = libusb_bulk_transfer(handle, ep >> 8, (uchar*)data, size, &l, toms);
    if (len) *len = l;
    return !b;
}

USBDevice_Scanner::USBDevice_Scanner()
{
    libusb_init(&ctx);
}

USBDevice_Scanner::~USBDevice_Scanner()
{
    libusb_exit(ctx);
}

QList<USBDevice_Info> USBDevice_Scanner::getList(const QList<int> &filter, int type, const USBDevice_Info &info)
{
    QList<USBDevice_Info> list;

    libusb_device **devicelist;
    int cn = libusb_get_device_list(ctx, &devicelist);
    uchar buf[1024];

    for (int i=0; i<cn; ++i) {
        libusb_device *dev = devicelist[i];
        libusb_device_descriptor desc;
        int err = libusb_get_device_descriptor(dev, &desc);
        if (err)
            continue;

        USBDevice_Info info0;
        info0.vid = desc.idVendor;
        info0.pid = desc.idProduct;
        int ep = getEndPoint(filter, info0.vid, info0.pid);
        if (ep == -1)
            continue;
        info0.ep = ep;
        if (type == 3 && (info0.vid != info.vid || info0.pid != info.pid))
            continue;

        libusb_device_handle *handle;
        if (libusb_open(dev, &handle))
            continue;
        buf[0] = 0;
        libusb_get_string_descriptor_ascii(handle, desc.iProduct, buf, sizeof(buf));
        info0.desc = QString::fromLocal8Bit((char*)buf);
        buf[0] = 0;
        libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, buf, sizeof(buf));
        info0.sn = QString::fromLocal8Bit((char*)buf);
        if (type == 3 && (info0.desc != info.desc || info0.sn != info.sn)) {
            libusb_close(handle);
            continue;
        }

        if (type == 0) {
            libusb_close(handle);
            info0.handle = USBDevice_INVALID_HANDLE_VALUE;
            list.append(info0);
            continue;
        }

        info0.handle = handle;
        list.append(info0);
        if (type == 1)
            continue;
        break;
    }

    libusb_free_device_list(devicelist, 1);

    return list;
}
#endif

int USBDevice_Scanner::getEndPoint(const QList<int> &filter, quint16 vid, quint16 pid)
{
    if (filter.isEmpty())
        return 0;
    for (int i = 0; i < filter.size(); i+=3) {
        if (vid == filter.at(i+0) && pid == filter.at(i+1))
            return filter.at(i+2);
    }
    return -1;
}
