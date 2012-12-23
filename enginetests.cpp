/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "enginetests.h"

using namespace Test;

EngineTests::EngineTests(QObject *parent) :
    QObject(parent)
{
}

void EngineTests::initTestCase() {
    //iEngine = new Engine(this);
}

void EngineTests::start() {
    QCOMPARE(true, true);
}

void EngineTests::askSaveAnimation_data() {
}

void EngineTests::askSaveAnimation() {
    QCOMPARE(true, true);
}

void EngineTests::cleanupTestCase() {

}
