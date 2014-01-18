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

