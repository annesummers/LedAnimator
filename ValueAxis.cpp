#include "ValueAxis.h"
#include "TimeAxis.h"

#include "FunctionValue.h"
#include "LinkedValue.h"

using namespace AnimatorModel;

ValueAxis::ValueAxis(QObject *parent,
                     Animation& animation,
                     int number,
                     int lowValue,
                     int highValue,
                     int zeroValue) :
      Axis(parent, animation, lowValue, highValue, zeroValue),
      iNumber(number) {
  }

ValueAxisData::ValueAxisData(QObject *parent,
                            Animation& animation,
                            ValueAxis& axis,
                            TimeAxis& timeAxis, Led &led,
                            QUndoStack &undoStack) :
    AxisData(parent, animation, axis, led, undoStack),
    iAnchorAxis(timeAxis) {

    for(int i = axis.lowValue(); i < axis.zeroValue(); i++) {
        frameAt(i).setValue(*(new FunctionValue(parent, Function())));
    }

    frameAt(iAxis.zeroValue()).setValue(*(new LinkedValue(parent, QColor())));

    for(int i = axis.zeroValue() + 1; i < axis.highValue(); i++) {
        frameAt(i).setValue(*(new FunctionValue(parent, Function())));
    }
}


void ValueAxisData::setLinkedValue(LinkedValue& linkedValue) {
    frameAt(iAxis.zeroValue()).setValue(linkedValue);
}


void ValueAxisData::copyFrames(const AxisData &copyAxis) {

}

void ValueAxisData::lowValueChanged(const int lowValue) {

}

void ValueAxisData::highValueChanged(const int highValue){

}
void ValueAxisData::zeroValueChanged(const int zeroValue){

}
