#ifndef VALUEAXISDATA_H
#define VALUEAXISDATA_H

#include "AxisData.h"

#include <QUndoStack>

namespace AnimatorModel {

class TimeAxis;
class LinkedValue;
class Animation;
class Led;
class ValueAxis;

class ValueAxisData : public AxisData {
    Q_OBJECT

public:
    explicit ValueAxisData(QObject *parent,
                            Animation& animation,
                            ValueAxis& axis,
                            TimeAxis& timeAxis,
                            Led& led,
                            QUndoStack *undoStack);

    void copyFrames(const AxisData &copyAxis);

public slots:
    void setLinkedValue(LinkedValue& linkedValue);

    virtual void lowValueChanged(const int lowValue);
    virtual void highValueChanged(const int highValue);
    virtual void zeroValueChanged(const int zeroValue);

private:
    TimeAxis& iAnchorAxis;
};
}

#endif // VALUEAXISDATA_H
