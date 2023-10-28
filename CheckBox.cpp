#include "CheckBox.h"

CheckBox::CheckBox(QWidget *parent) :
    QCheckBox(parent)
{

}

bool CheckBox::hitButton(const QPoint &pos) const
{
    Q_UNUSED(pos);
    return true;
}
