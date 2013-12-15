#include "ValueAxisDetailsWidget.h"

using namespace AnimatorUi;

ValueAxisDetailsWidget::ValueAxisDetailsWidget(QWidget *parent,
                                               Animation &animation,
                                               Axis &axis,
                                               Engine& engine) :
    AxisDetailsWidget(parent, animation, axis, engine)
{
}

AxisData& ValueAxisDetailsWidget::axisData(Led &led) {
    return led.axisAt(static_cast<ValueAxis&>(iAxis).number());
}
