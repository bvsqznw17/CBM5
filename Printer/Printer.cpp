#include "Printer.h"
#include "Config.h"
#include "Tool.h"
#include <QThread>
#include <QElapsedTimer>
#include <QDebug>

Printer g_printer;
Printer g_printer_sf(QList<int>()
                     << 0x20D0 << 0x7008 << 0x8101
                     << 0x20D0 << 0x7009 << 0x8101
                     );
Printer g_printer_lab(QList<int>()
                      << 0x20D0 << 0x7008 << 0x8101
                      << 0x20D0 << 0x7009 << 0x8101
                      );
Printer g_printer_bill(QList<int>()
                       << 0x20D0 << 0x7008 << 0x8101
                       << 0x20D0 << 0x7009 << 0x8101
                       << 0x20D1 << 0x7007 << 0x8202
                       << 0x0483 << 0x5720 << 0x8102
                       );

Printer::Printer(QObject *parent) :
    QObject(parent)
{
    filter << 0x20D0 << 0x7008 << 0x8101; // 雪姐标签打印机
    filter << 0x20D0 << 0x7009 << 0x8101; // 雪姐标签打印机2
    filter << 0x20D1 << 0x7007 << 0x8202; // 初版小票打印机
    filter << 0x0483 << 0x5720 << 0x8102; // HWISPRT小票打印机
}

Printer::Printer(const QList<int> f, QObject *parent) :
    QObject(parent),
    filter(f)
{
}

uint Printer::getPixpmm(quint16 vid, quint16 pid)
{
    if (vid == 0x20D0 && pid == 0x7009)
        return 12;
    else
        return 8;
}

bool Printer::isInFilter(quint16 vid, quint16 pid, const QList<int> &filter)
{
    for (int i = 0; i < filter.size(); i+=3) {
        if (filter.at(i+0) == vid && filter.at(i+1) == pid)
            return true;
    }
    return false;
}

QList<USBDevice_Info> Printer::getList()
{
    USBDevice_Scanner scanner;
    return scanner.getList(filter);
}

QList<int> Printer::readState()
{
    USBDevice_Scanner scanner;
    QList<USBDevice_Info> list = scanner.getList(filter, 1);
    QList<int> state;
    state.reserve(list.size());
    for (int i = 0; i < list.size(); i++) {
        USBDevice usb(list.at(i));
        if (!(g_printer_sf.isInFilter(list.at(i))))
            state.append(-1);
        else
            state.append(readState_usb(usb));
    }
    return state;
}

QList<int> Printer::readState2()
{
    USBDevice_Scanner scanner;
    QList<USBDevice_Info> list = scanner.getList(filter, 1);
    QList<int> state;
    state.reserve(list.size());
    for (int i = 0; i < list.size(); i++) {
        USBDevice usb(list.at(i));
        if (!(g_printer_sf.isInFilter(list.at(i))))
            state.append(-1);
        else
            state.append(readState2_usb(usb));
    }
    return state;
}

int Printer::readState(const USBDevice_Info &info)
{
    if (!(g_printer_sf.isInFilter(info)))
        return -1;

    USBDevice_Scanner scanner;
    QList<USBDevice_Info> list = scanner.getList(filter, 3, info);
    if (list.isEmpty())
        return -1;

    USBDevice usb(list.at(0));
    return readState_usb(usb);
}

int Printer::readState2(const USBDevice_Info &info)
{
    if (!(g_printer_sf.isInFilter(info)))
        return -1;

    USBDevice_Scanner scanner;
    QList<USBDevice_Info> list = scanner.getList(filter, 3, info);
    if (list.isEmpty())
        return -1;

    USBDevice usb(list.at(0));
    return readState2_usb(usb);
}

void Printer::feed(int n)
{
    USBDevice_Scanner scanner;
    QList<USBDevice_Info> list = scanner.getList(filter, 1);
    for (int i = 0; i < list.size(); i++) {
        USBDevice usb(list.at(i));
        feed_usb(usb, n);
    }
}

