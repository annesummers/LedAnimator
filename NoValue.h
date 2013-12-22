#ifndef NOVALUE_H
#define NOVALUE_H

#include <QObject>

#include "FrameValue.h"
#include "constants.h"

namespace AnimatorUi {

class NoValue : public FrameValue
{
    Q_OBJECT
public:
    explicit NoValue(QObject *parent);

    int type() const { return kNoValue; }
    inline FrameValue& operator=(const FrameValue& copyValue) { return *this; }

signals:

public slots:

};
}

#endif // NOVALUE_H
