/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "frame.h"

#include "exceptions.h"

#include "ledanimatoractionbase.h"

using namespace Exception;
using namespace AnimatorModel;

Frame::Frame(QObject *parent, Animation& animation, int number) :
    Selectable(parent, animation, number),
    iColour(Qt::black) {
}

Frame::Frame(QObject* parent, Animation& animation, QColor colour, int number) :
    Selectable(parent, animation, number),
    iColour(colour) {}

const QColor Frame::colour() const {
    return iColour;
}

void Frame::setColour(QColor colour) {
    if(!colour.isValid()) {
        throw IllegalColourException();
    }

    iAnimation.setFrameColour(*this, iColour, colour);
}

void Frame::doSetColour(QColor colour) {
    iColour = colour;

    emit colourChanged();
}
