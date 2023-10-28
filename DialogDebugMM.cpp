#include "DialogDebugMM.h"
#include "ui_DialogDebugMM.h"

DialogDebugMM::DialogDebugMM(QWidget *parent) :
    DlgArm(parent),
    ui(new Ui::DialogDebugMM)
{
    ui->setupUi(this);
    connect(ui->OK, SIGNAL(clicked()), this, SLOT(close()));
}

DialogDebugMM::~DialogDebugMM()
{
    delete ui;
}

void DialogDebugMM::init(FormA *f, int mm)
{
    form = f;
    mmode = mm;

    QIcon icon(":/rc/icon/specify_dou.png");
    icon.addFile(":/rc/icon/specify_dou1.png", QSize(), QIcon::Selected);
    icon.addFile(":/rc/icon/specify_dou2.png", QSize(), QIcon::Disabled);
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->listWidget->setIconSize(QSize(100*0.77, 70*0.77));
    ui->listWidget->setSpacing(5);
    for (int i = 0; i < form->dn; i++) {
        QListWidgetItem *item = new QListWidgetItem(icon, QString::number(i+1));
        if ((mmode&0xF0) != MM_COLLECT && !MP.sys_DOU_State[i+1]->value().toInt())
            item->setFlags(item->flags() & ~(Qt::ItemIsEnabled | Qt::ItemIsSelectable));
        if ((mmode&0xF0) == MM_COLLECT && i >= 2)
            item->setFlags(item->flags() & ~(Qt::ItemIsEnabled | Qt::ItemIsSelectable));
        ui->listWidget->addItem(item);
    }
}

void DialogDebugMM::on_listWidget_itemPressed(QListWidgetItem *item)
{
    if (!item)
        return;

    int n = ui->listWidget->currentRow() + 1;
    form->writeCMD(n << 8 | CTL_MOTOR_TEST, ((mmode&0xF0)<<4)|(mmode&0x0F));
    form->waitCmdFns(3000, true);
    item->setSelected(false);
}
