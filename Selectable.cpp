/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "Selectable.h"

using namespace AnimatorModel;

Selectable::Selectable(QObject *parent, Animation& animation, int number) :
    QObject(parent),
    iAnimation(animation),
    iIsSelected(false),
    iNumber(number) {
}

void Selectable::select(bool isSelected) {
    iIsSelected = isSelected;

    emit updated();
}
