/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "enginetests.h"

#include "Animation.h"
#include "engine.h"
#include "mainwindow.h"

using namespace AnimatorTest;

EngineTests::EngineTests(QObject *parent) :
    QObject(parent)
{
}

void EngineTests::initTestCase() {
    iEngine = new Engine(this);
    iAnimation = new Animation(*iEngine);
    iMainWindow = new MainWindow(*iEngine);
}

void EngineTests::start() {
    QCOMPARE(true, true);
}

void EngineTests::loadAnimation_data() {

}

void EngineTests::loadAnimation() {
    QCOMPARE(true, true);
}

void EngineTests::newAnimation_data() {

}

void EngineTests::newAnimation() {
    QTest::keyPress(iMainWindow, Qt::Key_N, Qt::ControlModifier);
}

void EngineTests::askSaveAnimation_data() {
}

void EngineTests::askSaveAnimation() {
    QCOMPARE(true, true);
}

void EngineTests::cleanupTestCase() {

}
