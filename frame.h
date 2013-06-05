/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef FRAME_H
#define FRAME_H

#include <QObject>
#include <QColor>

#include "selectable.h"

namespace AnimatorModel {

class Frame : public Selectable {
    Q_OBJECT

public:
    explicit Frame(QObject *parent, Animation &animation, int number);
    Frame(QObject* parent, Animation &animation, QColor colour, int number);
    
    const QColor colour() const;
    void setColour(QColor colour);

    void doSetColour(QColor colour);

signals:
    void colourChanged();

private:
    QColor iColour;
};
}

#endif // FRAME_H
