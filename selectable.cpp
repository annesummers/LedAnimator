/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "selectable.h"

using namespace AnimatorModel;

Selectable::Selectable(QObject *parent, int number) :
    QObject(parent),
    iIsSelected(false),
    iNumber(number) {
}

void Selectable::select(bool isSelected) {
    iIsSelected = isSelected;

    emit selected();
}
