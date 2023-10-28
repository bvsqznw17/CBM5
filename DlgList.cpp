#include "DlgList.h"
#include "ui_DlgList.h"

DlgList::DlgList(QWidget *parent) :
    DlgArm(parent),
    ui(new Ui::DlgList)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->OK, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->Cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

DlgList::~DlgList()
{
    delete ui;
}

int DlgList::currenti()
{
    QTableWidgetItem *item = ui->tableWidget->currentItem();
    if (!item)
        return  -1;
    return item->row() * ui->tableWidget->columnCount() + item->column();
}

QVariant DlgList::currentv()
{
    int i = currenti();
    if ((uint)i < (uint)varlist.size())
        return varlist.at(i);
    else
        return QVariant();
}

QString DlgList::currents()
{
    QTableWidgetItem *item = ui->tableWidget->currentItem();
    if (!item)
        return "";
    return item->text();
}

void DlgList::setList(const QStringList &list, int cols)
{
    ui->tableWidget->clearSelection();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(cols);
    ui->tableWidget->setRowCount((list.size()+cols-1)/cols);
    for (int i = 0; i < list.size(); i++) {
        QTableWidgetItem *item = new QTableWidgetItem(list.at(i));
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i/cols, i%cols, item);
    }
    int rows = ui->tableWidget->rowCount();
    if (rows <= 8) {
        ui->tableWidget->setMinimumHeight(50*rows+2);
        ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }
    else {
        ui->tableWidget->setMinimumHeight(50*8+2);
        ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    }
    this->adjustSize();
    this->resize(500, height());

    varlist.clear();
    ui->tableWidget->setCurrentItem(0);
    ui->OK->setEnabled(false);
}

void DlgList::setListWidth(int w)
{
    ui->tableWidget->setMinimumWidth(w);
}

void DlgList::setMultiSelectionMode(bool b)
{
    if (b)
        ui->tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    else
        ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
}

void DlgList::on_tableWidget_currentCellChanged(int row, int col, int , int )
{
    int i = row * ui->tableWidget->columnCount() + col;
    if (varlist.isEmpty())
        ui->OK->setEnabled(i >= 0);
    else if ((uint)i < (uint)varlist.size())
        ui->OK->setEnabled(varlist.at(i).isValid());
    else
        ui->OK->setEnabled(false);
}
