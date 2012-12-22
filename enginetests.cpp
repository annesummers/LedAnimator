#include "enginetests.h"

using namespace Test;

EngineTests::EngineTests(QObject *parent) :
    QObject(parent)
{
}

void EngineTests::initTestCase() {
    QCOMPARE(true, false);
}

void EngineTests::start() {
    QCOMPARE(true, false);
}

void EngineTests::askSaveAnimation_data() {
}

void EngineTests::askSaveAnimation() {
    QCOMPARE(true, false);
}

void EngineTests::cleanupTestCase() {

}
