#include "FunctionValue.h"

using namespace AnimatorModel;

FunctionValue::FunctionValue(QObject *parent, Function function, int number) :
    FrameValue(parent),
    iFunction(function),
    iNumber(number)
{
}

void FunctionValue::setFunction(Function function) {
    iFunction = function;
}

FrameValue& FunctionValue::operator=(const FrameValue& copyValue) {
    iFunction = static_cast<const FunctionValue&>(copyValue).function();
}
