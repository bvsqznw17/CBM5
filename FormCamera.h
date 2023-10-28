#ifndef FORMCAMERA_H
#define FORMCAMERA_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormCamera;
}

class FormCamera : public QWidget
{
    Q_OBJECT

public:
    explicit FormCamera(QWidget *parent = 0);
    ~FormCamera();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);
    void onCameraCalibrated();

private:
    Ui::FormCamera *ui;
    FormA *form;
    MPTRWidget *mptrw;
};

#endif // FORMCAMERA_H
