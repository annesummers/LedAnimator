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
                     int zeroValue,
                     int priority,
                     bool isOpaque) :
      Axis(parent, animation, lowValue, highValue, zeroValue, priority, isOpaque),
      iNumber(number) {
  }

ValueAxisData::ValueAxisData(QObject *parent,
                            Animation& animation,
                            ValueAxis& axis,
                            TimeAxis& timeAxis, Led &led,
                            QUndoStack &undoStack) :
    AxisData(parent, animation, axis, led, undoStack),
    iAnchorAxis(timeAxis) {

    iFunctionRanges.append(Range(axis.zeroValue(), axis.lowValue(), axis.zeroValue(), 0));
    iFunctionRanges.append(Range(axis.zeroValue(), axis.zeroValue(), axis.highValue(), 0));

    for(int i = axis.lowValue(); i < axis.zeroValue(); i++) {
        frameAt(i).setValue(*(new FunctionValue(parent, Function(), 0)));
    }

    frameAt(iAxis.zeroValue()).setValue(*(new LinkedValue(parent, QColor())));

    for(int i = axis.zeroValue() + 1; i < axis.highValue(); i++) {
        frameAt(i).setValue(*(new FunctionValue(parent, Function(), 0)));
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
