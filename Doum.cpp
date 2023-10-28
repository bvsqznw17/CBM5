#include "Doum.h"
#include "QLabel2.h"
#include <QFile>
#include <QTextStream>

Doum::Doum()
{
    dn = gn = dng = 0;
}

void Doum::init(int g, int d)
{
    switch (g) {
    case 1:
        switch (d) {
        case 10: break;
        case 12: break;
        case 14: break;
        case 16: break;
        case 20: break;
        default: return;
        }
        break;
    default:
        return;
    }
    gn = g;
    dng = d;
    dn = g * d;
    bgimg.load(QString("./images/%1d/%2g_bg.png").arg(dn).arg(gn));
    dstate = readState(dn);
    initDouImg();
    initDouPos();
}

void Doum::initDouImg()
{
    {
        QString dpathi = QString("./images/%1d/d%2.png").arg(dn);
        dimg = QVector<QVector<QPixmap> >(dn);
        dmask = QVector<QBitmap>(dn);
        QVector<QPixmap> *dimgd = dimg.data();
        QBitmap *dmaskd = dmask.data();

        for (int i = 0; i < dn; i++) {
            dimgd[i] = createDouImg(dpathi.arg(i+1), dstate);
            dmaskd[i] = dimgd[i].at(0).mask();
        }
    }
    {
        QString gpathi = QString("./images/%1d/%2g_%3.png").arg(dn).arg(gn);
        gimg = QVector<QVector<QPixmap> >(gn);
        gmask = QVector<QBitmap>(gn);
        QVector<QPixmap> *gimgd = gimg.data();
        QBitmap *gmaskd = gmask.data();

        for (int i = 0; i < gn; i++) {
            gimgd[i] = createDouImg(gpathi.arg(QChar(i+'A')), dstate);
            gmaskd[i] = gimgd[i].at(0).mask();
        }
    }
}

void Doum::initDouPos()
{
    dpos = readPos(QString("./images/%1d/dpos").arg(dn), dn, 2);
    gpos = readPos(QString("./images/%1d/%2g_pos").arg(dn).arg(gn), gn, 2);
    dwpos = readPos(QString("./images/%1d/dwpos").arg(dn), dn, 6);
    gwpos = readPos(QString("./images/%1d/%2g_wpos").arg(dn).arg(gn), gn, 6);
}

QVector<int> Doum::readPos(const QString &path, int rows, int cols)
{
    QVector<int> v(rows * cols, 0);
    QFile file(path);
    if(!file.open(QFile::ReadOnly))
        return v;

    int *data = v.data();
    QTextStream in(&file);
    for (int i = 0; i < rows && !in.atEnd(); i++) {
        QString line = in.readLine();
        QStringList split = line.split(',');
        int j = 0;
        for (; j < cols && j < split.size(); j++)
            data[i*cols+j] = split.at(j).toInt();
    }
    return v;
}

QVector<int> Doum::readState(int dn)
{
    int rows = 4;
    int cols = 16;
    QVector<int> v(rows * cols, 0);
    int *data = v.data();
    int nAngle[16] = {0,8,18,28,38,48,58,68,78,88,98,116,136,160,172,180};
    for (int i = 0; i < 16; i++) {
        int h = nAngle[i] + nAngle[i];
        if (h > 180)
            h -= 360;
        data[i] = h;
    }
    data[cols + cols - 1] = -100;

    QString path;
    QString path1 = QString("./images/%1d/dstate").arg(dn);
    QString path2 = path1 + "_def";
    if (QFile::exists(path1))
        path = path1;
    else if (QFile::exists(path2))
        path = path2;
    else
        return v;

    QFile file(path);
    if(!file.open(QFile::ReadOnly))
        return v;

    QTextStream in(&file);
    for (int i = 0; i < rows && !in.atEnd(); i++) {
        QString line = in.readLine();
        QStringList split = line.split(',');
        int j = 0;
        for (; j < cols && j < split.size(); j++) {
            QString str = split.at(j).trimmed();
            if (str.isEmpty())
                continue;
            data[i*cols+j] = str.toInt();
        }
    }
    return v;
}

