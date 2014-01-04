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
#include "LinkedValue.h"
#include "NoValue.h"

using namespace AnimatorModel;
using namespace UndoCommands;
using namespace Exception;

Frame::Frame(QObject *parent,
             AxisData &axisData,
             Animation& animation,
             int number,
             FrameValue& value,
             Frame *previous,
             QUndoStack &undoStack) :
    Selectable(parent, animation, number),
    iUndoStack(undoStack),
    iValue(&value),
    iAxisData(axisData),
    iPrevious(previous),
    iNext(NULL) {
}

Frame::Frame(QObject *parent,
             AxisData &axisData,
             Animation &animation,
             int number,
             Frame *previous,
             QUndoStack& undoStack) :
    Selectable(parent, animation, number),
    iUndoStack(undoStack),
    iValue(new NoValue(parent)),
    iAxisData(axisData),
    iPrevious(previous),
    iNext(NULL) {
}

void Frame::setValue(const FrameValue& value) {
    iUndoStack.push(new SetFrameColourCommand(iAnimation, *this, value));
    //doSetValue(value);
}

void Frame::doSetValue(const FrameValue& value) {
    iValue = (FrameValue*)(&value);

    if(value.type() == kLinked) {
        emit updateAll();
    } else {
        if(value.type() == kFunction) {

        }
        emit valueChanged();
    }
}

void Frame::setFirstInRange() {
    iAxisData.setFirstInRange(number());
}

void Frame::setLastInRange(Function function) {
    iAxisData.setLastInRange(number(), function);
}

void Frame::setAnchorInRange() {
    iAxisData.setAnchorInRange(number());
}

const Function Frame::function() const {
    switch(value().type()) {
    case kNoValue:
    case kColour:
    case kLinked:
        return Function();
    case kFunction: {
        const Frame* nextFrame = NULL;

        Function combinedFunction;

        const FunctionValue& functionValue = static_cast<const FunctionValue&>(value());

        combinedFunction += functionValue.function();

        if(number() < 0) {
            if(next() == NULL) {
                throw InvalidFrameException("Next frame should not be null");
            }

            nextFrame = next();

            while(nextFrame->value().type() == kFunction) {
                const FunctionValue& functionValue = static_cast<const FunctionValue&>(nextFrame->value());

                combinedFunction += functionValue.function();

                nextFrame = nextFrame->next();
            }
        }

        if(number() > 0) {
            if(previous() == NULL) {
                throw InvalidFrameException("Previous frame should not be null");
            }

            nextFrame = previous();

            while(nextFrame->value().type() == kFunction) {
                const FunctionValue& functionValue = static_cast<const FunctionValue&>(nextFrame->value());

                combinedFunction += functionValue.function();

                nextFrame = nextFrame->previous();
            }
        }

        if(nextFrame->value().type() == kNoValue) {
            throw InvalidFrameException("Frame has NoValue");
        }

        return combinedFunction;
    }
    }
}

const QColor Frame::colour() const {
    switch(value().type()) {
    case kNoValue:
        return QColor();
    case kColour:
    case kLinked:
        return static_cast<const ColourValue&>(value()).colour();
    case kFunction: {
        const Frame* nextFrame = NULL;

        Function combinedFunction;
        QColor colour;

        const FunctionValue& functionValue = static_cast<const FunctionValue&>(value());

        combinedFunction += functionValue.function();

        if(number() < 0) {
            if(next() == NULL) {
                throw InvalidFrameException("Next frame should not be null");
            }

            nextFrame = next();

            while(nextFrame->value().type() == kFunction) {
                const FunctionValue& functionValue = static_cast<const FunctionValue&>(nextFrame->value());

                combinedFunction += functionValue.function();

                nextFrame = nextFrame->next();
            }
        }

        if(number() > 0) {
            if(previous() == NULL) {
                throw InvalidFrameException("Previous frame should not be null");
            }

            nextFrame = previous();

            while(nextFrame->value().type() == kFunction) {
                const FunctionValue& functionValue = static_cast<const FunctionValue&>(nextFrame->value());

                combinedFunction += functionValue.function();

                nextFrame = nextFrame->previous();
            }
        }

        //Function combinedFunction = function();

        //if(nextFrame->value().type() == kNoValue) {
        //    throw InvalidFrameException("Frame has NoValue");
       // }

        const ColourValue& colourValue = static_cast<const ColourValue&>(nextFrame->value());
        float currentRedValue = colourValue.colour().redF();
        float currentGreenValue = colourValue.colour().greenF();
        float currentBlueValue = colourValue.colour().blueF();

        currentRedValue += combinedFunction.redIncrement();
        currentGreenValue += combinedFunction.greenIncrement();
        currentBlueValue += combinedFunction.blueIncrement();

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


       colour = QColor::fromRgbF(currentRedValue, currentGreenValue, currentBlueValue);

       return colour;
    }
    }
}
