#ifndef LINKEDVALUE_H
#define LINKEDVALUE_H

#include <QObject>

#include "FrameValue.h"
#include "ColourValue.h"
#include "constants.h"

namespace AnimatorModel {

class LinkedValue : public ColourValue {
    Q_OBJECT

public:
    explicit LinkedValue(QObject *parent, QColor colour);

    inline int type() const { return kLinked; }
};
}

#endif // LINKEDVALUE_H
