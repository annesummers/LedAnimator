#include "grouplistwidget.h"

GroupListWidget::GroupListWidget(QWidget *parent, Animation& animation) :
    QWidget(parent),
    iAnimation(animation)
{
    iComboBox = new QComboBox(this);

    connect(iComboBox, SIGNAL(activated(int)), this, SLOT(comboChanged(int)));
    connect(&iAnimation, SLOT(groupAdded(int)), this, SLOT(addGroup(int)));
}

void GroupListWidget::addGroup(int groupNumber) {
    iComboBox->addItem(QString("%1").arg(groupNumber));
}

void GroupListWidget::removeGroup(int groupNumber) {
    Q_UNUSED(groupNumber);
}

void GroupListWidget::comboChanged(int index) {
    iAnimation.selectGroup(iComboBox->itemText(index).toInt());
}