void Printer::feed(const USBDevice_Info &info, int n)
{
    USBDevice_Scanner scanner;
    QList<USBDevice_Info> list = scanner.getList(filter, 3, info);
    for (int i = 0; i < list.size(); i++) {
        USBDevice usb(list.at(i));
        feed_usb(usb, n);
    }
}

void Printer::print(const QImage &image)
{
    USBDevice_Scanner scanner;
    QList<USBDevice_Info> list = scanner.getList(filter, 1);
    for (int i = 0; i < list.size(); i++) {
        USBDevice usb(list.at(i));
        print_usb(usb, list.at(i), image);
    }
}

void Printer::print(const USBDevice_Info &info, const QImage &image)
{
    USBDevice_Scanner scanner;
    QList<USBDevice_Info> list = scanner.getList(filter, 3, info);
    for (int i = 0; i < list.size(); i++) {
        USBDevice usb(list.at(i));
        print_usb(usb, list.at(i), image);
    }
}

void Printer::test()
{
}

int Printer::readState_usb(USBDevice &usb)
{
#ifdef MYOPT_TEST
    return 0;
#endif
    int len;
    char data[1];

    char _1b76[2] = {0x1b,0x76};
    usb.write(_1b76, sizeof(_1b76), &len);
    usb.read(data, sizeof(data), &len);
    return len == 1 ? ((uint)(uchar)data[0]) : -1;
}

int Printer::readState2_usb(USBDevice &usb)
{
#ifdef MYOPT_TEST
    return 0;
#endif
    int len;
    char data[1];

    char _1004[3] = {0x10,0x04,1};
    usb.write(_1004, sizeof(_1004), &len);
    usb.read(data, 1, &len);
    return len == 1 ? ((uint)(uchar)data[0]) : -1;
}

bool Printer::feed_usb(USBDevice &usb, int n)
{
#ifdef MYOPT_TEST
    return true;
#endif
    char _1b64[3] = {0x1b, 0x64, (char)n};
    return usb.write(_1b64, sizeof(_1b64), 0);
}

