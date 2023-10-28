#ifdef MY_X11
#define _GLIBCXX_USE_CXX11_ABI 0
#endif
#include "Doum.h"
#include "QLabel2.h"
#include <QFile>
#include <QTextStream>
#include <opencv/highgui.h>
#include <opencv/cv.h>
using namespace cv;

Doum::Doum()
{
    dn = 0;
    gn = 0;
    dng = 0;
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
            dimgd[i] = createDouImg(dpathi.arg(i+1));
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
            gimgd[i] = createDouImg(gpathi.arg(QChar(i+'A')));
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

QVector<QPixmap> Doum::createDouImg(const QString &path)
{
    int nAngle[16] = {0,8,18,28,38,48,58,68,78,88,98,116,136,160,172,180};

    QVector<QPixmap> v(16);
    QPixmap *pm = v.data();

    Mat mat = imread(path.toStdString(), -1);
    if(mat.type() != CV_8UC4)
        return v;

    int matSize = mat.rows * mat.cols * 3;
    Mat matBgr, matHsv, matHsv2;
    cvtColor(mat, matBgr, CV_BGRA2BGR);
    cvtColor(matBgr, matHsv, CV_BGR2HLS);
    matHsv2 = matHsv.clone();
    for (int i = 1; i < 15; i++) {
        for(int k = 0; k < matSize; k+=3) {
            matHsv2.data[0+k] = (matHsv.data[k] + nAngle[i]) % 180;
        }

        cvtColor(matHsv2, matBgr, CV_HLS2BGR);

        QImage img(mat.cols, mat.rows, QImage::Format_ARGB32);
        uchar *imgd = img.bits();
        for (int k = 0, k2 = 0; k < matSize; k+=3, k2+=4) {
            if (mat.data[3+k2]) {
                // 颜色混合(c2叠加到c1): a = a1+a2 - a1*a2; (c1*a1*(1-a2) + c2*a2) / a;
                // 提前将非完全透明的像素融入白色背景
                float af = mat.data[3+k2]/255.0;
                float rf = matBgr.data[2+k]/255.0;
                float gf = matBgr.data[1+k]/255.0;
                float bf = matBgr.data[0+k]/255.0;
                uchar r = qRound(((1-af)+rf*af) * 255);
                uchar g = qRound(((1-af)+gf*af) * 255);
                uchar b = qRound(((1-af)+bf*af) * 255);
                *(quint32*)(imgd+k2) = qRgba(r, g, b, 255);
            }
            else {
                *(quint32*)(imgd+k2) = 0;
            }
        }
        pm[i] = QPixmap::fromImage(img);
    }

    // 原图和灰图
    QImage img(mat.cols, mat.rows, QImage::Format_ARGB32);
    QImage imgGray(mat.cols, mat.rows, QImage::Format_ARGB32);
    uchar *imgd = img.bits();
    uchar *imgdGray = imgGray.bits();
    matSize = mat.cols * mat.rows * 4;
    for(int k = 0; k < matSize; k+=4) {
        if (mat.data[3+k]) {
            // 颜色混合(c2叠加到c1): a = a1+a2 - a1*a2; (c1*a1*(1-a2) + c2*a2) / a;
            // 提前将非完全透明的像素融入白色背景
            float af = mat.data[3+k]/255.0;
            float rf = mat.data[2+k]/255.0;
            float gf = mat.data[1+k]/255.0;
            float bf = mat.data[0+k]/255.0;
            uchar r = qRound(((1-af)+rf*af) * 255);
            uchar g = qRound(((1-af)+gf*af) * 255);
            uchar b = qRound(((1-af)+bf*af) * 255);
            int gray = qGray(mat.data[2+k], mat.data[1+k], mat.data[0+k]);
            gray = qRound(((1-af)+(gray/255.0)*af) * 255);
            *(quint32*)(imgd+k) = qRgba(r, g, b, 255);
            *(quint32*)(imgdGray+k) = qRgba(gray, gray, gray, 255);
        }
        else {
            *(quint32*)(imgd+k) = 0;
            *(quint32*)(imgdGray+k) = 0;
        }
    }
    pm[0] = QPixmap::fromImage(img);
    pm[15] = QPixmap::fromImage(imgGray);

    return v;
}

QVector<int> Doum::readPos(const QString &path, int rows, int cols)
{
    QVector<int> v(rows * cols, 0);
    QFile file(path);
    if(!file.open(QFile::ReadOnly))
        return v;

    int i = 0;
    int *data = v.data();
    QTextStream in(&file);
    for (; i < rows && !in.atEnd(); i++) {
        QString line = in.readLine();
        QStringList split = line.split(',');
        int j = 0;
        for (; j < cols && j < split.size(); j++)
            data[i*cols+j] = split.at(j).toInt();
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

//            if (si <= 0) sf += sf * sif;
//            else sf += (1 - sf) * sif;
//            if (sf < 0) sf = 0;
//            else if (sf > 1) sf = 1;

//            c.setHslF(hf, sf, lf, af);
//            c = c.toRgb();
//            c.getRgbF(&rf, &gf, &bf, &af);

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
    if(L<0.5)
         S=delta / value;
    else
         S =delta / (2 - value);

    if (increment >= 0) {
        if((increment + S) >= 1)
            alpha = S;
        else
            alpha = 1 - increment;
        alpha = 1/alpha - 1;
        rf = rf+(rf- L) * alpha;
        gf = gf+(gf - L) *alpha;
        bf = bf+(bf - L) *alpha;
    }
    else {
        alpha=increment;
        rf = L + (rf- L) *(1+alpha);
        gf = L +(gf- L) * (1+alpha);
        bf = L +(bf- L) * (1+alpha);
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
        lab[i]->setPixmap(img.at(i).at(i%16));
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
            lab[i]->setStyleSheet(QString("font-size: %1px;").arg(pos[k+4]));
        else
            lab[i]->setStyleSheet("font-size: 14px;");
        lab[i]->setAlignment(Qt::AlignVCenter | (Qt::Alignment)pos[k+5]);
        lab[i]->setPalette(palette);
        lab[i]->setText("2134.5");
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
