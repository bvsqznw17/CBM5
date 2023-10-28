#include "FormTestDouKaiMen.h"
#include "ui_FormTestDouKaiMen.h"

FormTestDouKaiMen::FormTestDouKaiMen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormTestDouKaiMen)
{
    ui->setupUi(this);
    ui->val_1->setTextValue("0");

    btg = new QButtonGroup(this);
    int id = 0;
    btg->addButton(ui->pushButton_1, id++);
    btg->addButton(ui->pushButton_2, id++);
}

FormTestDouKaiMen::~FormTestDouKaiMen()
{
    delete ui;
}

void FormTestDouKaiMen::init(FormA *f)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP., form->mpri);

    connect(ui->btn_back, SIGNAL(clicked()), form, SLOT(backPage()));

    form->keybm->startadd();
    form->keybm->add(KEYBMET(ui->val_1), KbInputInfo("0 ~ 360"));
    form->keybm->endadd();
}

void FormTestDouKaiMen::showEvent(QShowEvent *)
{
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();
}

void FormTestDouKaiMen::hideEvent(QHideEvent *)
{
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));
}

void FormTestDouKaiMen::refreshUI()
{
    ui->dounum->setText(QString::number(dounum));
}

void FormTestDouKaiMen::onRegUpdated(const MRegAreas &areas)
{
    Q_UNUSED(areas);
}

void FormTestDouKaiMen::on_btn_ag_jian_clicked()
{
    int ag = ui->val_1->textv().toInt();
    if (ag > 0) {
        ui->val_1->setTextValue(QString::number(ag-1));
    }
}

void FormTestDouKaiMen::on_btn_ag_jia_clicked()
{
    int ag = ui->val_1->textv().toInt();
    if (ag < 360) {
        ui->val_1->setTextValue(QString::number(ag+1));
    }
}

void FormTestDouKaiMen::on_btn_zero_clicked()
{
    ui->val_1->setTextValue("0");
    on_btn_test_clicked();
}

void FormTestDouKaiMen::on_btn_test_clicked()
{
    int fun = btg->checkedId();
    int ag = ui->val_1->textv().toInt();
    form->writeCMD((fun<<6)|dounum, CTL_TEST_DOUKAIMEN, (ag>>8)&0xFF, (ag>>0)&0xFF);
}
