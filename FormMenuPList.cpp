#include "FormMenuPList.h"
#include "ui_FormMenuPList.h"
#include "MTools.h"
#include <math.h>

FormMenuPList::FormMenuPList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuPList)
{
    ui->setupUi(this);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setColumnWidth(1, 260-2);
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 6; j++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(i, j, item);
        }
    }
}

FormMenuPList::~FormMenuPList()
{
    delete ui;
}

void FormMenuPList::init(FormA *f)
{
    form = f;
    mptrw = new MPTRWidget(this, &form->mac, MMP.PrmList, form->mpri);
    mptrw->reader.appendMP(MMP.sys_prm_ids);

    form->writeCMD(CTL_PRMLIST, 1);
    mptrw->readOnce();
}

void FormMenuPList::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();

    bool b = MTools::auth() >= 1;
    ui->btn_copy->setVisible(b);
    ui->btn_del->setVisible(b);

    int id = MP.prmlist_id->value().toInt();
    if (id < 1) id = 1;
    else if(id > 91) id = 91;
    form->writeCMD(CTL_PRMLIST, id);
    mptrw->readOnce();
}

void FormMenuPList::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormMenuPList::refreshUI()
{
    int prmsel[form->gn];
    for (int i = 0; i < form->gn; i++)
        prmsel[i] = MP.sys_prm_ids[i]->value().toInt();

    int id = MP.prmlist_id->value().toInt();
    int i = 0;
    for (; i < 10 && i+id < 100; i++) {
        QString str_prmsel;
        for (int j = 0; j < form->gn; j++)
            if (prmsel[j] && prmsel[j] == i+id)
                str_prmsel.append(QChar('A'+j));
        if (!str_prmsel.isEmpty())
            str_prmsel.prepend("-");

        QString str_id = i+id >= 95 ? "*%1%2" : "%1%2";
        str_id = str_id.arg(i+id).arg(str_prmsel);
        ui->tableWidget->item(i, 0)->setText(str_id);
        ui->tableWidget->item(i, 1)->setText(MP.prmlist_name[i]->vStr());
        /*组合模式为1,2时，单位为P量纲为0*/
        int dotn = MP.prmlist_WorP[i]->value().toInt() != 0 ?
                    0 : MP.sys_dot_num->value().toInt() % 4;
        double setw = MP.prmlist_SetWeight[i]->value().toInt()*pow(10,-dotn);
        double setw1 = MP.prmlist_SetWeight1[i]->value().toInt()*pow(10,-dotn);
        double setw2 = MP.prmlist_SetWeight2[i]->value().toInt()*pow(10,-dotn);
        ui->tableWidget->item(i, 2)->setText(QString::number(setw, 'f', dotn));
        ui->tableWidget->item(i, 3)->setText(QString::number(setw1, 'f', dotn));
        ui->tableWidget->item(i, 4)->setText(QString::number(setw2, 'f', dotn));
        ui->tableWidget->item(i, 5)->setText(MP.prmlist_speed[i]->vStr());

        QColor c = str_prmsel.isEmpty() ? Qt::transparent : Qt::cyan;
        for (int j = 0; j < 6; j++)
            ui->tableWidget->item(i, j)->setBackground(c);
    }
    for (; i < 10; i++) {
        for (int j = 0; j < 6; j++) {
            ui->tableWidget->item(i, j)->setText("");
        }
    }
}

void FormMenuPList::onRegUpdated(const MRegAreas &areas)
{
    if (areas.isIn(MMP.PrmList))
        refreshUI();
}

void FormMenuPList::on_lineup_clicked()
{
    int row = ui->tableWidget->currentRow();
    if (row < 0)
        ui->tableWidget->selectRow(0);
    else if (row == 0) {
        on_pageup_clicked();
        ui->tableWidget->selectRow(9);
    }
    else
        ui->tableWidget->selectRow(row-1);
}

void FormMenuPList::on_linedown_clicked()
{
    int row = ui->tableWidget->currentRow();
    if (row < 0)
        ui->tableWidget->selectRow(0);
    else if (row == 9) {
        on_pagedown_clicked();
        ui->tableWidget->selectRow(0);
    }
    else
        ui->tableWidget->selectRow(row+1);
}

void FormMenuPList::on_pageup_clicked()
{
    int id = MP.prmlist_id->value().toInt();
    id -= 10;
    if (id < 1) id = 1;
    else if(id > 91) id = 91;
    form->writeCMD(CTL_PRMLIST, id);
    mptrw->readOnce();
}

void FormMenuPList::on_pagedown_clicked()
{
    int id = MP.prmlist_id->value().toInt();
    id += 10;
    if (id < 1) id = 1;
    else if(id > 91) id = 91;
    form->writeCMD(CTL_PRMLIST, id);
    mptrw->readOnce();
}

void FormMenuPList::on_btn_sel_clicked()
{
    int row = ui->tableWidget->currentRow();
    int id = MP.prmlist_id->value().toInt() + row;
    if (row < 0 || id <= 0 || id >= 100)
        return;
    DlgOKCancel dlg(form);
    dlg.setText(tr("确定要选择程序%1吗？").arg(id));
    if (dlg.exec() == QDialog::Accepted) {
        form->writeCMD(CTL_PRM_SELECT, id);
        form->waitCmdFns(8000);
        form->writeCMD(0x00,CTL_PRM_WRITE,0x41,0x00);
        id = MP.prmlist_id->value().toInt();
        form->writeCMD(CTL_PRMLIST, id);
        mptrw->readOnce();
    }
}

void FormMenuPList::on_btn_copy_clicked()
{
    int row = ui->tableWidget->currentRow();
    int id = MP.prmlist_id->value().toInt() + row;
    if (row < 0 || id <= 0 || id >= 100)
        return;
    DlgList dlg(form);
    dlg.setWindowTitle(ui->btn_copy->text());
    QStringList list;
    QVariantList vlist;
    list.reserve(100);
    vlist.reserve(100);
    for (int i = 1; i < 95; i++) {
        list.append(QString::number(i));
        vlist.append(i);
    }
    dlg.setList(list, 10);
    dlg.setVList(vlist);
    dlg.resize(760, dlg.height());
    if (dlg.exec() == QDialog::Accepted) {
        int dest = dlg.currentv().toInt();
        form->writeCMD(0x00,CTL_PRM_COPY,dest,id);
        form->waitCmdFns(8000);
        id = MP.prmlist_id->value().toInt();
        form->writeCMD(CTL_PRMLIST, id);
        mptrw->readOnce();
    }
}

void FormMenuPList::on_btn_del_clicked()
{
    int row = ui->tableWidget->currentRow();
    int id = MP.prmlist_id->value().toInt() + row;
    if (row < 0 || id <= 0 || id >= 100)
        return;
    DlgOKCancel dlg(form);
    dlg.setText(tr("确定要删除程序%1吗？").arg(id));
    if (dlg.exec() == QDialog::Accepted) {
        form->writeCMD(CTL_PRM_DELETE, id);
        form->waitCmdFns(8000);
        id = MP.prmlist_id->value().toInt();
        form->writeCMD(CTL_PRMLIST, id);
        mptrw->readOnce();
    }
}
