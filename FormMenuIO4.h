#ifndef FORMMENUIO4_H
#define FORMMENUIO4_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuIO4;
}

class FormMenuIO4 : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuIO4(QWidget *parent = 0);
    ~FormMenuIO4();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);

private:
    Ui::FormMenuIO4 *ui;
    FormA *form;
    MPTRWidget *mptrw;
};

#endif // FORMMENUIO4_H
