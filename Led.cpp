/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "Led.h"

#include "ledwidget.h"
#include "Frame.h"
#include "Animation.h"
#include "ColourValue.h"
#include "FunctionValue.h"

#include "constants.h"
#include "exceptions.h"

using namespace Exception;

Led::Led(QObject* parent, Animation &animation, int number, Position position, QUndoStack &undoStack) :
    GridItem(parent, animation, number, position),
    iSignalMapper(NULL),
    iUndoStack(undoStack),
    iHidden(false) {

#ifndef NDEBUG   
    if(!position.isValid()) {
        if(position.row() < 0) {
            throw IllegalArgumentException("Rows argument is invalid");
        }

        if(position.column() < 0) {
            throw IllegalArgumentException("Columns argument is invalid");
        }
    }
#endif

    iSignalMapper = new QSignalMapper(this);

    connect(&animation, SIGNAL(timeAxisAdded()), this, SLOT(addTimeAxis()));
    connect(&animation, SIGNAL(valueAxisAdded(int)), this, SLOT(addValueAxis(int)));

   // connect(&(animation), SIGNAL(framesInserted(int,int)), this, SLOT(framesInserted(int, int)));
}

Led::~Led() {
}

Led::Led(const Led& copyLed) :
    GridItem(copyLed.parent(), copyLed.iAnimation, copyLed.number(), copyLed.position()),
    iSignalMapper(NULL),
    iUndoStack(copyLed.iUndoStack),
    iHidden(false) {

    iSignalMapper = new QSignalMapper(this);

    copyAxes(copyLed);

    connect(&iAnimation, SIGNAL(timeAxisAdded()), this, SLOT(addTimeAxis()));
    connect(&iAnimation, SIGNAL(valueAxisAdded(int)), this, SLOT(addValueAxis(int)));

   // connect(&iAnimation, SIGNAL(framesInserted(int,int)), this, SLOT(framesInserted(int, int)));
}

Led& Led::operator=(const Led& led) {
    setNumber(led.number());
    setPosition(led.position());
    copyAxes(led);

    return *this;
}

void Led::addTimeAxis() {
    iTimeAxisData = new TimeAxisData(this, iAnimation, *iAnimation.timeAxis(), iUndoStack);
}

void Led::addValueAxis(int axisNumber) {
    iAxesData.append(new ValueAxisData(this, iAnimation, iAnimation.axisAt(axisNumber), iUndoStack));
}

const QColor Led::currentColour() const {
    Frame& frame = iTimeAxisData->frameAt(iTimeAxisData->currentFrame());

    return frame.colour();
    /*switch(frame.value().type()) {
    case kColour:
    return static_cast<const ColourValue&>(frame.value()).colour();
    case kFunction: {
        if(iTimeAxisData->currentFrame() > 0) {
            Frame& current = iTimeAxisData->frameAt(iTimeAxisData->currentFrame());
            const FunctionValue& functionValue = static_cast<const FunctionValue&>(current.value());
            Frame* previous = &current;
            int counter = 1;

            while(previous->value().type() == kFunction) {
                previous = &(iTimeAxisData->frameAt(iTimeAxisData->currentFrame() - counter));
                counter++;
            }

            const ColourValue& colourValue = static_cast<const ColourValue&>(previous->value());
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

    }*/
}

ValueAxisData &Led::axisAt(int axisNum) const {
#ifndef NDEBUG
    if(axisNum < 0) {
        throw IllegalFrameNumberException("Led::frameAt : Frame number is greater than led number of frames");
    }

    if(axisNum > iAxesData.count()) {
        throw IllegalFrameNumberException("Led::frameAt : Frame number is greater than led number of frames");
    }
#endif

    return *(iAxesData.at(axisNum));
}

void Led::copyAxes(const Led &copyLed) {
    for(int i = 0; i < iAnimation.numAxes(); i++) {
        axisAt(i).copyFrames(copyLed.axisAt(i));
    }

    timeAxis()->copyFrames(*copyLed.timeAxis());
}


/*void Led::colourChanged(int frameNum) {
    iAnimation.setSaved(false);

    if(frameNum == iAnimation.currentFrame()) {
        emit ledUpdated();
    }
}

void Led::setCurrentColour(QColor colour) {    
    frameAt(iAnimation.currentFrame()).setColour(colour);
}*/

void Led::move(Position newPosition) {
    setPosition(newPosition);
}

void Led::paste(Led& copyLed) {
  //  setCurrentColour(copyLed.currentColour());
}
