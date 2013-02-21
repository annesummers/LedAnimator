/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "frame.h"

#include "exceptions.h"

using namespace Exception;
using namespace AnimatorModel;

Frame::Frame(QObject *parent) :
    Selectable(parent),
    iColour(Qt::black){
}

Frame::Frame(QObject* parent, QColor colour) :
    Selectable(parent),
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