bool Printer::print_usb(USBDevice &usb, const USBDevice_Info &info, const QImage &image)
{
    //    // hwisprt sdk的使用
    //    if (info.vid == 0x0483 && info.pid == 0x5720) {
    //#ifdef __ARM_EABI__
    //        QImage img = image.convertToFormat(QImage::Format_Mono);
    //        img.invertPixels();
    //        for (int i = 0; i < img.height(); i+=64) {
    //            QImage img0 = img.copy(0, i, img.width(), 64);
    //            img0.save("hwisprt_print_bmp.bmp");
    //            int ret = OpenPort(USB_PORT, 0);
    //            if (ret < 0) {
    //                qDebug("OpenPort() fail ret=%d", ret);
    //                return;
    //            }
    //            POS_Output_PrintBmpDirect((uchar*)"hwisprt_print_bmp.bmp");
    //            ClosePort();
    //            QThread::msleep(100);
    //        }
    //#endif
    //        return;
    //    }

    int len;

    bool b_printer_sf = g_printer_sf.isInFilter(info);
    int b_printer_lab = false;
    if (b_printer_sf) {
        // 打印灰度
        char _1b3d[3] = {0x1b,0x3d,1};
        _1b3d[2] |= g_conf.Printer_HuiDu << 1;
        if (!waitModifySucceed(usb, _1b3d, sizeof(_1b3d)))
            return false;
        // 标签长度
        char _1d25[4] = {0x1d,0x25,0,0};
        _1d25[2] = g_conf.Printer_LabLength;
        _1d25[3] = g_conf.Printer_LabLength >> 8;
        if (!waitModifySucceed(usb, _1d25, sizeof(_1d25)))
            return false;
        // 标签间隙
        char _1d26[3] = {0x1d,0x26,0};
        _1d26[2] = g_conf.Printer_LabInterval;
        if (!waitModifySucceed(usb, _1d26, sizeof(_1d26)))
            return false;
        // 纸张类型
        char _1b63[4] = {0x1b,0x63,0x30,0};
        _1b63[3] = b_printer_lab;
        if (!waitModifySucceed(usb, _1b63, sizeof(_1b63)))
            return false;
        // 是否启动回退,打印初始位置
        char _1004[3] = {0x10,0x04,0};
        _1004[2] |= ((int)g_conf.Printer_IsHuiTui) << 3;
        _1004[2] |= (g_conf.Printer_StartPos << 4);
        if (!waitModifySucceed(usb, _1004, sizeof(_1004)))
            return false;
    }

    if (image.isNull())
        return true;

    QImage img = image.convertToFormat(QImage::Format_Mono);
    fillMonoImageLastCol(img);
    QList<QImage> imglist;

    if (info.vid == 0x0483 && info.pid == 0x5720) {
        imglist = splitImage(img);
        if (!imglist.isEmpty()) {
            img = imglist.takeFirst();
            if (b_printer_lab)
                imglist.clear();
        }
        do {
#ifdef MYOPT_TEST
            img.save(QString("print_%1.bmp").arg(imglist.size()));
#endif
            int w = (img.width()+7)/8;
            int h = img.height();

            const int maxh = 64;
            if (w > 255) w = 255;
            char data[maxh*(w+4) + 6];
            int size = 0;

            for (int i = 0; i < h; i+=maxh) {
                char *d = data;
                *d++ = 0x1d;
                *d++ = 0x44;
                *d++ = 0x01;
                for (int j = 0; j < maxh && j+i < h; j++) {
                    *d++ = 0x15;
                    *d++ = 0x01;
                    *d++ = 0x16;
                    *d++ = w;
                    memcpy(d, img.constScanLine(j+i), w);
                    d += w;
                }
                *d++ = 0x1d;
                *d++ = 0x44;
                *d++ = 0x00;

                size = d-data;
                int ssize = size;
                do {
                    len = 0;
                    usb.write(data + ssize - size, size, &len);
                    size -= len;
                } while (size > 0 && len > 0);
#ifndef MYOPT_TEST
                //qDebug("%d: %d/%d",i/maxh,ssize-size,ssize);
                if (size != 0) {
                    qDebug("Printer::print: err");
                    return false;
                }
#endif
            }
            if (imglist.isEmpty())
                break;
            img = imglist.takeFirst();
        } while (1);
    }
    else {
        if (b_printer_sf) {
            int imgw, imgh, maxh;
            imgh = img.height();
            if (info.vid == 0x20D0 && info.pid == 0x7009) {
                imgw = 80 * 12;
                maxh = 90 * 12;
            }
            else {
                imgw = 52 * 8;
                maxh = 90 * 8;
            }

            const uchar *imgd = img.constBits();
            int imgbl = img.bytesPerLine();

            if (b_printer_lab) {
                // 将图片调整到imgw宽度，最大maxh高度，并且镜像翻转
                QImage img2 = QImage(imgw, qMin(imgh, maxh), QImage::Format_Mono);
                img2.fill(0);
                uchar *img2d = img2.bits();
                int img2bl = img2.bytesPerLine();
                int minw = qMin((img.width()+7)/8, (img2.width()+7)/8);
                int minh = qMin(imgh, img2.height());
                for (int i = 0; i < minh; i++)
                    memcpy(img2d+i*img2bl, imgd+i*imgbl, minw);
                img = img2.mirrored(true, true);
            }
            else {
                // 将图片调整到imgw宽度，按maxh高度分割，并且原图要靠右
                for (int hi = 0; hi < img.height(); hi++) {
                    QImage img2 = QImage(imgw, qMin(imgh-hi, maxh), QImage::Format_Mono);
                    img2.fill(0);
                    uchar *img2d = img2.bits();
                    int img2bl = img2.bytesPerLine();
                    int minw = qMin(img.width(), img2.width());
                    int minh = img2.height();
                    int img2w = img2.width();
                    for (int i = 0; i < minh; i++)
                        bitcpy(img2d+i*img2bl, img2w-minw, imgd+(i+hi)*imgbl, 0, minw);
                    imglist.append(img2);
                    hi += minh;
                }
                if (!imglist.isEmpty())
                    img = imglist.takeFirst();
            }
        }
        else {
            imglist = splitImage(img);
            if (!imglist.isEmpty()) {
                img = imglist.takeFirst();
                if (b_printer_lab)
                    imglist.clear();
            }
        }
        do {
#ifdef MYOPT_TEST
            img.save(QString("print_%1.bmp").arg(imglist.size()));
#endif
            if (b_printer_sf) {
                // 等待打印机忙碌状态消失
                QElapsedTimer etimer;
                etimer.start();
                do {
                    int state = readState_usb(usb);
                    if (state == -1)
                        return false;
                    if (!(state & (1 << 4)))
                        break;
                    if (etimer.elapsed() >= 5000)
                        return false;
                    QThread::msleep(50);
                } while (1);
            }

            int w = (img.width()+7)/8;
            int h = img.height();
            char _1d76[8] = {0x1d,0x76,0x30,0};
            _1d76[4] = w; _1d76[5] = w >> 8;
            _1d76[6] = h; _1d76[7] = h >> 8;
            char data[8+w*h];
            memcpy(data, _1d76, 8);
            for (int i = 0; i < h; i++)
                memcpy(data+8+w*i, img.constScanLine(i), w);

            int size = 8+w*h;
            int ssize = size;
            do {
                len = 0;
                usb.write(data + ssize - size, size, &len);
                size -= len;
            } while (size > 0 && len > 0);
#ifndef MYOPT_TEST
            //qDebug("%d: %d/%d",1,ssize-size,ssize);
            if (size != 0) {
                qDebug("Printer::print: err");
                return false;
            }
#endif
            if (imglist.isEmpty())
                break;
            img = imglist.takeFirst();
        } while (1);
    }
    return true;
}

