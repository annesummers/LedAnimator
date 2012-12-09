#include "led.h"

#include "ledwidget.h"
#include "frame.h"
#include "animation.h"
#include "defaults.h"

Led::Led(Animation& animation, QPoint position) :
    QObject(&animation),
    iAnimation(animation),
    iPosition(position),
    iIsSelected(false) {

    for(int i = 0; i < animation.numFrames(); i++) {
        iFrames.append(new Frame(this));
    }
}

QColor Led::currentColour() {
    return iFrames.at(iAnimation.currentFrame())->colour();
}

void Led::setCurrentColour(QColor colour) {
    setColourAt(iAnimation.currentFrame(), colour);
}

QColor Led::colourAt(int frameNum) {
    return iFrames.at(frameNum)->colour();
}

void Led::setColourAt(int frameNum, QColor colour) {
    iFrames.at(frameNum)->setColour(colour);

    iAnimation.ledColourChanged(*this, frameNum);
}

QPoint Led::position() {
    return iPosition;
}

bool Led::selected() {
    return iIsSelected;
}

void Led::select(bool selected) {
    iIsSelected = selected;
}
