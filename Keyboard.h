#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "DlgArm.h"
#include "KeyboardInputInfo.h"

namespace Ui {
class Keyboard;
}

class Keyboard : public DlgArm
{
    Q_OBJECT

public:
    explicit Keyboard(QWidget *parent = nullptr);
    Keyboard(const KbInputInfo &ii, QWidget *parent = nullptr);
    ~Keyboard();

    QString value();
    void setOnlyNumberMode();
    void setPasswordMode();
    void reset(const KbInputInfo &ii = KbInputInfo(),
               const QString &title = QString(),
               const QString &help = QString());

protected:
    void timerEvent(QTimerEvent *event);

private slots:
    void on_Enter_clicked();

    void on_Cancel_clicked();

    void on_Clear_clicked();

    void on_Backspace_clicked();

    void onKey();

    void on_lineEdit_textChanged(const QString &text);

private:
    void abcCancel();
    void afterKey();

private:
    Ui::Keyboard *ui;
    QString uis_title;
    QString uis_prompt;

    KbInputInfo m_ii;
    bool m_isRange;

    int m_abcTimerId;
    void *m_abcLast;
    uint m_abcIndex;
};

#endif // KEYBOARD_H
