#include "FormMenuHSetMainboard.h"
#include "ui_FormMenuHSetMainboard.h"

FormMenuHSetMainboard::FormMenuHSetMainboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuHSetMainboard)
{
    ui->setupUi(this);
}

FormMenuHSetMainboard::~FormMenuHSetMainboard()
{
    delete ui;
    form->mac.echom.destroy(ecIds, this);
}

void FormMenuHSetMainboard::init(FormA *f)
{
    form = f;
    //mptr = new MPTRWidget(this, &form->mac, MMP.Mod, form->mpri);

    form->mac.echom.create(echo, ecIds, this, "onReadFns");

    int i = 0;
#define MyReplace(N,I) \
    ver[i] = ui->N##_ver_##I; state[i] = ui->N##_state_##I; i++;

    MyReplace(zzb, 1);
    MyReplace(flb, 1); MyReplace(flb, 2); MyReplace(flb, 3); MyReplace(flb, 4); MyReplace(flb, 5);
    MyReplace(flb, 6); MyReplace(flb, 7); MyReplace(flb, 8); MyReplace(flb, 9); MyReplace(flb, 10);
    MyReplace(flb, 11); MyReplace(flb, 12); MyReplace(flb, 13); MyReplace(flb, 14); MyReplace(flb, 15);
    MyReplace(flb, 16); MyReplace(flb, 17); MyReplace(flb, 18); MyReplace(flb, 19); MyReplace(flb, 20);
    MyReplace(flb, 21); MyReplace(flb, 22); MyReplace(flb, 23); MyReplace(flb, 24); MyReplace(flb, 25);
    MyReplace(flb, 26); MyReplace(flb, 27); MyReplace(flb, 28); MyReplace(flb, 29); MyReplace(flb, 30);
    MyReplace(flbj, 1); MyReplace(flbj, 2); MyReplace(flbj, 3); MyReplace(flbj, 4); MyReplace(flbj, 5);
    MyReplace(zjb, 1); MyReplace(zjb, 2); MyReplace(zjb, 3); MyReplace(zjb, 4); MyReplace(zjb, 5);
    MyReplace(zdb, 1); MyReplace(zdb, 2); MyReplace(zdb, 3); MyReplace(zdb, 4);
    MyReplace(kzb, 1);
#undef MyReplace
}

void FormMenuHSetMainboard::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
    ui->stackedWidget->setCurrentIndex(0);

    QTimer::singleShot(0, this, SLOT(onWriteCmd()));
}

void FormMenuHSetMainboard::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormMenuHSetMainboard::refreshUI()
{
    for (int i = 0; i < num; i++) {
        ver[i]->setText("");
        state[i]->setText("");
    }
}

void FormMenuHSetMainboard::onRegUpdated(const MRegAreas &areas)
{
    Q_UNUSED(areas);
}

void FormMenuHSetMainboard::onWriteCmd()
{
    connect(form, SIGNAL(cmdFns(int)), this, SLOT(onCmdFns(int)));
    form->writeCMD(CTL_READ_MKSTATUS, 0);
    form->waitCmdCnt = 4;
    if (form->waitCmdFns(12000))
        onCmdFns(0);
    disconnect(form, SIGNAL(cmdFns(int)), this, SLOT(onCmdFns(int)));
}

void FormMenuHSetMainboard::onCmdFns(int cnt)
{
    int id;
    switch (cnt) {
    case 4: id = 1; break;
    case 3: id = 2; break;
    case 2: id = 3; break;
    case 1: id = 4; break;
    default: id = 0; break;
    }
    echo.val = cnt;
    MAC.read(MMP.Mod, echo);
    if (id > 0)
        form->writeCMD(CTL_READ_MKSTATUS, id);
}

void FormMenuHSetMainboard::onReadFns(int err, const SignalEcho_zy1 &echo)
{
    form->mac.echom.finish(echo);
    if (err)
        return;

    int cnt = echo.val.toInt();
    int id, maxn;
    switch (cnt) {
    case 4: id = 0; maxn = 1; break;
    case 3: id = 1; maxn = 30; break;
    case 2: id = 36; maxn = 5; break;
    case 1: id = 41; maxn = 4; break;
    case 0: id = 45; maxn = 1; break;
    default: return;
    }

    int n = MP.mod_totaln->value().toInt();
    if (id == 1) {
        for (int i = form->dn, k = id+maxn; i < n && i < 5+form->dn; i++, k++) {
            ver[k]->setText(MP.mod_ver[i]->vStr());
            state[k]->setText(QString("%1").arg(MP.mod_status0[i]->vStr()));
        }
        if (n >= form->dn)
            n = form->dn;
    }
    for (int i = 0; i < n && i < maxn; i++) {
        ver[i+id]->setText(MP.mod_ver[i]->vStr());
        state[i+id]->setText(QString("%1").arg(MP.mod_status0[i]->vStr()));
    }
}

void FormMenuHSetMainboard::on_pushButton_clicked()
{
    if (ui->stackedWidget->currentIndex() == 0)
        ui->stackedWidget->setCurrentIndex(1);
    else
        ui->stackedWidget->setCurrentIndex(0);
}
