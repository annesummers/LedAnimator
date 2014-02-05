#include "TimeAxisData.h"

#include "Led.h"
#include "Animation.h"
#include "ColourValue.h"

using namespace AnimatorModel;

TimeAxisData::TimeAxisData(QObject *parent,
                           Animation& animation,
                           TimeAxis& axis,
                           Led& led,
                           QUndoStack* undoStack) :
    AxisData(parent, animation, axis, led, undoStack) {

    for(int i = axis.lowValue(); i <= axis.highValue(); i++) {
        frameAt(i).doSetValue(*(new ColourValue(parent,
                                              QColor())));
    }
}

void TimeAxisData::lowValueChanged(const int lowValue) {
    Q_UNUSED(lowValue);
}

void TimeAxisData::highValueChanged(const int highValue){
    Q_UNUSED(highValue);
}

void TimeAxisData::zeroValueChanged(const int zeroValue){
    Q_UNUSED(zeroValue);
}
