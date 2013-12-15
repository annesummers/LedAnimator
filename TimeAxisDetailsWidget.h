#ifndef TIMEAXISDETAILSWIDGET_H
#define TIMEAXISDETAILSWIDGET_H

#include <QWidget>

#include "AxisDetailsWidget.h"

namespace AnimatorUi {

class TimeAxisDetailsWidget : public AxisDetailsWidget {
    Q_OBJECT

public:
    explicit TimeAxisDetailsWidget(QWidget *parent,
                                   Animation &animation,
                                   Axis &axis,
                                   Engine& engine);

signals:

public slots:

protected:
    AxisData& axisData(Led& led);

};

}

#endif // TIMEAXISDETAILSWIDGET_H
