#ifndef FORMMENUIO_H
#define FORMMENUIO_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuIO;
}

class FormMenuIO : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuIO(QWidget *parent = 0);
    ~FormMenuIO();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);

private:
    Ui::FormMenuIO *ui;
    FormA *form;
    MPTRWidget *mptrw;
};

#endif // FORMMENUIO_H
