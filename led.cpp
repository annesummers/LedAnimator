#include "led.h"

#include "ledwidgets.h"
#include "frame.h"
#include "animation.h"
#include "defaults.h"

Led::Led(Animation& animation, int row, int column) :
    QObject(&animation),
    iAnimation(animation),
    iRow(row),
    iColumn(column),
    iIsSelected(false) {

    for(int i = 0; i < animation.numFrames(); i++) {
        Frame* frame = new Frame(this);
        iFrames.append(frame);
    }
}

QColor Led::currentColour() {
    return colourAt(iAnimation.currentFrame());
}

void Led::setCurrentColour(QColor colour) {
    setColourAt(iAnimation.currentFrame(), colour);
}

QColor Led::colourAt(int frameNum) {
    return iFrames.at(frameNum - 1)->colour(); // frames are indexed from 1!
}

void Led::setColourAt(int frameNum, QColor colour) {
    iFrames.at(frameNum - 1)->setColour(colour);  // frames are indexed from 1!

    if(frameNum == iAnimation.currentFrame()) {
        emit currentColourChanged();
    }
}

int Led::row() {
    return iRow;
}

int Led::column() {
    return iColumn;
}

Frame& Led::frameAt(int frameNum) {
    return *(iFrames.at(frameNum));
}

bool Led::isSelected() {
    return iIsSelected;
}

void Led::select(bool isSelected) {
    iIsSelected = isSelected;

    emit selected();
}
