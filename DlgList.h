#ifndef DLGLIST_H
#define DLGLIST_H

#include "DlgArm.h"
#include <QVariantList>

namespace Ui {
class DlgList;
}

class DlgList : public DlgArm
{
    Q_OBJECT

public:
    explicit DlgList(QWidget *parent = nullptr);
    ~DlgList();

    int currenti();
    QVariant currentv();
    QString currents();
    void setList(const QStringList &list, int cols = 1);
    void setVList(const QVariantList &list) { varlist = list; }
    void setListWidth(int w);
    void setMultiSelectionMode(bool b);

private slots:
    void on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

private:
    Ui::DlgList *ui;
    QVariantList varlist;
};

#endif // DLGLIST_H
