#ifndef TIMEAXISDATA_H
#define TIMEAXISDATA_H

#include "AxisData.h"

#include <QUndoStack>

namespace AnimatorModel {

class TimeAxis;
class Animation;
class Led;

class TimeAxisData : public AxisData {
    Q_OBJECT

public:
    explicit TimeAxisData(QObject *parent,
                          Animation& animation,
                          TimeAxis& axis,
                          Led &led,
                          QUndoStack *undoStack);

    virtual void lowValueChanged(const int lowValue);
    virtual void highValueChanged(const int highValue);
    virtual void zeroValueChanged(const int zeroValue);
};

}

#endif // TIMEAXISDATA_H
