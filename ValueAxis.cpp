#include "ValueAxis.h"

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
                            QUndoStack &undoStack) :
  AxisData(parent, animation, axis, undoStack) {

}


void ValueAxisData::copyFrames(const AxisData &copyAxis) {

}

void ValueAxisData::lowValueChanged(const int lowValue) {

}

void ValueAxisData::highValueChanged(const int highValue){

}
void ValueAxisData::zeroValueChanged(const int zeroValue){

}
