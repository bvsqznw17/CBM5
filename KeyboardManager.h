#ifndef KEYBOARDMANAGER_H
#define KEYBOARDMANAGER_H

#define KEYBMET(LE) LE->text(), LE->whatsThis(), LE

#include <QWidget>
#include <functional>
#include "Machine.h"
#include "Keyboard.h"
#include "KeyboardInputInfo.h"
class QLineEdit;

class KeyboardManager : QObject
{
    Q_OBJECT
public:
    typedef std::function<KbInputInfo()> IIFunc;
    struct Group {
        QString title;
        QString help;
        QLineEdit *edit;
        MParam *mp;
        KbInputInfo ii;
        ushort flag;
        ushort ecnt;
        IIFunc iif;
        KbInputInfo getii() const;
        QString getmpstr() const;
    };

public:
    KeyboardManager(Machine *mac, QWidget *parent);
    ~KeyboardManager();

    Keyboard *keyb;
    Keyboard *keybn;

    inline void interruptTabOrder() { m_tabOrder = false; }
    inline void setNeedKeyboard(bool b) { m_needKeyb = b; }
    inline int currenti() { return m_list.size(); }
    inline int startadd() { interruptTabOrder(); return  currenti(); }
    inline int endadd() { return currenti(); }

    void refreshMParam(int starti, int endi);
    void setText(int i, const QString &text);
    bool writeMP(int i);
    bool inline writeMP(QObject *edit) { return writeMP(indexOf(edit)); }
    const SignalEcho &getEcho(int i);

    inline const Group &group(int i) { return m_list.at(i); }
    inline QLineEdit *edit(int i) { return m_list.at(i).edit; }
    inline MParam *mp(int i) { return m_list.at(i).mp; }
    inline int static indexOf(QObject *edit) { QVariant v = edit->property("keybm_id"); return v.isValid() ? v.toInt() : -1; }
    inline bool static isError(QObject *edit) { return edit->property("keybm_err").toBool(); }
    inline bool isError(int i) { return m_list.at(i).flag & 0x4000; }

    int add(QLineEdit *edit, const KbInputInfo &ii = KbInputInfo());
    int add(QLineEdit *edit, MParam *mp, ushort flag = 0);
    int add(const QString &title, QLineEdit *edit, const KbInputInfo &ii = KbInputInfo());
    int add(const QString &title, QLineEdit *edit, MParam *mp, ushort flag = 0);
    int add(const QString &title, const QString &help, QLineEdit *edit, const KbInputInfo &ii = KbInputInfo());
    int add(const QString &title, const QString &help, QLineEdit *edit, MParam *mp, ushort flag = 0);

#define KEYBM_MPIINo        0x01 // 不使用机器参数的输入信息
#define KEYBM_WriteNo       0x02 // 在onEditFinished中不写参数
#define KEYBM_RefreshNo     0x04 // 在refreshMParam中不刷新
#define KEYBM_MPIIOnly      0x06 // 仅使用机器参数的输入信息
#define KEYBM_WriteOnly     0x05 // 仅在onEditFinished中写参数
#define KEYBM_RefreshOnly   0x03 // 仅在refreshMParam中刷新
#define KEYBM_UnitNo        0x08 // 在刷新MParam时不使用单位
#define KEYBM_unit          0x10 // 在刷新MParam时使用单位但不带空格

    int add(const QString &title, const QString &help, QLineEdit *edit, MParam *mp, const KbInputInfo &ii, ushort flag);
    void addTabOrder(QWidget *w);
    void setIIFunc(const IIFunc &iifunc, int i);
    void inline setIIFunc(const IIFunc &iifunc) { setIIFunc(iifunc, currenti()-1); }

private:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onShowKeyb(QObject *o);
    void onShowToopTip(QObject *o);
    void onClicked(QObject *o);
    void onEditFinished();
    void onTextEdited(const QString &text);
    void onActionFinished(int err, const SignalEcho_zy1 &echo);

private:
    void setErrState(int i, bool err);

private:
    Machine *m_mac;
    SignalEcho m_echo;
    SignalEchoIds m_ecIds;
    QList<Group> m_list;
    QWidget *m_addTabOrder;
    bool m_tabOrder;
    bool m_needKeyb;
    bool m_firstClick;
    bool m_isTextEdited;
};

#endif // KEYBOARDMANAGER_H
