#ifndef FUNCTIONVALUE_H
#define FUNCTIONVALUE_H

#include <QObject>

#include "FrameValue.h"
#include "Function.h"
#include "constants.h"

namespace AnimatorModel {

class FunctionValue : public FrameValue {
    Q_OBJECT

public:
    explicit FunctionValue(QObject *parent, Function function, int number);

    void setFunction(Function function);
    inline const Function function() const { return iFunction; }

    inline int number() const { return iNumber; }

    FrameValue& operator=(const FrameValue& copyValue);

    inline int type() const { return kFunction; }

signals:

public slots:

private:
    Function iFunction;
    int iNumber;
};

}

#endif // FUNCTIONVALUE_H
