#ifndef VALUEAXIS_H
#define VALUEAXIS_H

#include <QObject>

#include "Axis.h"

namespace AnimatorModel {

class TimeAxis;
class LinkedValue;


class ValueAxis : public Axis {
    Q_OBJECT
public:
    explicit ValueAxis(QObject *parent,
                       Animation &animation,
                       int number,
                       int lowValue,
                       int highValue,
                       int zeroValue,
                       int priority,
                       bool isOpaque);

    inline int number() const { return iNumber; }

private:
    int iNumber;
};

class ValueAxisData : public AxisData {
    Q_OBJECT

public:
    explicit ValueAxisData(QObject *parent,
                            Animation& animation,
                            ValueAxis& axis,
                            TimeAxis& timeAxis,
                            Led& led,
                            QUndoStack &undoStack);

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

#endif // VALUEAXIS_H
