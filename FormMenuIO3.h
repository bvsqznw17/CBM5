#ifndef FORMMENUIO3_H
#define FORMMENUIO3_H

#include <QWidget>
#include "FormA.h"

namespace Ui {
class FormMenuIO3;
}

class FormMenuIO3 : public QWidget
{
    Q_OBJECT

public:
    explicit FormMenuIO3(QWidget *parent = 0);
    ~FormMenuIO3();
    void init(FormA *f);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void refreshUI();
    void onRegUpdated(const MRegAreas &areas);

private:
    Ui::FormMenuIO3 *ui;
    FormA *form;
    MPTRWidget *mptrw;
};

#endif // FORMMENUIO3_H
