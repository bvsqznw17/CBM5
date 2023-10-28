#include "MWidget.h"
#include <QFile>
#include <QTextStream>
#include <QPainter>

QImages::QImages()
{
    detached[0] = 0;
    detached[1] = 1;
    detached[2] = 2;
    detached[3] = 3;
    detached[4] = 4;
    detached[5] = 5;
}

QPixmaps::QPixmaps()
{

}

QPixmaps::QPixmaps(const QImages &imgs)
{
    *this = imgs;
}

void QPixmaps::operator =(const QImages &imgs)
{
    if (imgs.detached[0] == 0) v[0] = QPixmap::fromImage(imgs.v[0]);
    else v[0] = v[(int)imgs.detached[0]];

    if (imgs.detached[1] == 1) v[1] = QPixmap::fromImage(imgs.v[1]);
    else v[1] = v[(int)imgs.detached[1]];

    if (imgs.detached[2] == 2) v[2] = QPixmap::fromImage(imgs.v[2]);
    else v[2] = v[(int)imgs.detached[2]];

    if (imgs.detached[3] == 3) v[3] = QPixmap::fromImage(imgs.v[3]);
    else v[3] = v[(int)imgs.detached[3]];

    if (imgs.detached[4] == 4) v[4] = QPixmap::fromImage(imgs.v[4]);
    else v[4] = v[(int)imgs.detached[4]];

    if (imgs.detached[5] == 5) v[5] = QPixmap::fromImage(imgs.v[5]);
    else v[5] = v[(int)imgs.detached[5]];
}

void QImagesManager_invertPixels(QImage &img) {
    if (img.colorCount() == 0)
        img.invertPixels();
    else {
        QVector<QRgb> ct = img.colorTable();
        QRgb *d = ct.data();
        quint32 *p = (quint32*)d;
        quint32 *end = (quint32*)(d + ct.size());
        while (p < end)
            *p++ ^= 0x00ffffff;
        img.setColorTable(ct);
    }
}

QString QImagesManager::resourceRoot;
QImagesManager QImagesManager::def(":/def.im");
QHash<QString, QImages> QImagesManager::load(const QImagesManager &imgm)
{
    QHash<QString, QImages> hash;
    QStringList list;

    QFile file(imgm.joinPath(imgm.imFileName));
    if (!file.open(QIODevice::ReadOnly))
        return hash;
    QTextStream in(&file);
    in.setCodec("UTF-8");
    while (!in.atEnd()) {
        list.append(in.readLine());
    }
    file.close();

    foreach (const QString &str, list) {
        // 根据分割符|指定各个状态的图片
        QStringList strSplit = str.split('|');
        if (strSplit.size() != 7)
            continue;
        QImages imgs;
        for (int i = 1; i < 7; i++) {
            if (strSplit.at(i).isEmpty())
                continue;
            int j = 1;
            for (; j < i; j++) {
                if (strSplit.at(i) == strSplit.at(j))
                    break;
            }
            if (i == j) {
                imgs.v[i - 1] = QImage(imgm.joinPath(strSplit.at(i)));
                imgs.v[i - 1] = imgs.v[i - 1].convertToFormat(QImage::Format_ARGB32_Premultiplied);
            }
            else {
                imgs.v[i - 1] = imgs.v[j - 1];
                imgs.detached[i - 1] = j - 1;
            }
        }

        if (imgm.invertSet.contains(strSplit.at(0))) {
            // 若未指定preesed则自动反色
            if (strSplit.at(1+1).isEmpty()) {
                imgs.v[1] = imgs.v[0];
                QImagesManager_invertPixels(imgs.v[1]);
            }
            if (strSplit.at(4+1).isEmpty()) {
                imgs.v[4] = imgs.v[3];
                QImagesManager_invertPixels(imgs.v[4]);
            }
        }

        hash.insert(strSplit.at(0), imgs);
    }

    return hash;
}

QImagesManager::QImagesManager(const QString &imfn, QObject *parent) :
    QObject(parent),
    imFileName(imfn)
{
    qRegisterMetaType<QHash<QString, QImages> >("QHash<QString, QImages>");
    invertSetEnabled = true;
}

void QImagesManager::replace(const QHash<QString, QImages> &hash)
{
    QMetaObject::invokeMethod(this, "onReplace",
                              QArgument<QHash<QString, QImages> >("QHash<QString, QImages>", hash));
}

