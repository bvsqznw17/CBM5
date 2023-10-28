#include "FormMenuHSetMM1.h"
#include "ui_FormMenuHSetMM1.h"
#include "DialogDebugMM.h"
#include <QButtonGroup>

#define m_datai_ag 0 // 角度
#define m_datai_sm 1 // 最大速度
#define m_datai_s0 2 // 起始速度
#define m_datai_s1 3 // 停止速度
#define m_datai_sa 4 // 加速度
#define m_datai_sb 5 // 减速度
#define m_datan 6
#define m_datan1 7

FormMenuHSetMM1::FormMenuHSetMM1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMenuHSetMM1)
{
    ui->setupUi(this);
    ui->val_1->hide();
    ui->page->setCurrentIndex(0);

    btg = new QButtonGroup(this);
    btg->addButton(ui->pushButton, 0);
    btg->addButton(ui->pushButton_2, 1);
    btg->addButton(ui->pushButton_3, 2);
    btg->addButton(ui->pushButton_4, 3);
    btg->addButton(ui->pushButton_5, 4);
    btg->setExclusive(false);
    connect(btg, SIGNAL(buttonClicked(int)), this, SLOT(onModeClicked(int)));
}

FormMenuHSetMM1::~FormMenuHSetMM1()
{
    delete ui;
}

void FormMenuHSetMM1::init(FormA *f, E_MotorMode mm)
{
    form = f;
    mptrw = new MPTRWidget(this, &form->mac, MMP.Motor, form->mpri);
    mptrw->start_later = true;

    edits[0] = ui->val_1; mps[0] = MP.motor_pos;
    edits[1] = ui->val_2; mps[1] = MP.motor_speed;
    edits[2] = ui->val_3; mps[2] = MP.motor_start;
    edits[3] = ui->val_4; mps[3] = MP.motor_end;
    edits[4] = ui->val_5; mps[4] = MP.motor_acc;
    edits[5] = ui->val_6; mps[5] = MP.motor_dec;
    edits[6] = ui->val_7; mps[6] = MP.motor_stoptime;

    motorm = mm;
    motormm = 0;
    motormi = 0;
    starti = form->keybm->startadd();
    for (int i = 0; i < m_datan1; i++)
        form->keybm->add(KEYBMET(((QImageLineEdit*)edits[i])),
                         mps[i][0], KEYBM_MPIIOnly);
    form->keybm->add(KEYBMET(ui->val_curi), KbInputInfo("1 ~ 10"));
    form->keybm->add(KEYBMET(ui->val_gdtzjd), MP.motor_gdtzjd);
    form->keybm->add(KEYBMET(ui->val_wgdtzjd), MP.motor_wgdtzjd);
    endi = form->keybm->endadd();
    ui->btn_ydjc->setEnabled(mm != MM_MAIN_VIB);

    for (int i = 0; i < m_datan1; i++)
        connect(edits[i], SIGNAL(editingFinished()), this, SLOT(onEditFinished()));
}

void FormMenuHSetMM1::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    form->writeCMD(0x00,CTL_MOTOR_SELECT,0x00,motorm+motormm);
}

void FormMenuHSetMM1::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormMenuHSetMM1::refreshUI()
{
    if (!isCorrectMode())
        return;

    int mid = MP.motor_id->value().toInt() & 0x00FF;
    for (int i = 0; i < 5; i++)
        btg->button(i)->setChecked(i == mid);
    bool mid4 = mid != 4;
    for (int i = 0; i < m_datan1; i++)
        edits[i]->setEnabled(mid4);
    ui->btn_add_1->setEnabled(mid4);
    ui->btn_add_2->setEnabled(mid4);
    ui->btn_del->setEnabled(mid4);
    ui->val_gdtzjd->setEnabled(mid4);
    ui->val_wgdtzjd->setEnabled(mid4);

    QVector<int> v = getMMData();
    if (!v.isEmpty()) {
        int mi = motormi*m_datan;
        if ((uint)mi >= (uint)v.size())
            mi = v.size() - m_datan;
        // ???keybm->setText有延迟，已解决 QTimer::singleShot(0, this, SLOT(refreshUI()));
        for (int i = 0; i < m_datan1; i++)
            form->keybm->setText(starti+i, mps[i][motormi]->vStrU());
        ui->val_1->setVisible(v.at(mi+1) != 0);
        ui->val_7->setVisible(v.at(mi+1) == 0);
        form->keybm->setText(form->keybm->indexOf(ui->val_curi),
                             QString::number(motormi+1));
    }
    else {
        for (int i = 0; i < m_datan1; i++)
            form->keybm->setText(starti+i, "");
        ui->val_1->setVisible(true);
        ui->val_7->setVisible(false);
        form->keybm->setText(form->keybm->indexOf(ui->val_curi), "");
    }
    form->keybm->refreshMParam(endi-2, endi);

    ui->gm->setData(v.constData(), v.size());
}

void FormMenuHSetMM1::onRegUpdated(const MRegAreas &areas)
{
    if (areas.isCon(MMP.Motor))
        QTimer::singleShot(0, this, SLOT(refreshUI()));
}

void FormMenuHSetMM1::onModeClicked(int i)
{
    motormm = i;
    motormi = 0;
    form->writeCMD(0x00,CTL_MOTOR_SELECT,0x00,motorm+i);
    mptrw->readOnce();
}

void FormMenuHSetMM1::onEditFinished()
{
    if (!isCorrectMode())
        return;

    QVector<int> v = getMMData();
    int mi = motormi*m_datan;
    if ((uint)mi >= (uint)v.size())
        return;

    int i = form->keybm->indexOf(sender());
    if (form->keybm->isError(i))
        return;
    MAC.write(mps[i-starti][motormi], form->keybm->edit(i)->text(),
              form->keybm->getEcho(i));
    mptrw->readOnce();
}

