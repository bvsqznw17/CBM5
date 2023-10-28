#include "FormMenuHSetPBak.h"
#include "ui_FormMenuHSetPBak.h"
#include "MPRWOperation.h"
#include "UDiskOperation.h"
#include <QFileDialog>

FormMenuHSetPBak::FormMenuHSetPBak(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuHSetPBak)
{
    ui->setupUi(this);
}

FormMenuHSetPBak::~FormMenuHSetPBak()
{
    delete ui;
}

void FormMenuHSetPBak::init(FormA *f)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP., form->mpri);
}

void FormMenuHSetPBak::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
}

void FormMenuHSetPBak::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormMenuHSetPBak::refreshUI()
{
}

void FormMenuHSetPBak::onRegUpdated(const MRegAreas &areas)
{
    Q_UNUSED(areas);
}

void FormMenuHSetPBak::on_btn_import_clicked()
{
#ifndef MYOPT_WINDOW
    if (!UDiskOperation::mountAny()) {
        DlgInfo::info(form, MPRWOperation::errStr(-6));
        return;
    }

    QStringList list = QDir("/mnt/usb").entryList(QStringList() << "*.dat");
    UDiskOperation::umountAny();
    DlgList dlgList(form);
    dlgList.setList(list);
    if (dlgList.exec() == QDialog::Rejected)
        return;
    QString filename = dlgList.currents();
#else
    QString filename = QFileDialog::getOpenFileName(form, 0, 0, "(*.dat);;(*)");
#endif
    if (filename.isEmpty())
        return;

    MPRWOperation o(form->mac.maddr(), form->mac.mcomm());
    DlgProgress progress(form, true);
    connect(&o, SIGNAL(progress(int,int)),
            &progress, SLOT(onProgress(int,int)));
    connect(&progress, SIGNAL(cancel()),
            &o, SLOT(cancel()), Qt::DirectConnection);
    QMetaObject::invokeMethod(&o, "Import", Q_ARG(QString, filename));
    progress.exec();
    MAC.read(MMP.Prm);
    MAC.read(MMP.Sys);

    DlgInfo::info(form, o.errStr(progress.result()));
}

void FormMenuHSetPBak::on_btn_export_clicked()
{
#ifndef MYOPT_WINDOW
    DlgOKCancel okc(form);
    okc.setText(tr("确定要导出参数吗？"));
    if (okc.exec() == QDialog::Rejected)
        return;
#else
    QString dir = QFileDialog::getExistingDirectory(form);
    if (dir.isEmpty())
        return;
#endif

    QString filename = QString("cbm5_param_v1_%1.dat")
            .arg(form->dt().toString("yyyyMMdd_hhmmss"));

#ifdef MYOPT_WINDOW
    filename = QString("%1/%2").arg(dir, filename);
#endif

    MPRWOperation o(form->mac.maddr(), form->mac.mcomm());
    DlgProgress progress(form, true);
    connect(&o, SIGNAL(progress(int,int)),
            &progress, SLOT(onProgress(int,int)));
    connect(&progress, SIGNAL(cancel()),
            &o, SLOT(cancel()), Qt::DirectConnection);
    QMetaObject::invokeMethod(&o, "Export", Q_ARG(QString, filename));
    progress.exec();
    MAC.read(MMP.Prm);

    DlgInfo::info(form, o.errStr(progress.result()));
}
