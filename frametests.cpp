#include "frametests.h"

using namespace Test;

FrameTests::FrameTests(QObject *parent) :
    QObject(parent)
{
}

void FrameTests::initTestCase() {
    QCOMPARE(true, false);
}

void FrameTests::setColour_data() {

}

void FrameTests::setColour() {
    QCOMPARE(true, false);
}
