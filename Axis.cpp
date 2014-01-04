#include "Axis.h"
#include "exceptions.h"
#include "Led.h"
#include "Animation.h"
#include "FunctionValue.h"

using namespace AnimatorModel;
using namespace Exception;

Axis::Axis(QObject *parent,
           Animation &animation,
           int lowValue,
           int highValue,
           int zeroValue,
           int priority,
           bool isOpaque) :
    QObject(parent),
    iLowValue(lowValue),
    iHighValue(highValue),
    iZeroValue(zeroValue),
    iAnimation(animation),
    iCurrentFrame(0),
    iOpaque(isOpaque),
    iPriority(priority) {

    emit lowValueChanged(iLowValue);
    emit highValueChanged(iHighValue);
}

const int Axis::numFrames() const {
    return iHighValue - iLowValue + 1;
}

void Axis::setCurrentFrame(int frame) {
    if(frame < iLowValue) {
        throw IllegalArgumentException("Axis::setCurrentFrame : Frame number is smaller than first frame");
    }

    if(frame > iHighValue) {
        throw IllegalArgumentException("Axis::setCurrentFrame : Frame number is greater than last frame");
    }

    iCurrentFrame = frame;
    emit currentFrameChanged(iCurrentFrame);
}

/*const Frame& Axis::currentFrame() const {
    return frameAt(iCurrentFrame);
}*/

AxisData::AxisData(QObject *parent,
                   Animation &animation,
                   Axis& axis,
                   Led &led,
                   QUndoStack &undoStack) :
    QObject(parent),
    iAxis(axis),
    iAnimation(animation),
    iUndoStack(undoStack),
    iSignalMapper(NULL),
    iNewAnchor(-1),
    iNewFirst(-1),
    iHasNewAnchor(false){

    iSignalMapper = new QSignalMapper(this);

    Frame* newFrame = new Frame(this, *this, animation, axis.lowValue(), NULL, undoStack);
    Frame* previousFrame = NULL;
    for(int i = axis.lowValue() + 1; i <= axis.highValue(); i++) {
        iFrames.insert(newFrame->number(), newFrame);

        iSignalMapper->setMapping(newFrame, newFrame->number());
        connect(newFrame, SIGNAL(valueChanged()), iSignalMapper, SLOT(map()));

        previousFrame = newFrame;
        newFrame = new Frame(this, *this, animation, i, previousFrame, undoStack);
        previousFrame->setNext(*newFrame);
    }

    iFrames.insert(newFrame->number(), newFrame);

    connect(iSignalMapper, SIGNAL(mapped(int)), &led, SLOT(colourChanged(int)));
}

void AxisData::setAnchorInRange(int number) {
    iNewAnchor = number;
    iHasNewAnchor = true;
}

void AxisData::setLastInRange(int number, Function function) {
    //if(iNewAnchor == -1) {
    //    throw InvalidFrameException("No anchor value set");
    //}

    if(iFunctionRanges.size() > 0) {

        for(int i = 0; i < iFunctionRanges.size(); i++) {
            Range range = iFunctionRanges.at(i);
            if(range.lowValue() == iNewFirst &&
                    range.highValue() == number) {
                iFunctionRanges.removeAt(i);
                range.setFunction(iAnimation.addFunction(function));
                iFunctionRanges.append(range);

                //QMap::iterator iter = iFrames.begin();
                for(int i = iNewFirst; i <= number; i++) {
                    if(i != range.anchor()) {
                        iFrames.find(i).value()->setValue(*(new FunctionValue(this, function, range.function())));
                    }
                }

                iHasNewAnchor = false;
                return;
            }

            if(range.lowValue() < iNewFirst &&
               range.highValue() <= number) {
                iFunctionRanges.removeAt(i);
                iFunctionRanges.append(Range(range.anchor(),
                                             range.lowValue(),
                                             iNewFirst - 1,
                                            range.function()));

                iFunctionRanges.append(Range(iHasNewAnchor?iNewAnchor:range.anchor(),
                                             iNewFirst,
                                             number,
                                            iAnimation.addFunction(function)));

                for(int i = iNewFirst; i <= number; i++) {
                    if(iHasNewAnchor && i != iNewAnchor) {
                        iFrames.find(i).value()->setValue(*(new FunctionValue(this, function, range.function())));
                    }
                }
                iHasNewAnchor = false;
                return;
            }

            if(range.lowValue() >= iNewFirst &&
               range.highValue() > number) {
                iFunctionRanges.removeAt(i);
                iFunctionRanges.append(Range(range.anchor(),
                                             iNewFirst + 1,
                                             range.highValue(),
                                            range.function()));

                iFunctionRanges.append(Range(iHasNewAnchor?iNewAnchor:range.anchor(),
                                             iNewFirst,
                                             number,
                                            iAnimation.addFunction(function)));

                for(int i = iNewFirst; i <= number; i++) {
                    if(iHasNewAnchor && i != iNewAnchor) {
                        iFrames.find(i).value()->setValue(*(new FunctionValue(this, function, range.function())));
                    }
                }
                iHasNewAnchor = false;
                return;
            }
        }
    }
}

void AxisData::setFirstInRange(int number) {
    iNewFirst = number;
}

void AxisData::copyFrames(const AxisData &copyAxis) {
    for(int i = iAxis.lowValue(); i < iAxis.highValue(); i++) {
        frameAt(i).doSetValue(copyAxis.frameAt(i).value());
    }
}