QString QImagesManager::joinPath(const QString &path) const
{
    if (resourceRoot.isEmpty() && rscRoot.isEmpty())
        return path;

    if (path.startsWith(":"))
        return QString(":/%1/%2/%3")
                .arg(resourceRoot, rscRoot, path.mid(1));
    else
        return QString("./%1/%2/%3")
                .arg(resourceRoot, rscRoot, path);
}

QPixmap QImagesManager::get(const QString &name, QImageStatus status) const
{
    return m_hash.value(name).v[status];
}

QPixmaps QImagesManager::gets(const QString &name) const
{
    return m_hash.value(name);
}

void QImagesManager::onReplace(const QHash<QString, QImages> &hash)
{
    // 将QImage转为QPixmap，测试发现只有RGB32格式才能无耗时转换。但不能用带透明的图片肯定是不行的。
    // 如果ARGB32能无耗时转换，可以先在线程里转为ARGB32。否则考虑是否可以不转换，直接绘制QImage??
    // 根据QPixmap::toImage的描述，可以猜到只有ARGB32_Premultiplied，RGB32，RGB16和1-bit depth才能无耗时转换
    // 所以现在在线程里提前转换为了ARGB32_Premultiplied格式。
    m_hash.clear();
    QHashIterator<QString, QImages> i(hash);
    while (i.hasNext()) {
        i.next();
        m_hash.insert(i.key(), i.value());
        // 若不能无耗时转换，可以考虑一边m_hash.insert，一边i.remove
        // 这样可以降低内存占用峰值，但需要保证调用此函数的hash没有副本。
    }
    emit ImageChange();
}

char QImageButton::defi[6] = {0,0,1,1,3,4};
QImageButton::QImageButton(QWidget *parent) :
    QPushButton(parent)
{
    m_imgm = 0;
    m_pm = &m_pms.v[0];

    m_minw = minimumWidth();
    m_minh = minimumHeight();
    m_maxw = maximumWidth();
    m_maxh = maximumHeight();

    onf_keepDownPaintAtMRE = true;
    m_mouseRlsEvent = false;
    m_downPaintKeep1 = false;
    m_downPaintKeep2 = false;
    m_isDisClick = false;
}

void QImageButton::setImage(const QString &name, QImagesManager *imgm)
{
    if (m_name == name && m_imgm == imgm)
        return;
    if (imgm)
        imgm->invertSetInsert(name);

    const QImagesManager *o_imgm = m_imgm;
    m_name = name;
    m_imgm = imgm;
    if (m_imgm != o_imgm) {
        if (o_imgm) disconnect(o_imgm, SIGNAL(ImageChange()), this, SLOT(onImageChange()));
        if (m_imgm) connect(m_imgm, SIGNAL(ImageChange()), this, SLOT(onImageChange()));
    }

    onImageChange();
}

void QImageButton::setMinimumSize(int minw, int minh)
{
    m_minw = minw;
    m_minh = minh;

    if (!m_pm->isNull())
        return;

    QPushButton::setMinimumSize(m_minw, m_minh);
}

void QImageButton::setMaximumSize(int maxw, int maxh)
{
    m_maxw = maxw;
    m_maxh = maxh;

    if (!m_pm->isNull())
        return;

    QPushButton::setMaximumSize(m_maxw, m_maxh);
}

void QImageButton::downPaintKeep()
{
    if (!m_downPaintKeep1) {
        m_downPaintKeep1 = true;
        update();
    }
}

void QImageButton::downPaintKeepCancel()
{
    m_isDisClick = false;
    if (m_downPaintKeep1) {
        m_downPaintKeep1 = false;
        if (m_downPaintKeep2) {
            m_downPaintKeep2 = false;
            setAttribute(Qt::WA_OpaquePaintEvent, false);
        }
        update();
    }
}

bool QImageButton::disClickUntilKeepCancel()
{
    bool b = m_isDisClick;
    m_isDisClick = true;
    return b;
}

void QImageButton::releaseUpdate()
{
    if (m_mouseRlsEvent) {
        m_mouseRlsEvent = false;
        setAttribute(Qt::WA_OpaquePaintEvent, false);
        update();
    }
}

void QImageButton::mouseReleaseEvent(QMouseEvent *e)
{
    if (onf_keepDownPaintAtMRE) {
        m_mouseRlsEvent = true;
        setAttribute(Qt::WA_OpaquePaintEvent, true);

        QPushButton::mouseReleaseEvent(e);

        releaseUpdate();
    }
    else
        QPushButton::mouseReleaseEvent(e);
}

