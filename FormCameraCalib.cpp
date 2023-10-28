#include "FormCameraCalib.h"
#include "ui_FormCameraCalib.h"
#include <QButtonGroup>

FormCameraCalib::FormCameraCalib(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCameraCalib)
{
    ui->setupUi(this);
    isOpen = false;
}

FormCameraCalib::~FormCameraCalib()
{
    delete ui;
    if (isOpen) {
        isOpen = false;
        form->cam_t1->close();
        form->cam_t2->close();
    }
}

void FormCameraCalib::init(FormA *f)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP., form->mpri);

    connect(ui->btn_back, SIGNAL(clicked()), form, SLOT(backPage()));

    ui->page_1->cam = form->cam_c1;
    ui->page_2->cam = form->cam_c2;
    ui->page_3->cam.cam1 = form->cam_t1;
    ui->page_3->cam.cam2 = form->cam_t2;
    connect(form, SIGNAL(cameraCalibrated()),
            this, SLOT(onCameraCalibrated()));
    onCameraCalibrated();

    ui->yz_1->setTextValue("128");
    ui->yz_2->setTextValue("128");
    ui->page_1->cam->setThresh(128);
    ui->page_2->cam->setThresh(128);

    QButtonGroup *btg = new QButtonGroup(this);
    btg->addButton(ui->pushButton, 0);
    btg->addButton(ui->pushButton_2, 1);
    btg->addButton(ui->pushButton_3, 2);
    connect(btg, SIGNAL(buttonClicked(int)),
            ui->page, SLOT(setCurrentIndex(int)));
    ui->pushButton->click();
}

void FormCameraCalib::showEvent(QShowEvent *)
{/*
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);*/
    refreshUI();

    if (!isOpen) {
        isOpen = true;
        form->cam_t1->open();
        form->cam_t2->open();
    }
}

void FormCameraCalib::hideEvent(QHideEvent *)
{/*
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));*/

    if (isOpen) {
        isOpen = false;
        form->cam_t1->close();
        form->cam_t2->close();
    }
}

void FormCameraCalib::refreshUI()
{
}

void FormCameraCalib::onRegUpdated(const MRegAreas &areas)
{
    Q_UNUSED(areas);
}

void FormCameraCalib::onCameraCalibrated()
{
    ui->page_3->cam.setPoints(form->cam_pts1, form->cam_pts2,
                              form->cam_ptsc1, form->cam_ptsc2);
}

void FormCameraCalib::on_ezh_1_clicked(bool checked)
{
    ui->page_1->setErZhiHua(checked);
}

void FormCameraCalib::on_ezh_2_clicked(bool checked)
{
    ui->page_2->setErZhiHua(checked);
}

void FormCameraCalib::on_yz_1_editingFinished()
{
    edityz1(ui->yz_1->textv().toInt());
}

void FormCameraCalib::on_yz_2_editingFinished()
{
    edityz2(ui->yz_2->textv().toInt());
}

void FormCameraCalib::on_yzjian_1_clicked()
{
    edityz1(ui->yz_1->textv().toInt() - 1);
}

void FormCameraCalib::on_yzjian_2_clicked()
{
    edityz2(ui->yz_2->textv().toInt() - 1);
}

void FormCameraCalib::on_yzjia_1_clicked()
{
    edityz1(ui->yz_1->textv().toInt() + 1);
}

void FormCameraCalib::on_yzjia_2_clicked()
{
    edityz2(ui->yz_2->textv().toInt() + 1);
}

void FormCameraCalib::on_calib_1_clicked()
{
    QVector<float> pts = ui->page_1->cam->getPoints();
    if (pts.size() != 8)
        return;
    form->cam_pts1 = pts;
    form->cameraCalibrated();
    CameraCalib::savePoints(form->cam_pts1, form->cam_pts2);
    DlgInfo::info(form, tr("标定完成"));
}

void FormCameraCalib::on_calib_2_clicked()
{
    QVector<float> pts = ui->page_2->cam->getPoints();
    if (pts.size() != 8)
        return;
    form->cam_pts2 = pts;
    form->cameraCalibrated();
    CameraCalib::savePoints(form->cam_pts1, form->cam_pts2);
    DlgInfo::info(form, tr("标定完成"));
}

void FormCameraCalib::edityz1(int v)
{
    if (v > 255) v = 255;
    else if (v < 0) v = 0;
    ui->yz_1->setTextValue(QString::number(v));
    ui->page_1->cam->setThresh(v);
}

void FormCameraCalib::edityz2(int v)
{
    if (v > 255) v = 255;
    else if (v < 0) v = 0;
    ui->yz_2->setTextValue(QString::number(v));
    ui->page_2->cam->setThresh(v);
}
