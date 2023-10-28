#ifndef MWIDGET_H
#define MWIDGET_H

#include <QImage>
#include <QHash>
#include <QSet>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

struct QImages
{
    QImage v[6];
    char detached[6];
    QImages();
};

struct QPixmaps
{
    QPixmap v[6];
    QPixmaps();
    QPixmaps(const QImages &imgs);
    void operator =(const QImages &imgs);
};

class QImagesManager : public QObject
{
    Q_OBJECT
public:
    enum QImageStatus
    {
        Nomorl = 0,
        Pressed,
        Disabled,
        Checked,
        Checked_Pressed,
        Checked_Disabled
    };

    static QString resourceRoot;
    static QImagesManager def;
    static QHash<QString, QImages> load(const QImagesManager &imgm = def);

    QImagesManager(const QString &imfn = 0, QObject *parent = 0);
    QString rscRoot;
    QString imFileName;
    QSet<QString> invertSet;
    bool invertSetEnabled;
    inline void invertSetInsert(const QString &str)
    { if (invertSetEnabled) invertSet.insert(str); }

    void replace(const QHash<QString, QImages> &hash);
    QString joinPath(const QString &path) const;
    QPixmap get(const QString &name, QImageStatus status = Nomorl) const;
    QPixmaps gets(const QString &name) const;

signals:
    void ImageChange();

private slots:
    void onReplace(const QHash<QString, QImages> &hash);

private:
    QHash<QString, QPixmaps> m_hash;
};

// 有图片时，控件尺寸会被调整到与图片尺寸相同
// 不要调用setFixedSize，用setMinimumSize和setMaximumSize来代替
// 不要调用其父类的setMinimumSize和setMaximumSize
class QImageButton : public QPushButton
{
    Q_OBJECT
public:
    static char defi[6];
    explicit QImageButton(QWidget *parent = 0);

    void setImage(const QString &name, QImagesManager *imgm = &QImagesManager::def);

    inline const QString &imgn() const { return m_name; }

    void setMinimumSize(int minw, int minh);
    inline void setMinimumSize(const QSize &size) { setMinimumSize(size.width(), size.height()); }
    inline void setMinimumWidth(int minw) { setMinimumSize(minw, m_minh); }
    inline void setMinimumHeight(int minh) { setMinimumSize(m_minw, minh); }

    void setMaximumSize(int maxw, int maxh);
    inline void setMaximumSize(const QSize &size) { setMaximumSize(size.width(), size.height()); }
    inline void setMaximumWidth(int maxw) { setMaximumSize(maxw, m_maxh); }
    inline void setMaximumHeight(int maxh) { setMaximumSize(m_maxw, maxh); }

    void downPaintKeep();
    void downPaintKeepCancel();
    bool disClickUntilKeepCancel();

    void releaseUpdate();

protected:
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void paintEvent(QPaintEvent *e);

private:
    void paintEvent2(QPaintEvent *e);

private slots:
    void onImageChange();

private:
    QImagesManager *m_imgm;
    QString m_name;
    QPixmaps m_pms;
    QPixmap *m_pm;
    int m_minw;
    int m_minh;
    int m_maxw;
    int m_maxh;

public:
    ushort onf_keepDownPaintAtMRE : 1;
    ushort m_mouseRlsEvent : 1;
    ushort m_downPaintKeep1 : 1;
    ushort m_downPaintKeep2 : 1;
    ushort m_isDisClick : 1;
    ushort m_unused : 12;
};

// 图片可使用各种对齐方式，控件可使用样式表
// 有图片时，控件最小尺寸将不会小于图片尺寸
// 不要调用setFixedSize，用setMinimumSize和setMaximumSize代替
// 不要调用其父类的setText和setMinimumSize
class QIconButton : public QPushButton
{
    Q_OBJECT
public:
    static char defi[6];
    explicit QIconButton(QWidget *parent = 0);

    void setImage(const QString &name, QImagesManager *imgm = &QImagesManager::def);

    void setText(const QString &text);

    inline const QString &imgn() const { return m_name; }
    inline const QString &text() const { return m_text; }

    void setMinimumSize(int minw, int minh);
    inline void setMinimumSize(const QSize &size) { setMinimumSize(size.width(), size.height()); }
    inline void setMinimumWidth(int minw) { setMinimumSize(minw, m_minh); }
    inline void setMinimumHeight(int minh) { setMinimumSize(m_minw, minh); }

    void setImageMargins(short l, short t, short r, short b);
    inline void setImageMargins(short m) { setImageMargins(m, m, m, m); }

    void setAlignment(Qt::Alignment align);
    inline Qt::Alignment alignment() const { return m_align; }