void QImageButton::paintEvent(QPaintEvent *e)
{
    if (m_mouseRlsEvent)
        return;

    if (m_downPaintKeep2) {
        m_downPaintKeep2 = false;
        setAttribute(Qt::WA_OpaquePaintEvent, false);
        return;
    }
    if (m_downPaintKeep1 && !isDown()) {
        setDown(true);
        paintEvent2(e);
        setDown(false);

        m_downPaintKeep2 = true;
        setAttribute(Qt::WA_OpaquePaintEvent, true);
        return;
    }

    paintEvent2(e);
}

void QImageButton::paintEvent2(QPaintEvent *e)
{
    int i;
    if (!isChecked()) {
        if (isDown()) {
            i = 1;
        } else if (isEnabled()) {
            i = 0;
        } else {
            i = 2;
        }
    } else {
        if (isDown()) {
            i = 4;
        } else if (isEnabled()) {
            i = 3;
        } else {
            i = 5;
        }
    }

    while (i != 0 && m_pms.v[i].isNull())
        i = defi[i];
    const QPixmap &pm = m_pms.v[i];

    if (!pm.isNull()) {
        QPainter pter(this);
        pter.drawPixmap(0, 0, pm);
    } else
        QPushButton::paintEvent(e);
}

void QImageButton::onImageChange()
{
    if (m_imgm) {
        m_pms = m_imgm->gets(m_name);
        for (int i=0; i<6; i++) {
            if (!m_pms.v[i].isNull()) {
                m_pm = &m_pms.v[i];
                break;
            }
        }
    } else
        m_pms = QPixmaps();

    if (!m_pm->isNull()) {
        QPushButton::setMinimumSize(m_pm->size());
        QPushButton::setMaximumSize(m_pm->size());
    } else {
        QPushButton::setMinimumSize(m_minw, m_minh);
        QPushButton::setMaximumSize(m_maxw, m_maxh);
    }

    update();
}

char QIconButton::defi[6] = {0,0,1,1,3,4};
QIconButton::QIconButton(QWidget *parent):
    QPushButton(parent)
{
    m_imgm = 0;
    m_pm = &m_pms.v[0];

    m_minw = minimumWidth();
    m_minh = minimumHeight();
    m_margin_left = m_margin_top = 0;
    m_margin_right = m_margin_bottom = 0;
    m_align = Qt::AlignCenter;

    onf_keepDownPaintAtMRE = true;
    m_mouseRlsEvent = false;
}

void QIconButton::setImage(const QString &name, QImagesManager *imgm)
{
    if (m_name == name && m_imgm == imgm)
        return;
    if (imgm)
        imgm->invertSetInsert(name);

    const QImagesManager *o_imgm = m_imgm;
    m_name = name;
    m_imgm = imgm;
    if (m_imgm != o_imgm) {
        if (o_imgm) disconnect(o_imgm, SIGNAL(ImageChange()), this, SLOT(onImageChange()));
        if (m_imgm) connect(m_imgm, SIGNAL(ImageChange()), this, SLOT(onImageChange()));
    }

    onImageChange();
}

void QIconButton::setText(const QString &text)
{
    m_text = text;

    if (!m_pm->isNull())
        return;

    QPushButton::setText(m_text);
}

void QIconButton::setMinimumSize(int minw, int minh)
{
    m_minw = minw;
    m_minh = minh;
    updateMinimumSize();
}

void QIconButton::setImageMargins(short l, short t, short r, short b)
{
    m_margin_left = l;
    m_margin_top = t;
    m_margin_right = r;
    m_margin_bottom = b;
    updateMinimumSize();
}

void QIconButton::setAlignment(Qt::Alignment align)
{
    if (m_align != align) {
        m_align = align;
        update();
    }
}

void QIconButton::releaseUpdate()
{
    if (m_mouseRlsEvent) {
        m_mouseRlsEvent = false;
        setAttribute(Qt::WA_OpaquePaintEvent, false);
        update();
    }
}

void QIconButton::mouseReleaseEvent(QMouseEvent *e)
{
    if (onf_keepDownPaintAtMRE) {
        m_mouseRlsEvent = true;
        setAttribute(Qt::WA_OpaquePaintEvent, true);

        QPushButton::mouseReleaseEvent(e);

        releaseUpdate();
    }
    else
        QPushButton::mouseReleaseEvent(e);
}

