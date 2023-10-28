#ifndef DLGARM_H
#define DLGARM_H

#include <QDialog>

class QLabel;
class DlgArm : public QDialog
{
    Q_OBJECT
public:
    QDialog *dlg;

public:
    explicit DlgArm(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~DlgArm();

    void setHelpText(const QString &text);
    Q_SLOT void showHelpText();

    void setVisible(bool visible) override;

public Q_SLOTS:
    void open() override;
    int exec() override;
    void done(int) override;
    void accept() override;
    void reject() override;

protected:
    bool event(QEvent *ev) override;

private:
    void fixThisSize();

private:
    int tit_height;
    QWidget *tit;
    QLabel *tit_title;
    QPushButton *tit_help;
    QWidget *body;
    QString helptext;
};

#endif // DLGARM_H
