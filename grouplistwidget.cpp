/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "grouplistwidget.h"

GroupListWidget::GroupListWidget(QWidget *parent, Animation& animation) :
    QWidget(parent),
    iAnimation(animation) {

    //iComboBox = new QComboBox(this);

   // connect(iComboBox, SIGNAL(activated(int)), this, SLOT(comboChanged(int)));
  //  connect(&iAnimation, SLOT(groupAdded(int)), this, SLOT(addGroup(int)));
}

void GroupListWidget::addGroup(int groupNumber) {
   // iComboBox->addItem(QString("%1").arg(groupNumber));
    Q_UNUSED(groupNumber);
}

void GroupListWidget::removeGroup(int groupNumber) {
    Q_UNUSED(groupNumber);
}

void GroupListWidget::comboChanged(int index) {
    Q_UNUSED(index);
    //iAnimation.selectGroup(iComboBox->itemText(index).toInt());
}
