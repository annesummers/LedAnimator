#include "ColourValue.h"

using namespace AnimatorModel;

ColourValue::ColourValue(QObject *parent, QColor colour) :
    FrameValue(parent),
    iColour(colour)
{
}

void ColourValue::setColour(QColor colour) {
    iColour = colour;
}

FrameValue& ColourValue::operator=(const FrameValue& copyValue) {
    iColour = static_cast<const ColourValue&>(copyValue).colour();

    return *this;
}
