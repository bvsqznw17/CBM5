#ifndef CRC_H
#define CRC_H

#include <QtGlobal>

ushort crc16_makecrc(const char *data, int len);
void crc16_make(QByteArray &data);
void crc16_make(char *data, int len);
bool crc16_check(const QByteArray &data);
bool crc16_check(const char *data, int len);

#endif // CRC_H
