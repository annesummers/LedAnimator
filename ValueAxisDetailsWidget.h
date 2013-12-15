#ifndef VALUEAXISDETAILSWIDGET_H
#define VALUEAXISDETAILSWIDGET_H

#include <QWidget>

#include "AxisDetailsWidget.h"

namespace AnimatorUi {

class ValueAxisDetailsWidget : public AxisDetailsWidget
{
    Q_OBJECT
public:
    explicit ValueAxisDetailsWidget(QWidget *parent, Animation &animation, Axis &axis, Engine &engine);

signals:

public slots:

protected:
    AxisData& axisData(Led& led);

};
}

#endif // VALUEAXISDETAILSWIDGET_H
