#include "ValueAxisData.h"

#include "Led.h"
#include "Animation.h"
#include "FunctionValue.h"
#include "LinkedValue.h"

using namespace AnimatorModel;

ValueAxisData::ValueAxisData(QObject *parent,
                            Animation& animation,
                            ValueAxis& axis,
                            TimeAxis& timeAxis, Led &led,
                            QUndoStack *undoStack) :
    AxisData(parent, animation, axis, led, undoStack),
    iAnchorAxis(timeAxis) {

    iFunctionRanges.append(Range(axis.zeroValue(), axis.lowValue(), axis.zeroValue(), 0));
    iFunctionRanges.append(Range(axis.zeroValue(), axis.zeroValue(), axis.highValue(), 0));

    for(int i = axis.lowValue(); i < axis.zeroValue(); i++) {
        frameAt(i).doSetValue(*(new FunctionValue(parent, Function(), 0)));
    }

    frameAt(iAxis.zeroValue()).doSetValue(*(new LinkedValue(parent, QColor())));

    for(int i = axis.zeroValue() + 1; i <= axis.highValue(); i++) {
        frameAt(i).doSetValue(*(new FunctionValue(parent, Function(), 0)));
    }
}


void ValueAxisData::setLinkedValue(LinkedValue& linkedValue) {
    frameAt(iAxis.zeroValue()).doSetValue(linkedValue);
}

void ValueAxisData::copyFrames(const AxisData &copyAxis) {
    AxisData::copyFrames(copyAxis);

    //iFunctionRanges = copyAxis.iFunctionRanges;
}

void ValueAxisData::lowValueChanged(const int lowValue) {
    Q_UNUSED(lowValue);
}

void ValueAxisData::highValueChanged(const int highValue) {
    Q_UNUSED(highValue);

}
void ValueAxisData::zeroValueChanged(const int zeroValue) {
    Q_UNUSED(zeroValue);
}