void Doum::saveState(int dn, const QVector<int> &dstate)
{
    QString path = QString("./images/%1d/dstate").arg(dn);

    QFile file(path);
    if(!file.open(QFile::WriteOnly | QFile::Truncate))
        return;

    QTextStream out(&file);
    for (int i = 0; i < 16; i++)
        out << dstate.at(i) << ",";
    out << "\n";
    for (int i = 0; i < 16; i++)
        out << dstate.at(i + 16) << ",";
    out << "\n";
    for (int i = 0; i < 16; i++)
        out << dstate.at(i + 32) << ",";
    out << "\n";
    for (int i = 0; i < 16; i++)
        out << dstate.at(i + 48) << ",";
#ifdef __ARM_EABI__
    system("sync");
#endif
}

void Doum::removeState(int dn)
{
    QString path = QString("./images/%1d/dstate").arg(dn);
    QFile::remove(path);
#ifdef __ARM_EABI__
    system("sync");
#endif
}

QVector<QPixmap> Doum::createDouImg(const QImage &img, const QVector<int> &dstate)
{
    QVector<QPixmap> v(16);
    QPixmap *pm = v.data();

    for (int i = 0; i < 16; i++) {
        int h = dstate.at(i);
        int s = dstate.at(i + 16);
        int l = dstate.at(i + 32);
        pm[i] = QPixmap::fromImage(adjustHSL(img, h, s, l));
    }

    return v;
}

QImage Doum::adjustHSL(const QImage &img, int hi, int si, int li)
{
    QImage ret(img.size(), QImage::Format_ARGB32);

    qreal hif = ((hi + 360) % 360) / qreal(360);
    qreal sif = si / qreal(100);
    qreal lif = li / qreal(100);

    for (int y = 0; y < img.height(); y++) {
        for (int x = 0; x < img.width(); x++) {
            qreal hf, sf, lf, af, rf, gf, bf;
            QColor c = img.pixelColor(x, y);

            c = c.toHsl();
            c.getHslF(&hf, &sf, &lf, &af);

            hf += hif;
            if (hf > 1) hf -= 1;
            if (hf < 0) hf = 0;
            else if (hf > 1) hf = 1;

            c.setHslF(hf, sf, lf, af);
            c = c.toRgb();
            c.getRgbF(&rf, &gf, &bf, &af);

            adjustSaturation(rf, gf, bf, sif);

            if (li <= 0) {
                rf += rf * lif;
                gf += gf * lif;
                bf += bf * lif;
            }
            else {
                rf += (1 - rf) * lif;
                gf += (1 - gf) * lif;
                bf += (1 - bf) * lif;
            }

            // 颜色混合(c2叠加到c1): a = a1+a2 - a1*a2; (c1*a1*(1-a2) + c2*a2) / a;
            // 提前将非完全透明的像素融入白色背景
            uchar r = qRound(((1-af)+rf*af) * 255);
            uchar g = qRound(((1-af)+gf*af) * 255);
            uchar b = qRound(((1-af)+bf*af) * 255);
            c.setRgb(r, g, b, c.alpha());

            ret.setPixelColor(x, y, c);
        }
    }

    return ret;
}

void Doum::adjustSaturation(qreal &rf, qreal &gf, qreal &bf, qreal increment)
{
    qreal max = qMax(qMax(rf, gf), bf);
    qreal min = qMin(qMin(rf, gf), bf);
    qreal delta = max - min;
    if (delta == 0)
        return;

    qreal value = max + min;
    qreal L = value / 2;
    qreal S, alpha;
    if(L < 0.5)
         S = delta / value;
    else
         S = delta / (2 - value);

    if (increment >= 0) {
        if((increment + S) >= 1)
            alpha = S;
        else
            alpha = 1 - increment;
        alpha = 1 / alpha - 1;
        rf = rf + (rf - L) * alpha;
        gf = gf + (gf - L) * alpha;
        bf = bf + (bf - L) * alpha;
    }
    else {
        alpha = increment;
        rf = L + (rf- L) * (1 + alpha);
        gf = L + (gf- L) * (1 + alpha);
        bf = L + (bf- L) * (1 + alpha);
    }
}

QVector<QLabel *> Doum::genDouWidget(
        const QVector<QVector<QPixmap> > &img,
        const QVector<int> &pos,
        QWidget *p, int px, int py)
{
    int size = img.size();
    QVector<QLabel *> v(size);
    QLabel **lab = v.data();

    for (int i = 0; i < size; i++) {
        int k = i*2;
        lab[i] = new QLabel(p);
        lab[i]->move(pos[k+0]+px, pos[k+1]+py);
        lab[i]->resize(img.at(i).at(0).size());
        //lab[i]->setPixmap(img.at(i).at(i%16));
    }

    return v;
}

