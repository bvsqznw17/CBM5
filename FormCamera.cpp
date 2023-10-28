#include "FormCamera.h"
#include "ui_FormCamera.h"

FormCamera::FormCamera(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCamera)
{
    ui->setupUi(this);
}

FormCamera::~FormCamera()
{
    delete ui;
}

void FormCamera::init(FormA *f)
{
    form = f;
    //mptrw = new MPTRWidget(this, &form->mac, MMP., form->mpri);

    ui->camera->cam.cam1 = form->cam_t1;
    ui->camera->cam.cam2 = form->cam_t2;
    connect(form, SIGNAL(cameraCalibrated()),
            this, SLOT(onCameraCalibrated()));
    onCameraCalibrated();
}

void FormCamera::showEvent(QShowEvent *)
{/*
    connect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
            this, SLOT(onRegUpdated(MRegAreas)),
            Qt::UniqueConnection);
    refreshUI();*/
}

void FormCamera::hideEvent(QHideEvent *)
{/*
    disconnect(&form->mac, SIGNAL(regUpdated(MRegAreas)),
               this, SLOT(onRegUpdated(MRegAreas)));*/
}

void FormCamera::refreshUI()
{
}

void FormCamera::onRegUpdated(const MRegAreas &areas)
{
    Q_UNUSED(areas);
}

void FormCamera::onCameraCalibrated()
{
    ui->camera->cam.setPoints(form->cam_pts1, form->cam_pts2,
                              form->cam_ptsc1, form->cam_ptsc2);
}
