#include "FormMainRec.h"
#include "ui_FormMainRec.h"
#include "MPRWOperation.h"
#include "DialogWeightDeviation.h"
#include "DialogGraphBar.h"
#include <QHeaderView>
#include <QFileDialog>
#include <qmath.h>

#define PageSizeD 1
#define PageSizeR 10

FormMainRec::FormMainRec(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMainRec)
{
    ui->setupUi(this);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for (int i = 0; i < PageSizeR; i++) {
        for (int j = 0; j < 3; j++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(i, j, item);
        }
    }
}

FormMainRec::~FormMainRec()
{
    delete ui;
}

void FormMainRec::init(FormA *f)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP., form->mpri);

    reco = new RecOperate(form->recDB, this);
    connect(&reco->worker, SIGNAL(queryFns(QVariant)),
            this, SLOT(onQueryFns(QVariant)));
    rq.did = 0;
    rq.dn = PageSizeD;
    rq.di = 0;
    rq.drevs = true;
    rq.rid = 0;
    rq.rn = PageSizeR;
    rq.rrevs = true;
    rqr.rc.rcnt = 0;
    rqr.rc.dcnt = 0;
    isQuerying = 0;
    queryWait = false;

    dlg_wd = 0;
    reco_wd = 0;
    rq_wd = rq;
    rq_wd.rn = 100;
    dlg_bar = 0;
}

void FormMainRec::pageTop()
{
    rq.did = 0;
    rq.di = 0;
    rq.rid = 0;
}

void FormMainRec::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    connect(form->recDB, SIGNAL(recUpdate()),
            this, SLOT(onRecUpdate()),
            Qt::UniqueConnection);
    refreshUI();
    queryRec();
    queryRec_wd();

    form->writeCMD(CTL_INTO_PAGE, 2);
}

void FormMainRec::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
    disconnect(form->recDB, SIGNAL(recUpdate()),
               this, SLOT(onRecUpdate()));
}

void FormMainRec::refreshUI()
{
    bool run = form->isRun();
    ui->ClearRec->setEnabled(!run);
    ui->ExportRec->setEnabled(!run);
}

void FormMainRec::onRegUpdated(const MRegAreas &areas)
{
    Q_UNUSED(areas);
    if (areas.isIn(MP.runstatus_union))
        refreshUI();
}

void FormMainRec::onQueryFns(QVariant v)
{
    isQuerying--;
    RecQR_Handy r = v.value<RecQR_Handy>();
    if (!r.err) {
        rqr = r;
        updateRec();
        if (isQuerying == 0) {
            if ((r.rc.dcnt <= 0 || r.did > r.rc.dcnt - PageSizeD) && rq.did != 0) {
                rq.did = 0;
                rq.rid = 0;
                queryRec();
            }
            else if ((uint)r.di < (uint)r.ld.size() &&
                     r.rid > r.ld.at(r.di).drcnt - PageSizeR && rq.rid != 0) {
                rq.rid = 0;
                queryRec();
            }
        }
    }

    if (queryWait)
        queryRec();
}

void FormMainRec::onQueryFns_wd(QVariant v)
{
    if (dlg_wd == 0)
        return;

    RecQR_Handy r = v.value<RecQR_Handy>();
    if (!r.err) {
        int w0, w1, w2, p;
        w0 = w1 = w2 = p = 0;
        if (!r.ld.isEmpty()) {
            w0 = r.ld.at(0).weight1;
            w1 = r.ld.at(0).weight2;
            w2 = r.ld.at(0).weight3;
            p = r.ld.at(0).prec;
        }

        int n = r.lr.size();
        int w[n];
        for (int i = 0; i < n; i++)
            w[i] = r.lr.at(i).weight;

        GraphWeightDeviation *graph = dlg_wd->graph();
        graph->clear();
        graph->add(w, n);
        graph->setWeight(w0, w1, w2, p);
    }
}

void FormMainRec::onQueryFns_bar(QVariant v)
{
    if (dlg_bar == 0)
        return;

    RecQR_Handy r = v.value<RecQR_Handy>();
    if (!r.err) {
        int w0, w1, w2, p;
        w0 = w1 = w2 = p = 0;
        if (!r.ld.isEmpty()) {
            w0 = r.ld.at(0).weight1;
            w1 = r.ld.at(0).weight2;
            w2 = r.ld.at(0).weight3;
            p = r.ld.at(0).prec;
        }

        int n = r.lr.size();
        QVector<int> data(n);
        int *w = data.data();
        for (int i = 0; i < n; i++)
            w[i] = r.lr.at(i).weight - w0;

        int d = (w1 + w2) * 2 / 17 + 1;
        dlg_bar->setDistanceValue(d);
        GraphBar *graph = dlg_bar->graph();
        graph->setData(data);
        graph->setRange(w0, w1, w2, p);
        graph->setDistance(d);
        graph->adjustMinSize();
    }
}

void FormMainRec::onRecUpdate()
{
    queryRec();
    queryRec_wd();
}

