#include "FunctionValue.h"

using namespace AnimatorModel;

FunctionValue::FunctionValue(QObject *parent, Function function) :
    FrameValue(parent),
    iFunction(function)
{
}

void FunctionValue::setFunction(Function function) {
    iFunction = function;
}

FrameValue& FunctionValue::operator=(const FrameValue& copyValue) {
    iFunction = static_cast<const FunctionValue&>(copyValue).function();
}
