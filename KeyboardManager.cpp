#include "KeyboardManager.h"
#include "Machine.h"
#include <QLineEdit>
#include <QEvent>
#include <QFocusEvent>
#include <QToolTip>
#include <QApplication>
#include <QDebug>

KeyboardManager::KeyboardManager(Machine *mac, QWidget *parent) :
    QObject(parent)
{
    m_mac = mac;
    keyb = new Keyboard(parent);
    keybn = new Keyboard(parent);
    keybn->setOnlyNumberMode();

    m_addTabOrder = 0;
    m_tabOrder = false;
    m_needKeyb = true;
    m_firstClick = false;
    m_isTextEdited = false;

    if (m_mac)
        m_mac->echom.create(m_echo, m_ecIds, this, "onActionFinished");
}

KeyboardManager::~KeyboardManager()
{
    m_mac->echom.destroy(m_ecIds, this);
}

void KeyboardManager::refreshMParam(int starti, int endi)
{
    for (int i = starti; i < endi; i++) {
        // 1.正在输入的参数不需刷新
        // 2.仅使用机器参数的输入信息不需刷新
        // 3.输入完成但还没收到下位机的回应不需刷新(避免闪烁)
        const Group &g = m_list.at(i);
        if (g.mp == 0)
            continue;
        if (g.edit->hasFocus())
            continue;
        if (g.flag & KEYBM_RefreshNo)
            continue;
        if (g.ecnt != 0)
            continue;
        g.edit->setText(g.getmpstr());
        setErrState(i, false);
    }
}

void KeyboardManager::setText(int i, const QString &text)
{
    const Group &g = m_list.at(i);
    if (g.edit->hasFocus())
        return;
    if (g.ecnt != 0)
        return;
    g.edit->setText(text);
    setErrState(i, false);
}

bool KeyboardManager::writeMP(int i)
{
    const Group &g = m_list.at(i);
    QString text = g.edit->text();
    if (isError(i))
        return false;

    m_echo.val = i;
    m_list[i].ecnt++;
    m_mac->write(g.mp, text, m_echo);
    return true;
}

const SignalEcho &KeyboardManager::getEcho(int i)
{
    m_echo.val = i;
    m_list[i].ecnt++;
    return m_echo;
}

int KeyboardManager::add(QLineEdit *edit, const KbInputInfo &ii)
{
    return add(0, 0, edit, 0, ii, 0);
}

int KeyboardManager::add(QLineEdit *edit, MParam *mp, ushort flag)
{
    return add(0, 0, edit, mp, KbInputInfo(), flag);
}

int KeyboardManager::add(const QString &title, QLineEdit *edit, const KbInputInfo &ii)
{
    return add(title, 0, edit, 0, ii, 0);
}

int KeyboardManager::add(const QString &title, QLineEdit *edit, MParam *mp, ushort flag)
{
    return add(title, 0, edit, mp, KbInputInfo(), flag);
}

int KeyboardManager::add(const QString &title, const QString &help, QLineEdit *edit, const KbInputInfo &ii)
{
    return add(title, help, edit, 0, ii, 0);
}

int KeyboardManager::add(const QString &title, const QString &help, QLineEdit *edit, MParam *mp, ushort flag)
{
    return add(title, help, edit, mp, KbInputInfo(), flag);
}

int KeyboardManager::add(const QString &title, const QString &help, QLineEdit *edit, MParam *mp, const KbInputInfo &ii, ushort flag)
{
    if (m_tabOrder)
        QWidget::setTabOrder(m_addTabOrder ? m_addTabOrder : m_list.constLast().edit, edit);
    m_tabOrder = true;
    m_addTabOrder = 0;

    edit->setProperty("keybm_id", m_list.size());
    edit->installEventFilter(this);
    if (!edit->isReadOnly()) {
        connect(edit, SIGNAL(editingFinished()),
                this, SLOT(onEditFinished()));
        connect(edit, SIGNAL(textEdited(QString)),
                this, SLOT(onTextEdited(QString)));
    }
    else {
        flag |= KEYBM_RefreshNo;
    }

    Group g;
    g.title = title;
    g.help = help;
    g.edit = edit;
    g.mp = mp;
    g.ii = ii;
    g.flag = flag;
    g.ecnt = 0;
    g.iif = 0;
    m_list.append(g);

    return m_list.size() - 1;
}

void KeyboardManager::addTabOrder(QWidget *w)
{
    if (m_tabOrder)
        QWidget::setTabOrder(m_addTabOrder ? m_addTabOrder : m_list.constLast().edit, w);
    m_tabOrder = true;
    m_addTabOrder = w;
}

void KeyboardManager::setIIFunc(const KeyboardManager::IIFunc &iifunc, int i)
{
    m_list[i].iif = iifunc;
}