void FormMainRec::on_lineup_clicked()
{
    int cnt = rqr.rc.dcnt;
    int cnt0 = cnt - PageSizeD;
    if (cnt <= PageSizeD)
        rq.did = 0;
    else if (cnt0 - rq.did > PageSizeD)
        rq.did += PageSizeD;
    else if (cnt0 - rq.did > 0)
        rq.did = cnt0;
    else
        rq.did = 0;
    rq.rid = 0;
    queryRec();
}

void FormMainRec::on_linedown_clicked()
{
    int cnt = rqr.rc.dcnt;
    if (cnt <= PageSizeD)
        rq.did = 0;
    else if (rq.did > PageSizeD)
        rq.did -= PageSizeD;
    else if (rq.did > 0)
        rq.did = 0;
    else
        rq.did = cnt - PageSizeD;
    rq.rid = 0;
    queryRec();
}

void FormMainRec::on_pageup_clicked()
{
    rq.did += 10;
    if (rq.did < 0)
        rq.did = 0;
    else if (rq.did >= rqr.rc.dcnt)
        rq.did = rqr.rc.dcnt - 1;
    rq.rid = 0;
    queryRec();
}

void FormMainRec::on_pagedown_clicked()
{
    rq.did -= 10;
    if (rq.did < 0)
        rq.did = 0;
    else if (rq.did >= rqr.rc.dcnt)
        rq.did = rqr.rc.dcnt - 1;
    rq.rid = 0;
    queryRec();
}

void FormMainRec::on_jt_up_clicked()
{
    if (rqr.di < 0 || rqr.di >= rqr.ld.size())
        return;

    int cnt = rqr.ld.at(rqr.di).drcnt;
    if (cnt <= PageSizeR)
        rq.rid = 0;
    else if (rq.rid > PageSizeR)
        rq.rid -= PageSizeR;
    else if (rq.rid > 0)
        rq.rid = 0;
    else
        rq.rid = cnt - PageSizeR;
    queryRec();
}

void FormMainRec::on_jt_down_clicked()
{
    if (rqr.di < 0 || rqr.di >= rqr.ld.size())
        return;

    int cnt = rqr.ld.at(rqr.di).drcnt;
    int cnt0 = cnt - PageSizeR;
    if (cnt <= PageSizeR)
        rq.rid = 0;
    else if (cnt0 - rq.rid > PageSizeR)
        rq.rid += PageSizeR;
    else if (cnt0 - rq.rid > 0)
        rq.rid = cnt0;
    else
        rq.rid = 0;
    queryRec();
}

void FormMainRec::on_ClearRec_clicked()
{
    DlgOKCancel dlg(form);
    dlg.setText(tr("确定要清空记录吗？"));
    if (dlg.exec() == QDialog::Accepted)
        reco->clear();
}

void FormMainRec::on_ExportRec_clicked()
{
#ifndef MYOPT_WINDOW
    DlgOKCancel dlg(form);
    dlg.setText(tr("确定要导出记录吗？"));
    if(dlg.exec() == QDialog::Rejected)
        return;
#else
    QString dir = QFileDialog::getExistingDirectory(form);
    if (dir.isEmpty())
        return;
#endif

    QString time = form->dt().toString("yyyyMMdd_hhmmss");
    QString filename = QString("cbm5_record_%1").arg(time);

#ifdef MYOPT_WINDOW
    filename = QString("%1/%2").arg(dir, filename);
#endif

    DlgProgress progress(form, true);
    connect(&reco->worker, SIGNAL(progress(int,int)),
            &progress, SLOT(onProgress(int,int)));
    connect(&progress, SIGNAL(cancel()),
            reco, SLOT(cancel()));
    reco->exportRec(filename);
    progress.exec();
    DlgInfo::info(form, MPRWOperation::errStr(progress.result()));
}

void FormMainRec::on_WeightDeviation_clicked()
{
    rq_wd.did = rq.did;
    reco_wd = new RecOperate(form->recDB, this);
    connect(&reco_wd->worker, SIGNAL(queryFns(QVariant)),
            this, SLOT(onQueryFns_wd(QVariant)));
    reco_wd->query(rq_wd);

    DialogWeightDeviation dlg(form);
    dlg_wd = &dlg;
    dlg.graph()->clear();
    dlg.exec();

    reco_wd->deleteLater();
    reco_wd = 0;
    dlg_wd = 0;
}

void FormMainRec::on_barGraph_clicked()
{
    RecOperate *reco = new RecOperate(form->recDB, this);
    connect(&reco->worker, SIGNAL(queryFns(QVariant)),
            this, SLOT(onQueryFns_bar(QVariant)));
    reco->queryDRec(rqr.did);

    DialogGraphBar dlg(form);
    dlg_bar = &dlg;
    dlg.graph()->clear();
    dlg.exec();

    reco->deleteLater();
    reco = 0;
    dlg_bar = 0;
}

