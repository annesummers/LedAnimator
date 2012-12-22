/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "frame.h"

Frame::Frame(QObject *parent) :
    QObject(parent) {
}

Frame::Frame(QObject* parent, QColor colour) :
    QObject(parent),
    iColour(colour) {}

const QColor Frame::colour() const {
    return iColour;
}

void Frame::setColour(QColor colour) {
    iColour = colour;

    emit colourChanged();
}