bool KeyboardManager::eventFilter(QObject *watched, QEvent *event)
{
    QLineEdit *edit = (QLineEdit *)watched;
    if (edit->isReadOnly()) {
        if (event->type() == QEvent::MouseButtonRelease) {
            QMetaObject::invokeMethod(this, "onClicked",
                                      Qt::QueuedConnection,
                                      Q_ARG(QObject *, watched));
        }
        return false;
    }
    if (m_needKeyb) {
        if (event->type() == QEvent::MouseButtonRelease) {
            QMetaObject::invokeMethod(this, "onShowKeyb",
                                      Qt::QueuedConnection,
                                      Q_ARG(QObject *, watched));
        }
    }
    else {
        if (event->type() == QEvent::FocusIn) {
            m_isTextEdited = false;
            QFocusEvent *e = (QFocusEvent *)event;
            if (e->reason() != Qt::MouseFocusReason) {
                QMetaObject::invokeMethod(this, "onShowToopTip",
                                          Qt::QueuedConnection,
                                          Q_ARG(QObject *, watched));
            }
            else
                m_firstClick = true;
        }
        else if (event->type() == QEvent::MouseButtonRelease) {
            if (m_firstClick) {
                onShowToopTip(watched);
                QLineEdit *edit = (QLineEdit *)watched;
                edit->selectAll();
                m_firstClick = false;
            }
        }
        else if (event->type() == QEvent::ToolTip) {
            onShowToopTip(watched);
        }
    }
    return false;
}

void KeyboardManager::onShowKeyb(QObject *o)
{
    QLineEdit *edit = (QLineEdit *)o;
    QWidget *w = QApplication::focusWidget();
    if (!w || w != edit)
        return;

    edit->blockSignals(true);
    edit->clearFocus();
    edit->blockSignals(false);

    int id = edit->property("keybm_id").toInt();
    const Group &g = m_list.at(id);

    KbInputInfo ii = g.getii();
    bool ii_type_0 = ii.type() == '0';
    Keyboard *kb = ii_type_0 ? keybn : keyb;
    if (!ii_type_0 && ii.value().isEmpty())
        ii.setValue(g.edit->text());
    kb->reset(ii, g.title, g.help);
    if (kb->exec() == QDialog::Accepted) {
        edit->setText(kb->value());
        m_isTextEdited = true;
        setErrState(id, false);
        edit->editingFinished();
    }
}

void KeyboardManager::onShowToopTip(QObject *o)
{
    QLineEdit *edit = (QLineEdit *)o;
    int id = edit->property("keybm_id").toInt();
    const Group &g = m_list.at(id);
    KbInputInfo ii = g.getii();
    if (ii.prompt().isEmpty())
        return;

#ifdef Q_OS_WIN
    QPoint p(0, edit->height()-21+2);
#else
    QPoint p(0, edit->height()-16+2);
#endif
    QToolTip::showText(edit->mapToGlobal(p), ii.prompt());
}

void KeyboardManager::onClicked(QObject *o)
{
    QLineEdit *edit = (QLineEdit *)o;
    QWidget *w = QApplication::focusWidget();
    if (!w || w != edit)
        return;

    edit->blockSignals(true);
    edit->clearFocus();
    edit->blockSignals(false);
    edit->editingFinished();
}

void KeyboardManager::onEditFinished()
{
    if (!m_isTextEdited)
        return;

    QLineEdit *edit = (QLineEdit *)sender();
    int id = edit->property("keybm_id").toInt();
    const Group &g = m_list.at(id);

    KbInputInfo ii = g.getii();
    QString text = edit->text();
    if (!ii.isRange(text))
        return;

    if (g.mp && !(g.flag & KEYBM_WriteNo)) {
        m_echo.val = id;
        m_list[id].ecnt++;
        m_mac->write(g.mp, text, m_echo);
    }

    if (edit->hasFocus()) {
        edit->blockSignals(true);
        edit->clearFocus();
        edit->blockSignals(false);
    }
}

void KeyboardManager::onTextEdited(const QString &text)
{
    m_isTextEdited = true;

    QLineEdit *edit = (QLineEdit *)sender();
    int id = edit->property("keybm_id").toInt();
    const Group &g = m_list.at(id);

    KbInputInfo ii = g.getii();
    setErrState(id, !ii.isRange(text));
}

void KeyboardManager::onActionFinished(int err, const SignalEcho_zy1 &echo)
{
    Q_UNUSED(err);
    m_mac->echom.finish(echo);

    int id = echo.val.toInt();
    m_list[id].ecnt--;
    if (m_list.at(id).ecnt == (ushort)-1)
        qWarning("KeyboardManager::onActionFinished: m_gs.at(i).ecnt == -1");

    const Group &g = m_list.at(id);
    if (g.mp == 0)
        return;
    if (g.edit->hasFocus())
        return;
    if (g.flag & KEYBM_RefreshNo)
        return;
    if (g.ecnt != 0)
        return;
    g.edit->setText(g.getmpstr());
    setErrState(id, false);
}

void KeyboardManager::setErrState(int i, bool err)
{
    const Group &g = m_list.at(i);
    if (err && !(g.flag & 0x4000)) {
        m_list[i].flag = g.flag | 0x4000;
    }
    else if (!err && (g.flag & 0x4000)) {
        m_list[i].flag = g.flag & ~0x4000;
    }
    else
        return;
    g.edit->setProperty("keybm_err", err);
    g.edit->setStyle(g.edit->style());
}

KbInputInfo KeyboardManager::Group::getii() const
{
    if (iif)
        return iif();
    if (mp && !(flag & KEYBM_MPIINo))
        return KbInputInfo(mp->getRange());
    else
        return ii;
}

QString KeyboardManager::Group::getmpstr() const
{
    if (flag & KEYBM_UnitNo)
        return mp->vStr();
    if (flag & KEYBM_unit)
        return mp->vStru();
    return mp->vStrU();
}
