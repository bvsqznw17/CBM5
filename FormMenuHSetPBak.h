#ifndef FORMMENUHSETPBAK_H
#define FORMMENUHSETPBAK_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuHSetPBak;
}

class FormMenuHSetPBak : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuHSetPBak(QWidget *parent = 0);
    ~FormMenuHSetPBak();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);

    void on_btn_import_clicked();

    void on_btn_export_clicked();

private:
    Ui::FormMenuHSetPBak *ui;
    FormA *form;
    MPTRWidget *mptrw;
};

#endif // FORMMENUHSETPBAK_H