bool Printer::isModifySucceed(USBDevice &usb)
{
#ifdef MYOPT_TEST
    return true;
#endif
    int len;
    char _1004[3] = {0x10,0x04,1};
    usb.write(_1004, sizeof(_1004), &len);
    char data[1];
    usb.read(data, 1, &len);
    return (len == 1) && ((data[0] & 0x01) == 0);
}

bool Printer::waitModifySucceed(USBDevice &usb, const char *data, int size)
{
#ifdef MYOPT_TEST
    return true;
#endif
    int i = 10;
    do {
        if (!usb.write(data, size, 0))
            return false;
        if (isModifySucceed(usb))
            return true;
        if (--i <= 0)
            return false;
        QThread::msleep(500);
    } while(1);
}

void Printer::fillMonoImageLastCol(const QImage &img)
{
    int width = img.width();
    if (width % 8 == 0)
        return;

    int m = 8 - (width % 8);
    int w = width / 8;
    int h = img.height();
    for (int i = 0; i < h; i++) {
        uchar *d = const_cast<uchar *>(img.constScanLine(i));
        d[w] &= 0xFF << m;
    }
}

void Printer::bitcpy(uchar *dest, int dest_pos, const uchar *src, int src_pos, int n)
{
    for (int i = 0; i < n;) {
        int dest_i = (dest_pos + i) / 8;
        int dest_m = (dest_pos + i) % 8;
        int src_i = (src_pos + i) / 8;
        int src_m = (src_pos + i) % 8;
        uchar c = src[src_i] << src_m;
        c &= 0xFF << dest_m;
        c >>= dest_m;
        dest[dest_i] |= c;
        i += qMin(8 - dest_m, 8 - src_m);
    }
}

QList<QImage> Printer::splitImage(const QImage &image, int maxw, int maxh)
{
    QList<QImage> list;
    QImage img = image;
    if (img.width() > maxw)
        img = img.copy(0, 0, maxw, img.height());
    if (img.height() <= maxh) {
        list.append(img);
        return list;
    }
    int w = img.width();
    int h = img.height();
    for (int i = 0; i < img.height(); i++) {
        QImage img0 = img.copy(0, i, w, qMin(h-i, maxh));
        list.append(img0);
        i+=maxh;
    }
    return list;
}
