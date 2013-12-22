/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "Frame.h"

#include "exceptions.h"

#include "LedAnimatorActionBase.h"
#include "Animation.h"
#include "ColourValue.h"
#include "FunctionValue.h"
#include "NoValue.h"

using namespace AnimatorModel;
using namespace UndoCommands;

Frame::Frame(QObject *parent,
             Animation& animation,
             int number,
             FrameValue& value,
             Frame *previous,
             QUndoStack &undoStack) :
    Selectable(parent, animation, number),
    iUndoStack(undoStack),
    iValue(&value),
    iPrevious(previous) {
}

Frame::Frame(QObject *parent,
             Animation &animation,
             int number,
             Frame *previous,
             QUndoStack& undoStack) :
    Selectable(parent, animation, number),
    iUndoStack(undoStack),
    iValue(NULL),//*new NoValue(parent)),
    iPrevious(previous) {
}

void Frame::setValue(const FrameValue& value) {
    iUndoStack.push(new SetFrameColourCommand(iAnimation, *this, value));
    //doSetValue(value);
}

const QColor Frame::colour() const {
    switch(value().type()) {
    case kNoValue:
        return QColor();
    case kColour:
        return static_cast<const ColourValue&>(value()).colour();
    case kFunction: {
        if(previous() != NULL) {
            const FunctionValue& functionValue = static_cast<const FunctionValue&>(value());
            const Frame* previousFrame = previous();
            int counter = 1;

            while(previousFrame->value().type() == kFunction) {
                previousFrame = previousFrame->previous();
                counter++;
            }

            const ColourValue& colourValue = static_cast<const ColourValue&>(previousFrame->value());
            float currentRedValue = colourValue.colour().redF();
            float currentBlueValue = colourValue.colour().blueF();
            float currentGreenValue = colourValue.colour().greenF();

            for(int i = 0; i < counter; i++) {
                currentRedValue -= functionValue.function().redIncrement();
                currentBlueValue -= functionValue.function().greenIncrement();
                currentGreenValue -= functionValue.function().blueIncrement();

                //qDebug("red : %f green : %f blue %f", iCurrentRedValue, iCurrentGreenValue, iCurrentBlueValue);

                if(currentRedValue > 1) {
                    currentRedValue = 1;
                }

                if(currentBlueValue > 1) {
                    currentBlueValue = 1;
                }

                if(currentGreenValue > 1) {
                    currentGreenValue = 1;
                }

                if(currentRedValue < 0) {
                    currentRedValue = 0;
                }

                if(currentBlueValue < 0) {
                    currentBlueValue = 0;
                }

                if(currentGreenValue < 0) {
                    currentGreenValue = 0;
                }
            }

            return QColor::fromRgbF(currentRedValue, currentGreenValue, currentBlueValue);
        }
    }
    }
}
