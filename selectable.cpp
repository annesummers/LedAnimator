/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "selectable.h"

using namespace AnimatorModel;

Selectable::Selectable(QObject *parent) :
    QObject(parent),
    iIsSelected(false) {
}

void Selectable::select(bool isSelected) {
    iIsSelected = isSelected;

    emit selected();
}
