#ifndef DIALOGSETSPECIFY_H
#define DIALOGSETSPECIFY_H

#include "DlgArm.h"
#include <QVector>
class QListWidgetItem;

namespace Ui {
class DialogSetSpecify;
}

class DialogSetSpecify : public DlgArm
{
    Q_OBJECT

public:
    explicit DialogSetSpecify(int n, QWidget *parent = nullptr);
    ~DialogSetSpecify();

    void setSel(quint32 sel, quint32 dis);
    quint32 getSel();

    int currenti();
    void setMultiSelectionMode(bool b);

private slots:
    void on_listWidget_itemSelectionChanged();

private:
    Ui::DialogSetSpecify *ui;
    QVector<QListWidgetItem*> items;
    quint32 dis;
};

#endif // DIALOGSETSPECIFY_H