QVector<QLabel *> Doum::genDWeightWidget(
        const QVector<int> &pos,
        QWidget *p, int px, int py)
{
    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::black);
    int size = pos.size() / 6;
    QVector<QLabel *> v(size);
    QLabel **lab = v.data();

    for (int i = 0; i < size; i++) {
        lab[i] = new QLabel2(p);
        int k = i*6;
        lab[i]->move(pos[k+0]+px, pos[k+1]+py);
        lab[i]->resize(pos[k+2], pos[k+3]);
        if (pos[k+4] > 0)
            lab[i]->setStyleSheet(QString("QLabel { font-size: %1px; } "
                                          "QLabel:!enabled { color: red; }")
                                  .arg(pos[k+4]));
        else
            lab[i]->setStyleSheet(QString("QLabel { font-size: 14px; } "
                                          "QLabel:!enabled { color: red; }"));
        lab[i]->setAlignment(Qt::AlignVCenter | (Qt::Alignment)pos[k+5]);
        lab[i]->setPalette(palette);
        //lab[i]->setText(QString("2134.5:%1").arg(i%16,0,16).toUpper());
    }

    return v;
}

void Doum::setDouMask(
        const QVector<QLabel *> &lab,
        const QVector<QBitmap> &mask)
{
    for (int i = 0; i < lab.size(); i++)
        lab[i]->setMask(mask[i]);
}

void Doum::setDouNumber(const QVector<QLabel *> &lab, QObject *filter)
{
    for (int i = 0; i < lab.size(); i++) {
        lab.at(i)->setProperty("dou_number", i+1);
        lab.at(i)->installEventFilter(filter);
    }
}

void Doum::setGDouNumber(const QVector<QLabel *> &lab, QObject *filter)
{
    for (int i = 0; i < lab.size(); i++) {
        lab.at(i)->setProperty("dou_number", i+'A');
        lab.at(i)->installEventFilter(filter);
    }
}

void Doum::setODouNumber(const QVector<QLabel *> &lab, QObject *filter)
{
    if (oimg.isEmpty()) {
        setGDouNumber(lab, filter);
        return;
    }
    int m = lab.size() - 1;
    for (int i = 0; i < m; i++) {
        lab.at(i)->setProperty("dou_number", i+'A');
        lab.at(i)->installEventFilter(filter);
    }
    if (m >= 0) {
        lab.at(m)->setProperty("dou_number", 0);
        lab.at(m)->installEventFilter(filter);
    }
}

void Doum::setGDouImage(const QVector<QLabel *> &dLab, const QVector<QLabel *> &gLab, int n, int m)
{
    if (n >= 1 && n <= dn) {
        int i = n - 1;
        dLab.at(i)->setPixmap(dimg.at(i).at(m));
    }
    else if (n >= 'A' && n - 'A' <= gn) {
        int i = n - 'A';
        gLab.at(i)->setPixmap(gimg.at(i).at(m));
    }
}

void Doum::setODouImage(const QVector<QLabel *> &dLab, const QVector<QLabel *> &oLab, int n, int m)
{
    if (n >= 1 && n <= dn) {
        int i = n - 1;
        dLab.at(i)->setPixmap(dimg.at(i).at(m));
    }
    else {
        int i;
        if (n == 0)
            i = oLab.size() - 1;
        else if (n >= 'A' && n - 'A' <= gn)
            i = n - 'A';
        else
            return;
        oLab.at(i)->setPixmap(oimg.at(i).at(m));
    }
}

#include "MachineParamList.h"

int Doum::getDouWeight(MParam **mp, int n)
{
    int i;
    if (n >= 1 && n <= dn)
        i = n - 1;
    else if (n >= 21 && n <= 24)
        i = n - 21 + gn + dn;
    else if (n >= 'A' && n < 'A' + gn)
        i = n - 'A' + dn;
    else
        return 0;

    int doustatuscnt = (dn + 1) / 2;
    return mp[i + doustatuscnt]->valuenp().toUInt();
}

int Doum::getDouStatus(MParam **mp, int i, int jyd)
{
    Q_UNUSED(jyd);
    uint s = mp[i/2]->valuenp().toUInt();
    int p = i % 2;
    return (s >> 8*p) & 0x0F;
}

int Doum::getDouStatus(const QByteArray &ba, int i)
{
    uint s = (uchar)ba.at(i/2);
    int p = i % 2;
    return (s >> 4*p) & 0x0F;
}
