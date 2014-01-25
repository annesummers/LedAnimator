#include "AxisData.h"

#include "Led.h"
#include "Animation.h"
#include "FunctionValue.h"

using namespace AnimatorModel;

AxisData::AxisData(QObject *parent,
                   Animation &animation,
                   Axis& axis,
                   Led &led,
                   QUndoStack *undoStack) :
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

void AxisData::calculateRanges() {
      setRange(iAxis.lowValue(),
               iAxis.zeroValue(),
               iAxis.zeroValue(),
               frameAt(iAxis.zeroValue() - 1).function());

      setRange(iAxis.zeroValue(),
               iAxis.highValue(),
               iAxis.zeroValue(),
               frameAt(iAxis.zeroValue() + 1).function());
}

void AxisData::setRange(int first, int last, int anchor, Function function) {
    setFirstInRange(first);
    setAnchorInRange(anchor);
    setLastInRange(last, function);
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

