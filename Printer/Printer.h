#ifndef PRINTER_H
#define PRINTER_H

#include <QObject>
#include <QImage>
#include "USBDevice.h"

extern class Printer g_printer;
extern class Printer g_printer_sf;
extern class Printer g_printer_lab;
extern class Printer g_printer_bill;

class Printer : public QObject
{
    Q_OBJECT
public:
    explicit Printer(QObject *parent = 0);
    explicit Printer(const QList<int> f, QObject *parent = 0);

    static uint getPixpmm(quint16 vid, quint16 pid);
    inline static uint getPixpmm(const USBDevice_Info &info)
    { return getPixpmm(info.vid, info.pid); }

    static bool isInFilter(quint16 vid, quint16 pid, const QList<int> &filter);
    inline static bool isInFilter(const USBDevice_Info &info, const QList<int> &filter)
    { return isInFilter(info.vid, info.pid, filter); }

    inline QList<int> getFilter() { return filter; }
    inline bool isInFilter(quint16 vid, quint16 pid)
    { return isInFilter(vid, pid, filter); }
    inline bool isInFilter(const USBDevice_Info &info)
    { return isInFilter(info.vid, info.pid, filter); }

    QList<USBDevice_Info> getList();
    QList<int> readState();
    QList<int> readState2();
    int readState(const USBDevice_Info &info);
    int readState2(const USBDevice_Info &info);
    void feed(int n = 1);
    void feed(const USBDevice_Info &info, int n = 1);
    void print(const QImage &image);
    void print(const USBDevice_Info &info, const QImage &image);
    void test();

public:
    static int readState_usb(USBDevice &usb);
    static int readState2_usb(USBDevice &usb);
    static bool feed_usb(USBDevice &usb, int n = 1);
    static bool print_usb(USBDevice &usb, const USBDevice_Info &info, const QImage &image);
    static bool isModifySucceed(USBDevice &usb);
    static bool waitModifySucceed(USBDevice &usb, const char *data, int size);
    static void fillMonoImageLastCol(const QImage &img);
    static void bitcpy(uchar *dest, int dest_pos, const uchar *src, int src_pos, int n);
    static QList<QImage> splitImage(const QImage &image, int maxw = 32767, int maxh = 32767);

private:
    QList<int> filter;
};

#endif // PRINTER_H
