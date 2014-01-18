/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef GRIDITEM_H
#define GRIDITEM_H

#include "selectable.h"

#include "Position.h"

namespace AnimatorModel {

class GridItem : public Selectable {
    Q_OBJECT

public:
    explicit GridItem(QObject *parent, Animation& animation, int number, Position position);

    inline const Position position() const { return iPosition; }

protected:
    inline void setPosition(Position position) { iPosition = position; }

private:
    Position iPosition;
};
}

#endif // GRIDITEM_H
