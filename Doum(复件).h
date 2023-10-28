#ifndef DOUM_H
#define DOUM_H

#include <QVector>
#include <QPixmap>
#include <QBitmap>
#include <QLabel>

class Doum
{
public:
    Doum();

    void init(int g, int d);

    int dn;
    int gn;
    int dng;

    QPixmap bgimg;
    QVector<QVector<QPixmap> > dimg;
    QVector<QVector<QPixmap> > gimg;
    QVector<QBitmap> dmask;
    QVector<QBitmap> gmask;
    QVector<int> dpos;
    QVector<int> gpos;
    QVector<int> dwpos;
    QVector<int> gwpos;

private:
    void initDouImg();
    void initDouPos();
    QVector<QPixmap> createDouImg(const QString &path);
    QVector<int> readPos(const QString &path, int rows, int cols);

public:
    static QImage adjustHSL(const QImage &img, int hi, int si, int li);
    static void adjustSaturation(qreal &rf, qreal &gf, qreal &bf, qreal increment);
    static QVector<QLabel *> genDouWidget(
            const QVector<QVector<QPixmap> > &img,
            const QVector<int> &pos,
            QWidget *p, int px, int py);
    static QVector<QLabel *> genDWeightWidget(
            const QVector<int> &pos,
            QWidget *p, int px, int py);
    static void setDouMask(
            const QVector<QLabel *> &lab,
            const QVector<QBitmap> &mask);
};

#endif // DOUM_H
