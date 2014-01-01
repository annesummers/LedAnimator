#include "Function.h"

using namespace AnimatorModel;

Function::Function(float redIncrement,
                   float greenIncrement,
                   float blueIncrement) :
    iRedIncrement(redIncrement),
    iGreenIncrement(greenIncrement),
    iBlueIncrement(blueIncrement)
{
}

Function::Function()  :
    iRedIncrement(0),
    iGreenIncrement(0),
    iBlueIncrement(0) {
}

Function::Function(const Function& function) :
    iRedIncrement(function.iRedIncrement),
    iBlueIncrement(function.iBlueIncrement),
    iGreenIncrement(function.iGreenIncrement) {
}

Function& Function::operator=(const Function& function) {
    iRedIncrement = function.iRedIncrement;
    iGreenIncrement = function.iGreenIncrement;
    iBlueIncrement = function.iBlueIncrement;

    return *this;
}

Function& Function::operator+=(const Function& function) {
    iRedIncrement += function.iRedIncrement;
    iGreenIncrement += function.iGreenIncrement;
    iBlueIncrement += function.iBlueIncrement;

    return *this;
}
