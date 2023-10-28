#include "DialogSetSpecify.h"
#include "ui_DialogSetSpecify.h"

DialogSetSpecify::DialogSetSpecify(int n, QWidget *parent) :
    DlgArm(parent),
    ui(new Ui::DialogSetSpecify)
{
    ui->setupUi(this);
    connect(ui->OK, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->Cancel, SIGNAL(clicked()), this, SLOT(reject()));

    QIcon icon(":/rc/icon/specify_dou.png");
    icon.addFile(":/rc/icon/specify_dou1.png", QSize(), QIcon::Selected);
    icon.addFile(":/rc/icon/specify_dou2.png", QSize(), QIcon::Disabled);
    ui->listWidget->setIconSize(QSize(100*0.77, 70*0.77));
    ui->listWidget->setSpacing(5);
    items.reserve(n);
    for (int i = 0; i < n; i++) {
        QListWidgetItem *item = new QListWidgetItem(icon, QString::number(i+1));
        ui->listWidget->addItem(item);
        items.append(item);
    }
    dis = 0;
}

DialogSetSpecify::~DialogSetSpecify()
{
    delete ui;
}

void DialogSetSpecify::setSel(quint32 sel, quint32 dis)
{
    ui->listWidget->blockSignals(true);
    for (int i = 0; i < items.size(); i++) {
        if ((dis & (0x01 << i)) && !(sel & (0x01 << i)))
            items.at(i)->setFlags(items.at(i)->flags() &
                                  ~(Qt::ItemIsEnabled | Qt::ItemIsSelectable));
        else
            items.at(i)->setFlags(items.at(i)->flags() |
                                  (Qt::ItemIsEnabled | Qt::ItemIsSelectable));
        items.at(i)->setSelected(sel & (0x01 << i));
    }
    this->dis = dis;
    ui->listWidget->blockSignals(false);
}

quint32 DialogSetSpecify::getSel()
{
    quint32 sel = 0;
    for (int i = 0; i < items.size(); i++)
        if (items.at(i)->isSelected())
            sel |= (0x01 << i);
    return sel;
}

int DialogSetSpecify::currenti()
{
    for (int i = 0; i < items.size(); i++)
        if (items.at(i)->isSelected())
            return i;
    return -1;
}

void DialogSetSpecify::setMultiSelectionMode(bool b)
{
    if (b)
        ui->listWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    else
        ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
}

void DialogSetSpecify::on_listWidget_itemSelectionChanged()
{
    for (int i = 0; i < items.size(); i++) {
        if ((dis & (0x01 << i)) && !items.at(i)->isSelected())
            items.at(i)->setFlags(items.at(i)->flags() &
                                  ~(Qt::ItemIsEnabled | Qt::ItemIsSelectable));
    }
}