void QIconButton::paintEvent(QPaintEvent *e)
{
    if (m_mouseRlsEvent)
        return;

    int i;
    if (!isChecked()) {
        if (isDown()) {
            i = 1;
        } else if (isEnabled()) {
            i = 0;
        } else {
            i = 2;
        }
    } else {
        if (isDown()) {
            i = 4;
        } else if (isEnabled()) {
            i = 3;
        } else {
            i = 5;
        }
    }

    while (i != 0 && m_pms.v[i].isNull())
        i = defi[i];
    const QPixmap &pm = m_pms.v[i];

    QPushButton::paintEvent(e);
    if (!pm.isNull()) {
        int x, y;

        if (alignment() & Qt::AlignLeft)
            x = m_margin_left;
        else if(alignment() & Qt::AlignRight)
            x = (width() - pm.width() - m_margin_right);
        else
            x = (width() - pm.width() + m_margin_left - m_margin_right) / 2;

        if (alignment() & Qt::AlignTop)
            y = m_margin_top;
        else if(alignment() & Qt::AlignBottom)
            y = (height() - pm.height()) - m_margin_bottom;
        else
            y = (height() - pm.height() + m_margin_top - m_margin_bottom) / 2;

        QPainter pter(this);
        pter.drawPixmap(x, y, pm);
    }
}

void QIconButton::onImageChange()
{
    if (m_imgm) {
        m_pms = m_imgm->gets(m_name);
        for (int i=0; i<6; i++) {
            if (!m_pms.v[i].isNull()) {
                m_pm = &m_pms.v[i];
                break;
            }
        }
    } else
        m_pms = QPixmaps();

    updateMinimumSize();
    QPushButton::setText(m_pm->isNull() ? m_text : "");

    update();
}

void QIconButton::updateMinimumSize()
{
    if (m_pm->isNull()) {
        QPushButton::setMinimumSize(m_minw, m_minh);
        return;
    }

    int imgw = m_pm->width() + m_margin_left + m_margin_right;
    int imgh = m_pm->height() + m_margin_top + m_margin_bottom;

    int maxw = m_minw > imgw ? m_minw : imgw;
    int maxh = m_minh > imgh ? m_minh : imgh;
    QPushButton::setMinimumSize(maxw, maxh);
}

QImageLabel::QImageLabel(QWidget *parent) :
    QLabel(parent)
{
    m_imgm = 0;

    m_minw = minimumWidth();
    m_minh = minimumHeight();
    m_maxw = maximumWidth();
    m_maxh = maximumHeight();
}

void QImageLabel::setImage(const QString &name, QImagesManager *imgm)
{
    if (m_name == name && m_imgm == imgm)
        return;

    const QImagesManager *o_imgm = m_imgm;
    m_name = name;
    m_imgm = imgm;
    if (m_imgm != o_imgm) {
        if (o_imgm) disconnect(o_imgm, SIGNAL(ImageChange()), this, SLOT(onImageChange()));
        if (m_imgm) connect(m_imgm, SIGNAL(ImageChange()), this, SLOT(onImageChange()));
    }

    onImageChange();
}

void QImageLabel::setMinimumSize(int minw, int minh)
{
    m_minw = minw;
    m_minh = minh;

    if (!m_pm.isNull())
        return;

    QLabel::setMinimumSize(m_minw, m_minh);
}

void QImageLabel::setMaximumSize(int maxw, int maxh)
{
    m_maxw = maxw;
    m_maxh = maxh;

    if (!m_pm.isNull())
        return;

    QLabel::setMaximumSize(m_maxw, m_maxh);
}

void QImageLabel::paintEvent(QPaintEvent *e)
{
    const QPixmap &pm = m_pm;

    if (!pm.isNull()) {
        QPainter pter(this);
        pter.drawPixmap(0, 0, pm);
    } else
        QLabel::paintEvent(e);
}

void QImageLabel::onImageChange()
{
    if (m_imgm) {
        m_pm = m_imgm->get(m_name);
    } else
        m_pm = QPixmap();

    if (!m_pm.isNull()) {
        QLabel::setMinimumSize(m_pm.size());
        QLabel::setMaximumSize(m_pm.size());
    } else {
        QLabel::setMinimumSize(m_minw, m_minh);
        QLabel::setMaximumSize(m_maxw, m_maxh);
    }

    update();
}

