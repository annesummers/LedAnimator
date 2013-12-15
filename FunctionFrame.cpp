#include "FunctionFrame.h"

using namespace AnimatorModel;

FunctionFrame::FunctionFrame(QObject *parent,
                             Animation& animation,
                             int number,
                             Frame &previousFrame,
                             QUndoStack& undoStack) :
    Frame(parent, animation, number, *(new FunctionValue(parent, Function())), undoStack),
    iPreviousFrame(previousFrame) {
}

FunctionFrame::FunctionFrame(QObject* parent,
                           Animation& animation,
                           Function function,
                           int number,
                           Frame& previousFrame,
                           QUndoStack &undoStack) :
    Frame(parent, animation, number, *(new FunctionValue(parent, function)), undoStack),
    iPreviousFrame(previousFrame) {}

const QColor FunctionFrame::colour() const {
    Frame& previous = iPreviousFrame;
    int counter = 1;

    while(previous.type() == kFunction) {
        counter++;
    }
    ColourValue& colourValue = static_cast<ColourValue&>(previous.value());
   // ColourFrame& colourframe = static_cast<ColourFrame&>(previous);
    float currentRedValue = colourValue.colour().redF();
    float currentBlueValue = colourValue.colour().blueF();
    float currentGreenValue = colourValue().colour().greenF();

    for(int i = 0; i < counter; i++) {
        currentRedValue -= function().redIncrement;
        currentBlueValue -= function().greenIncrement;
        currentGreenValue -= function().blueIncrement;

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

void FunctionFrame::setValue(FrameValue& value) {
    Frame::setValue(value);
}
