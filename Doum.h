#ifndef DOUM_H
#define DOUM_H

#include <QVector>
#include <QPixmap>
#include <QBitmap>
#include <QLabel>
#include <QVariant>

class Doum
{
public:
    Doum();

    void init(int g, int d);

    // 总斗数，总组数，每组斗数
    int dn, gn, dng;

    QPixmap bgimg;
    QVector<QVector<QPixmap> > dimg;
    QVector<QVector<QPixmap> > gimg;
    QVector<QVector<QPixmap> > oimg;
    QVector<QBitmap> dmask;
    QVector<QBitmap> gmask;
    QVector<QBitmap> omask;
    QVector<int> dpos;
    QVector<int> gpos;
    QVector<int> opos;
    QVector<int> dwpos;
    QVector<int> gwpos;
    QVector<int> owpos;
    QVector<int> dstate;

private:
    void initDouImg();
    void initDouPos();

public:
    static QVector<int> readPos(const QString &path, int rows, int cols);
    static QVector<int> readState(int dn);
    static void saveState(int dn, const QVector<int> &dstate);
    static void removeState(int dn);
    static QVector<QPixmap> createDouImg(const QImage &img, const QVector<int> &dstate);
    inline static QVector<QPixmap> createDouImg(const QString &path, const QVector<int> &dstate)
    { return createDouImg(QImage(path), dstate); }
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

    inline QVector<QLabel *> genDouWidget(QWidget *p, int px, int py)
    { return genDouWidget(dimg, dpos, p, px, py); }
    inline QVector<QLabel *> genGDouWidget(QWidget *p, int px, int py)
    { return genDouWidget(gimg, gpos, p, px, py); }
    inline QVector<QLabel *> genODouWidget(QWidget *p, int px, int py)
    { if (oimg.isEmpty()) return genGDouWidget(p, px, py);
      else return genDouWidget(oimg, opos, p, px, py); }

    inline QVector<QLabel *> genDWeightWidget(QWidget *p, int px, int py)
    { return genDWeightWidget(dwpos, p, px, py); }
    inline QVector<QLabel *> genGWeightWidget(QWidget *p, int px, int py)
    { return genDWeightWidget(gwpos, p, px, py); }
    inline QVector<QLabel *> genOWeightWidget(QWidget *p, int px, int py)
    { if (oimg.isEmpty()) return genGWeightWidget(p, px, py);
      else return genDWeightWidget(owpos, p, px, py); }

    inline void setDouMask(const QVector<QLabel *> &lab)
    { setDouMask(lab, dmask); }
    inline void setGDouMask(const QVector<QLabel *> &lab)
    { setDouMask(lab, gmask); }
    inline void setODouMask(const QVector<QLabel *> &lab)
    { if (oimg.isEmpty()) setGDouMask(lab);
      else setDouMask(lab, omask); }

    void setDouNumber(const QVector<QLabel *> &lab, QObject *filter);
    void setGDouNumber(const QVector<QLabel *> &lab, QObject *filter);
    void setODouNumber(const QVector<QLabel *> &lab, QObject *filter);
    inline static int getDouNumber(QLabel *lab)
    { return lab->property("dou_number").toInt(); }
    void setGDouImage(const QVector<QLabel *> &dLab, const QVector<QLabel *> &gLab, int n, int m);
    void setODouImage(const QVector<QLabel *> &dLab, const QVector<QLabel *> &oLab, int n, int m);

    int getDouWeight(class MParam **mp, int n);
    int getDouStatus(class MParam **mp, int i, int jyd = 0);
    int getDouStatus(const QByteArray &ba, int i);
};

#endif // DOUM_H
