#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <QCheckBox>

class CheckBox : public QCheckBox
{
    Q_OBJECT

public:
    explicit CheckBox(QWidget *parent = 0);

    // QAbstractButton interface
protected:
    bool hitButton(const QPoint &pos) const;
};

#endif // CHECKBOX_H