void FormMainRec::queryRec()
{
    if (isQuerying >= 1) {
        queryWait = true;
        return;
    }

    isQuerying++;
    queryWait = false;
    reco->query(rq);
}

void FormMainRec::queryRec_wd()
{
    if (reco_wd && rq_wd.did == 0)
        reco_wd->query(rq_wd);
}

static QString vStrU(qint64 v, int p, int u)
{
    int worP = u >> 4;
    if (worP == 1 || worP == 2)
        return QString::number(v).append(" P");
    QString ustr;
    switch (u & 0x0F) {
    case 0: ustr = " g"; break;
    case 1: ustr = " kg"; break;
    default:  ustr = ""; break;
    }
    return QString::number(v * qPow(10, -p), 'f', p).append(ustr);
}

static QString vStr(int v, int p, int u)
{
    int worP = u >> 4;
    if (worP == 1 || worP == 2)
        return QString::number(v);
    return QString::number(v * qPow(10, -p), 'f', p);
}

void FormMainRec::updateRec()
{
    if (rqr.err)
        return;

    if (rqr.rc.dcnt == 0)
        ui->rec_num->setText("0");
    else
        ui->rec_num->setText(QString("%1/%2").arg(rqr.did + 1).arg(rqr.rc.dcnt));

    if (rqr.ld.size() > 0) {
        const Rec_Date &r = rqr.ld.at(0);
        ui->val_1->setText(QString::number(r.prm_id));
        ui->val_2->setText(r.prm_name);
        ui->val_3->setText(r.start_time.toString("yyyy-MM-dd hh:mm:ss"));
        ui->val_4->setText(r.end_time.toString("yyyy-MM-dd hh:mm:ss"));
        ui->val_5->setText(vStrU(r.weight1, r.prec, r.unit));
        ui->val_6->setText(vStrU(r.weight2, r.prec, r.unit));
        ui->val_7->setText(vStrU(r.weight3, r.prec, r.unit));
        ui->val_8->setText(QString("%1 ppm").arg(r.speed));
        ui->val_9->setText(QString::number(r.hegeCnt));
        ui->val_10->setText(QString::number(r.drcnt - r.hegeCnt));

        double dval;
        dval = (double)r.hegeCnt / r.drcnt * 100;
        if (qIsNaN(dval) || qIsInf(dval))
            dval = 0;
        ui->val_11->setText(QString("%1 %").arg(dval, 0, 'f', 2));

        if (r.drcnt > 0) {
            int worP = r.unit >> 4;
            if (worP == 1 || worP == 2)
                dval = (double)r.totalPakCnt / RecDF_PakCntF / r.drcnt;
            else
                dval = (double)r.totalWeight / r.drcnt;
            if (qIsNaN(dval) || qIsInf(dval))
                dval = 0;
            ui->val_12->setText(vStrU(qRound(dval) - r.weight1, r.prec, r.unit));
        }
        else
            ui->val_12->setText(vStrU(0, r.prec, r.unit));

        qint64 secsto = r.start_time.secsTo(r.end_time) + 1;
        dval = (double)r.drcnt * 60 / secsto;
        if (qIsNaN(dval) || qIsInf(dval))
            dval = 0;
        ui->val_13->setText(QString::number(dval, 'f', 1).append(" ppm"));
        ui->val_14->setText(vStrU(r.totalWeight, r.prec, r.unit&0x0F));
        ui->val_15->setText(QString::number(r.totalPakCnt / RecDF_PakCntF, 'f', 0).append(" P"));
    }
    else {
        ui->val_1->setText("");
        ui->val_1->setText("");
        ui->val_2->setText("");
        ui->val_3->setText("");
        ui->val_4->setText("");
        ui->val_5->setText("");
        ui->val_6->setText("");
        ui->val_7->setText("");
        ui->val_8->setText("");
        ui->val_9->setText("");
        ui->val_10->setText("");
        ui->val_11->setText("");
        ui->val_12->setText("");
        ui->val_13->setText("");
        ui->val_14->setText("");
        ui->val_15->setText("");
    }

    for (int i = 0; i < PageSizeR && i < rqr.lr.size(); i++) {
        int id = rqr.lr.size() - 1 - i;
        const Rec_Rec &r = rqr.lr.at(id);
        ui->tableWidget->item(i, 0)->setText(QString::number(rqr.rid + id + 1));
        ui->tableWidget->item(i, 1)->setText(vStr(r.weight, r.prec, r.unit));
        ui->tableWidget->item(i, 2)->setText(r.time.toString("yyyy-MM-dd hh:mm:ss"));
        QColor c = r.hege ? Qt::transparent : Qt::red;
        for (int j = 0; j < 3; j++)
            ui->tableWidget->item(i, j)->setBackground(c);
    }
    for (int i = rqr.lr.size(); i < PageSizeR; i++)
        for (int j = 0; j < 3; j++) {
            ui->tableWidget->item(i, j)->setText("");
            ui->tableWidget->item(i, j)->setBackground(Qt::transparent);
        }
}