    void releaseUpdate();

protected:
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void paintEvent(QPaintEvent *e);

private slots:
    void onImageChange();

private:
    void updateMinimumSize();

private:
    QImagesManager *m_imgm;
    QString m_name;
    QPixmaps m_pms;
    QPixmap *m_pm;
    QString m_text;
    int m_minw;
    int m_minh;
    short m_margin_left;
    short m_margin_top;
    short m_margin_right;
    short m_margin_bottom;
    Qt::Alignment m_align;

public:
    ushort onf_keepDownPaintAtMRE : 1;
    ushort m_mouseRlsEvent : 1;
    ushort m_downPaintKeep1 : 1;
    ushort m_downPaintKeep2 : 1;
    ushort m_isClickable : 1;
    ushort m_unused : 12;
};

class QImageLabel : public QLabel
{
    Q_OBJECT
public:
    explicit QImageLabel(QWidget *parent = 0);

    void setImage(const QString &name, QImagesManager *imgm = &QImagesManager::def);

    inline const QString &imgn() const { return m_name; }

    void setMinimumSize(int minw, int minh);
    inline void setMinimumSize(const QSize &size) { setMinimumSize(size.width(), size.height()); }
    inline void setMinimumWidth(int minw) { setMinimumSize(minw, m_minh); }
    inline void setMinimumHeight(int minh) { setMinimumSize(m_minw, minh); }

    void setMaximumSize(int maxw, int maxh);
    inline void setMaximumSize(const QSize &size) { setMaximumSize(size.width(), size.height()); }
    inline void setMaximumWidth(int maxw) { setMaximumSize(maxw, m_maxh); }
    inline void setMaximumHeight(int maxh) { setMaximumSize(m_maxw, maxh); }

protected:
    virtual void paintEvent(QPaintEvent *e);

private slots:
    void onImageChange();

private:
    QImagesManager *m_imgm;
    QString m_name;
    QPixmap m_pm;
    int m_minw;
    int m_minh;
    int m_maxw;
    int m_maxh;
};

class QIconLabel : public QLabel
{
    Q_OBJECT
public:
    explicit QIconLabel(QWidget *parent = 0);

    void setImage(const QString &name, QImagesManager *imgm = &QImagesManager::def);

    void setText(const QString &text);

    inline const QString &imgn() const { return m_name; }
    inline const QString &text() const { return m_text; }

    void setMinimumSize(int minw, int minh);
    inline void setMinimumSize(const QSize &size) { setMinimumSize(size.width(), size.height()); }
    inline void setMinimumWidth(int minw) { setMinimumSize(minw, m_minh); }
    inline void setMinimumHeight(int minh) { setMinimumSize(m_minw, minh); }

    void setImageMargins(short l, short t, short r, short b);
    inline void setImageMargins(short m) { setImageMargins(m, m, m, m); }

protected:
    virtual void paintEvent(QPaintEvent *e);

private slots:
    void onImageChange();

private:
    void updateMinimumSize();

private:
    QImagesManager *m_imgm;
    QString m_name;
    QPixmap m_pm;
    QString m_text;
    int m_minw;
    int m_minh;
    short m_margin_left;
    short m_margin_top;
    short m_margin_right;
    short m_margin_bottom;
};

class QImageLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor DESIGNABLE true)
    Q_PROPERTY(QColor textColord READ textColord WRITE setTextColord DESIGNABLE true)

public:
    static char defi[6];
    explicit QImageLineEdit(QWidget *parent = 0);
    explicit QImageLineEdit(const QString &contents, QWidget *parent = 0);
    void init();

    void setImage(const QString &name, QImagesManager *imgm = &QImagesManager::def);
    void setImageValue(const QString &name, QImagesManager *imgm = &QImagesManager::def);

    void setText(const QString &text);
    void setTextValue(const QString &text);

    inline const QString &imgn() const { return m_name; }
    inline const QString &imgnv() const { return m_namev; }
    inline const QString &text() const { return m_text; }
    inline const QString &textv() const { return m_textv; }

    void setImageMargins(short l, short t, short r, short b);
    inline void setImageMargins(short m) { setImageMargins(m, m, m, m); }

    QColor textColor() const;
    void setTextColor(const QColor &c);
    QColor textColord() const;
    void setTextColord(const QColor &c);

protected:
    virtual void paintEvent(QPaintEvent *e);

private slots:
    void onImageChange();
    void onImageChangeV();
    void onTextChanged(const QString &text);

private:
    QImagesManager *m_imgm;
    QImagesManager *m_imgmv;
    QString m_name;
    QString m_namev;
    QPixmaps m_pms;
    QPixmaps m_pmsv;
    bool m_isPmv;
    QString m_text;
    QString m_textv;
    short m_margin_left;
    short m_margin_top;
    short m_margin_right;
    short m_margin_bottom;
    QColor m_text_color;
    QColor m_text_colord;
};

class ButtonCheck : public QImageButton
{
    Q_OBJECT
public:
    explicit ButtonCheck(QWidget *parent = 0);
protected:
    virtual void nextCheckState();
};

class ButtonDnPtKeep : public QImageButton
{
    Q_OBJECT
public:
    explicit ButtonDnPtKeep(QWidget *parent = 0);
protected:
    virtual void nextCheckState();
};

#endif // MWIDGET_H