QIconLabel::QIconLabel(QWidget *parent) :
    QLabel(parent)
{
    m_imgm = 0;

    m_minw = minimumWidth();
    m_minh = minimumHeight();
    m_margin_left = m_margin_top = 0;
    m_margin_right = m_margin_bottom = 0;
}

void QIconLabel::setImage(const QString &name, QImagesManager *imgm)
{
    if (m_name == name && m_imgm == imgm)
        return;

    const QImagesManager *o_imgm = m_imgm;
    m_name = name;
    m_imgm = imgm;
    if (m_imgm != o_imgm) {
        if (o_imgm) disconnect(o_imgm, SIGNAL(ImageChange()), this, SLOT(onImageChange()));
        if (m_imgm) connect(m_imgm, SIGNAL(ImageChange()), this, SLOT(onImageChange()));
    }

    onImageChange();
}

void QIconLabel::setText(const QString &text)
{
    m_text = text;

    if (!m_pm.isNull())
        return;

    QLabel::setText(m_text);
}

void QIconLabel::setMinimumSize(int minw, int minh)
{
    m_minw = minw;
    m_minh = minh;
    updateMinimumSize();
}

void QIconLabel::setImageMargins(short l, short t, short r, short b)
{
    m_margin_left = l;
    m_margin_top = t;
    m_margin_right = r;
    m_margin_bottom = b;
    updateMinimumSize();
}

void QIconLabel::paintEvent(QPaintEvent *e)
{
    const QPixmap &pm = m_pm;

    QLabel::paintEvent(e);
    if (!pm.isNull()) {
        int x, y;

        if (alignment() & Qt::AlignLeft)
            x = m_margin_left;
        else if(alignment() & Qt::AlignRight)
            x = (width() - pm.width() - m_margin_right);
        else
            x = (width() - pm.width() + m_margin_left - m_margin_right) / 2;

        if (alignment() & Qt::AlignTop)
            y = m_margin_top;
        else if(alignment() & Qt::AlignBottom)
            y = (height() - pm.height()) - m_margin_bottom;
        else
            y = (height() - pm.height() + m_margin_top - m_margin_bottom) / 2;

        QPainter pter(this);
        pter.drawPixmap(x, y, pm);
    }
}

void QIconLabel::onImageChange()
{
    if (m_imgm) {
        m_pm = m_imgm->get(m_name);
    } else
        m_pm = QPixmap();

    updateMinimumSize();
    QLabel::setText(m_pm.isNull() ? m_text : "");

    update();
}

void QIconLabel::updateMinimumSize()
{
    if (m_pm.isNull()) {
        QLabel::setMinimumSize(m_minw, m_minh);
        return;
    }

    int imgw = m_pm.width() + m_margin_left + m_margin_right;
    int imgh = m_pm.height() + m_margin_top + m_margin_bottom;

    int maxw = m_minw > imgw ? m_minw : imgw;
    int maxh = m_minh > imgh ? m_minh : imgh;
    QLabel::setMinimumSize(maxw, maxh);
}

char QImageLineEdit::defi[6] = {0,0,0,0,3,3};
QImageLineEdit::QImageLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    init();
}

QImageLineEdit::QImageLineEdit(const QString &contents, QWidget *parent) :
    QLineEdit("", parent)
{
    init();
    m_text = contents;
}

void QImageLineEdit::init()
{
    setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    connect(this, SIGNAL(textChanged(QString)),
            this, SLOT(onTextChanged(QString)));

    m_imgm = 0;
    m_imgmv = 0;
    m_isPmv = false;

    m_margin_left = m_margin_right = 6;
    m_margin_top = m_margin_bottom = 0;

    m_text_color = Qt::black;
    m_text_colord = Qt::gray;
}

void QImageLineEdit::setImage(const QString &name, QImagesManager *imgm)
{
    if (m_name == name && m_imgm == imgm)
        return;

    const QImagesManager *o_imgm = m_imgm;
    m_name = name;
    m_imgm = imgm;
    if (m_imgm != o_imgm) {
        if (o_imgm) disconnect(o_imgm, SIGNAL(ImageChange()), this, SLOT(onImageChange()));
        if (m_imgm) connect(m_imgm, SIGNAL(ImageChange()), this, SLOT(onImageChange()));
    }

    onImageChange();
}

