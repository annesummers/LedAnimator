/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "selectable.h"

Selectable::Selectable(QObject *parent) :
    QObject(parent),
    iIsSelected(false) {
}

void Selectable::select(bool isSelected) {
    iIsSelected = isSelected;

    emit selected();
}
