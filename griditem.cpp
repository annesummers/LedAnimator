/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "griditem.h"

using namespace AnimatorModel;

GridItem::GridItem(QObject *parent, Animation& animation, int number, Position position) :
    Selectable(parent, animation, number),
    iPosition(position) {
}
