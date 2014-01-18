#ifndef VALUEAXIS_H
#define VALUEAXIS_H

#include <QObject>

#include "Axis.h"

namespace AnimatorModel {

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
}

#endif // VALUEAXIS_H