void FormMenuHSetMM1::on_btn_add_1_clicked()
{
    if (!isCorrectMode())
        return;

    QVector<int> v = getMMData();
    if (v.size() >= 10*m_datan)
        return;

    int mi = motormi*m_datan;
    if ((uint)mi >= (uint)v.size() && !v.isEmpty())
        return;
    int s = v.size();
    v.resize(v.size() + m_datan);
    int *d = v.data();
    for (int i = s-1; i >= mi; i--)
        d[i+m_datan] = d[i];

    for (int i = 0; i < m_datan; i++)
        d[i+mi] = 60;
    d[m_datai_ag+mi] = 600;
    adjustMMData(mi, d, v.size());

    setMMData(v);
}

void FormMenuHSetMM1::on_btn_add_2_clicked()
{
    if (!isCorrectMode())
        return;

    QVector<int> v = getMMData();
    if (v.size() >= 10*m_datan)
        return;

    int mi = motormi*m_datan;
    if ((uint)mi >= (uint)v.size() && !v.isEmpty())
        return;
    if (v.size() > 0)
        mi += m_datan;
    int s = v.size();
    v.resize(v.size() + m_datan);
    int *d = v.data();
    for (int i = s-1; i >= mi; i--)
        d[i+m_datan] = d[i];

    for (int i = 0; i < m_datan; i++)
        d[i+mi] = 60;
    d[m_datai_ag+mi] = 600;
    adjustMMData(mi, d, v.size());

    if (v.size() > m_datan)
        motormi++;
    setMMData(v);
}

void FormMenuHSetMM1::on_btn_del_clicked()
{
    if (!isCorrectMode())
        return;

    QVector<int> v = getMMData();
    int mi = motormi*m_datan;
    if ((uint)mi >= (uint)v.size())
        return;

    v.remove(mi, m_datan);
    if ((uint)mi >= (uint)v.size() && motormi > 0)
        motormi--;
    setMMData(v);
}

void FormMenuHSetMM1::on_val_curi_editingFinished()
{
    int i = ui->val_curi->textv().toInt() - 1;
    QVector<int> v = getMMData();
    int mi = i*m_datan;
    if ((uint)mi >= (uint)v.size()) {
        ui->val_curi->setTextValue("");
        return;
    }

    motormi = i;
    refreshUI();
}

void FormMenuHSetMM1::on_gm_clicked(int i)
{
    QVector<int> v = getMMData();
    if ((uint)i >= (uint)v.size())
        return;

    motormi = i/m_datan;
    refreshUI();
    ui->page->setCurrentIndex(0);
}

void FormMenuHSetMM1::on_btn_copy_clicked()
{
    int id = btg->checkedId();
    if (id < 0)
        return;

    DlgList dlg(form);
    dlg.setWindowTitle(ui->btn_copy->text());
    QStringList list;
    QVariantList vlist;
    for (int i = 0; i < 4; i++) {
        if (i != id) {
            list.append(btg->button(i)->text());
            vlist.append(i);
        }
    }
    dlg.setList(list);
    dlg.setVList(vlist);
    if (dlg.exec() == QDialog::Accepted) {
        int dest = dlg.currentv().toInt();
        form->writeCMD(0x00,CTL_MOTOR_COPY,motorm+dest,motorm+id);
    }
}

void FormMenuHSetMM1::on_btn_test_clicked()
{
    int id = btg->checkedId();
    if (id < 0)
        return;

    DialogDebugMM dlg(form);
    dlg.init(form, motorm+id);
    dlg.exec();
}

QVector<int> FormMenuHSetMM1::getMMData()
{
    QVector<int> v;
    for (int k = 0; k < 10; k++) {
        int d[m_datan];
        for (int i = 0; i < m_datan; i++)
            d[i] = mps[i][k]->valuenp().toInt();

        if (d[m_datai_ag] == 0)
            break;
        for (int i = 0; i < m_datan; i++)
            v.append(d[i]);
    }
    return v;
}

void FormMenuHSetMM1::setMMData(const QVector<int> &v)
{
    QList<const MParam *> mlist;
    QVariantList vlist;
    mlist.reserve(v.size()+m_datan);
    vlist.reserve(v.size()+m_datan);
    for (int k = 0; k < 10; k++) {
        for (int i = 0; i < m_datan; i++) {
            if (k*m_datan+i >= v.size()) {
                k = 10;
                break;
            }
            mlist.append(mps[i][k]);
            vlist.append(mps[i][k]->vP(v.at(k*m_datan+i)));
        }
    }
    if (v.size()/m_datan < 10) {
        for (int i = 0; i < m_datan; i++) {
            mlist.append(mps[i][v.size()/m_datan]);
            vlist.append(0);
        }
    }

    MAC.write(mlist, vlist);
    mptrw->readOnce();
}

void FormMenuHSetMM1::adjustMMData(int mi, int *d, int size)
{
    for (int i = (mi-m_datan+size)%size; i != mi; i = (i-m_datan+size)%size)
        if (d[m_datai_sm+i] != 0) {
            d[m_datai_s0+mi] = d[m_datai_s1+i];
            break;
        }
    for (int i = (mi+m_datan)%size; i != mi; i = (i+m_datan)%size)
        if (d[m_datai_sm+i] != 0) {
            d[m_datai_s1+mi] = d[m_datai_s0+i];
            break;
        }
}

void FormMenuHSetMM1::on_btn_ydjc_clicked()
{
    ui->page->setCurrentIndex(1);
}

void FormMenuHSetMM1::on_btn_back_clicked()
{
    ui->page->setCurrentIndex(0);
}
