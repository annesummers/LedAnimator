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
#include "LinkedValue.h"

#include "constants.h"
#include "exceptions.h"

using namespace Exception;

Led::Led(QObject* parent,
         Animation &animation,
         int number,
         Position position,
         QUndoStack *undoStack) :
    GridItem(parent, animation, number, position),
    iTimeAxisData(NULL),
    iUndoStack(undoStack),
    iHidden(false) {

    if(!position.isValid()) {
        if(position.row() < 0) {
            throw IllegalArgumentException("Led:led : rows argument is invalid");
        }

        if(position.column() < 0) {
            throw IllegalArgumentException("Led:led : columns argument is invalid");
        }
    }

    if(number < INITIAL_LED) {
        throw IllegalArgumentException("Led:led : number is invalid");
    }

    connect(&animation, SIGNAL(timeAxisAdded()), this, SLOT(addTimeAxis()));
    connect(&animation, SIGNAL(valueAxisAdded(int)), this, SLOT(addValueAxis(int)));
    connect(&animation, SIGNAL(valueAxisDeleted(int)), this, SLOT(deleteValueAxis(int)));


   // connect(&(animation), SIGNAL(framesInserted(int,int)), this, SLOT(framesInserted(int, int)));
}

Led::~Led() {
}

Led::Led(const Led& copyLed) :
    GridItem(copyLed.parent(), copyLed.iAnimation, copyLed.number(), copyLed.position()),
    iTimeAxisData(NULL),
    iUndoStack(copyLed.iUndoStack),
    iHidden(false) {

    copyAxes(copyLed);

    connect(&iAnimation, SIGNAL(timeAxisAdded()), this, SLOT(addTimeAxis()));
    connect(&iAnimation, SIGNAL(valueAxisAdded(int)), this, SLOT(addValueAxis(int)));
    connect(&iAnimation, SIGNAL(valueAxisDeleted(int)), this, SLOT(deleteValueAxis(int)));

   // connect(&iAnimation, SIGNAL(framesInserted(int,int)), this, SLOT(framesInserted(int, int)));
}

Led& Led::operator=(const Led& led) {
    setNumber(led.number());
    setPosition(led.position());
    copyAxes(led);

    return *this;
}

void Led::addTimeAxis() {
    iTimeAxisData = new TimeAxisData(this,
                                     iAnimation,
                                     *iAnimation.timeAxis(),
                                     *this,
                                     iUndoStack);

    connect(&iTimeAxisData->axis(), SIGNAL(currentFrameChanged(int)), this, SLOT(currentFrameChanged(int)));
}

void Led::addValueAxis(int axisNumber) {
    iAxesData.append(new ValueAxisData(this,
                                       iAnimation,
                                       iAnimation.axisAt(axisNumber),
                                       *iAnimation.timeAxis(),
                                       *this,
                                       iUndoStack));

    if(iTimeAxisData != NULL) {
        currentFrameChanged(iTimeAxisData->currentFrameNum());
    }
}


void Led::deleteValueAxis(int axisNumber) {
    ValueAxisData* data = iAxesData.at(axisNumber);

    delete data;

    iAxesData.removeAt(axisNumber);
}

void Led::setTimeAxisCurrentValue(FrameValue& value) {
    iTimeAxisData->frameAt(iTimeAxisData->axis().currentFrameNum()).setValue(value);
}

const QColor Led::currentColour() const {
    const Frame& frame = iTimeAxisData->currentFrame();

    QColor colour = frame.colour();
    QColor backgroundColour = static_cast<TimeAxis&>(iTimeAxisData->axis()).backgroundColour();

    if(static_cast<TimeAxis&>(iTimeAxisData->axis()).usesBackgroundColour() &&
            colour.red() == backgroundColour.red() &&
            colour.green() == backgroundColour.green() &&
            colour.blue() == backgroundColour.blue()) {
        return colour;
    }

    Function combinedFunction;

    for(int i = 0; i < iAnimation.numValueAxes(); i++) {
        if(i < iAxesData.count()) {
            Function function = axisAt(i).currentFrame().function();

            if(!function.isNull()) {
                combinedFunction += function;
            }
        }
    }

    float currentRedValue = frame.colour().redF();
    float currentGreenValue = frame.colour().greenF();
    float currentBlueValue = frame.colour().blueF();

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

    return QColor::fromRgbF(currentRedValue, currentGreenValue, currentBlueValue);
}

ValueAxisData &Led::axisAt(int axisNum) const {
    if(axisNum < 0) {
        throw IllegalFrameNumberException("Led::frameAt : Frame number is greater than led number of frames");
    }

    if(axisNum > iAxesData.count()) {
        throw IllegalFrameNumberException("Led::frameAt : Frame number is greater than led number of frames");
    }

    return *(iAxesData.at(axisNum));
}

void Led::copyAxes(const Led &copyLed) {
    for(int i = 0; i < iAnimation.numValueAxes(); i++) {
        copyValueAxis(copyLed, i);
    }

    if(iAnimation.timeAxis() != NULL) {
        copyTimeAxis(copyLed);
    }
}

void Led::copyTimeAxis(const Led& copyLed) {
    if(iTimeAxisData == NULL) {
        addTimeAxis();
    }

    timeAxis()->copyFrames(*copyLed.timeAxis());
}

void Led::copyTimeAxisFrame(const Led& copyLed, int frameNum) {
    timeAxis()->frameAt(timeAxis()->currentFrameNum()).setValue(copyLed.timeAxis()->frameAt(frameNum).value());
}

void Led::copyValueAxis(const Led& copyLed, int axisNum) {
    if(iAxesData.size() < axisNum - 1) {
        addValueAxis(axisNum);
    }

    axisAt(axisNum).copyFrames(copyLed.axisAt(axisNum));
}

void Led::colourChanged(int frameNum) {
    iAnimation.setSaved(false);

    if(iTimeAxisData != NULL &&
       frameNum == iTimeAxisData->axis().currentFrameNum()) {
        emit ledUpdated();
    }
}

void Led::currentFrameChanged(int currentFrame) {
    for(int i = 0; i < iAnimation.numValueAxes(); i++) {
        if(i < iAxesData.count()) {
            axisAt(i).setLinkedValue(*(new LinkedValue(&axisAt(i), iTimeAxisData->frameAt(currentFrame).colour())));
        }
    }

    emit updated();
}

void Led::move(Position newPosition) {
    setPosition(newPosition);
}

void Led::paste(Led& copyLed) {
    Q_UNUSED(copyLed);
  //  setCurrentColour(copyLed.currentColour());
}
