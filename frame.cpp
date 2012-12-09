#include "frame.h"

Frame::Frame(QObject *parent) :
    QObject(parent) {
}

Frame::Frame(QObject* parent, QColor colour) :
    QObject(parent),
    iColour(colour) {}

QColor Frame::colour() {
    return iColour;
}

void Frame::setColour(QColor colour) {
    iColour = colour;
}
