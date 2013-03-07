/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "frame.h"

#include "exceptions.h"

using namespace Exception;
using namespace AnimatorModel;

Frame::Frame(QObject *parent, int number) :
    Selectable(parent, number),
    iColour(Qt::black) {
}

Frame::Frame(QObject* parent, QColor colour, int number) :
    Selectable(parent, number),
    iColour(colour) {}

const QColor Frame::colour() const {
    return iColour;
}

void Frame::setColour(QColor colour) {
    if(!colour.isValid()) {
        throw IllegalColourException();
    }

    iColour = colour;

    emit colourChanged();
}
