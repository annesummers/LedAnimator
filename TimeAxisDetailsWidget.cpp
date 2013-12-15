#include "TimeAxisDetailsWidget.h"

using namespace AnimatorUi;

TimeAxisDetailsWidget::TimeAxisDetailsWidget(QWidget *parent,
                                             Animation &animation,
                                             Axis &axis,
                                             Engine &engine) :
    AxisDetailsWidget(parent, animation, axis, engine)
{
}


AxisData& TimeAxisDetailsWidget::axisData(Led &led) {
    return *led.timeAxis();
}