void QImageLineEdit::setImageValue(const QString &name, QImagesManager *imgm)
{
    if (m_namev == name && m_imgmv == imgm)
        return;

    const QImagesManager *o_imgm = m_imgmv;
    m_namev = name;
    m_imgmv = imgm;
    if (m_imgmv != o_imgm) {
        if (o_imgm) disconnect(o_imgm, SIGNAL(ImageChange()), this, SLOT(onImageChangeV()));
        if (m_imgmv) connect(m_imgmv, SIGNAL(ImageChange()), this, SLOT(onImageChangeV()));
    }

    onImageChangeV();
}

void QImageLineEdit::setText(const QString &text)
{
    m_text = text;
    update();
}

void QImageLineEdit::setTextValue(const QString &text)
{
    m_textv = text;
    if (!m_isPmv)
        QLineEdit::setText(text);
}

void QImageLineEdit::setImageMargins(short l, short t, short r, short b)
{
    m_margin_left = l;
    m_margin_top = t;
    m_margin_right = r;
    m_margin_bottom = b;
    update();
}

QColor QImageLineEdit::textColor() const
{
    return m_text_color;
}

void QImageLineEdit::setTextColor(const QColor &c)
{
    m_text_color = c;
}

QColor QImageLineEdit::textColord() const
{
    return m_text_colord;
}

void QImageLineEdit::setTextColord(const QColor &c)
{
    m_text_colord = c;
}

void QImageLineEdit::paintEvent(QPaintEvent *e)
{
    QLineEdit::paintEvent(e);
    QPainter pter(this);
    bool isChecked = property("Checked").toBool();
    bool isEnabled = property("Enabled").toBool();
    int i;
    if (!isChecked) {
        if (isEnabled) {
            i = 0;
        } else {
            i = 2;
        }
    } else {
        if (isEnabled) {
            i = 3;
        } else {
            i = 5;
        }
    }
    int iv = i;
    while (i != 0 && m_pms.v[i].isNull())
        i = defi[i];
    const QPixmap &pm = m_pms.v[i];
    while (iv != 0 && m_pmsv.v[iv].isNull())
        iv = defi[iv];
    const QPixmap &pmv = m_pmsv.v[iv];

    if (!pmv.isNull()) {
        QPoint p(width() - pmv.width() - m_margin_right,
                 (height() - pmv.height() + m_margin_top - m_margin_bottom) / 2);
        pter.drawPixmap(p, pmv);
    }

    if (!pm.isNull()) {
        QPoint p(m_margin_left,
                 (height() - pm.height() + m_margin_top - m_margin_bottom) / 2);
        pter.drawPixmap(p, pm);
    }
    else {
        QTextOption opt(Qt::AlignLeft | Qt::AlignVCenter);
        QRect r(m_margin_left,
                (m_margin_top - m_margin_bottom) / 2,
                width() - m_margin_left - m_margin_right,
                height());
        pter.setPen(this->isEnabled() ? m_text_color : m_text_colord);
        pter.drawText(r, m_text, opt);
    }
}

void QImageLineEdit::onImageChange()
{
    if (m_imgm)
        m_pms = m_imgm->gets(m_name);
    else
        m_pms = QPixmaps();

    update();
}

void QImageLineEdit::onImageChangeV()
{
    if (m_imgmv) {
        m_pmsv = m_imgmv->gets(m_namev);
        m_isPmv = !m_pmsv.v[0].isNull() || !m_pmsv.v[2].isNull() ||
                !m_pmsv.v[3].isNull() || !m_pmsv.v[5].isNull();
    }
    else
        m_pmsv = QPixmaps();

    blockSignals(true);
    QLineEdit::setText(!m_isPmv ? m_textv : "");
    blockSignals(false);

    update();
}

void QImageLineEdit::onTextChanged(const QString &text)
{
    m_textv = text;
    if (m_isPmv) {
        blockSignals(true);
        QLineEdit::setText("");
        blockSignals(false);
    }
}

ButtonCheck::ButtonCheck(QWidget *parent) :
    QImageButton(parent)
{
    setCheckable(true);
}

void ButtonCheck::nextCheckState()
{
    setChecked(true);
}

ButtonDnPtKeep::ButtonDnPtKeep(QWidget *parent) :
    QImageButton(parent)
{
    setCheckable(true);
}

void ButtonDnPtKeep::nextCheckState()
{
    downPaintKeep();
}
