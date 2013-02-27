/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "leddetailswidgetstests.h"

#include "engine.h"
#include "animation.h"

#include "defaults.h"

using namespace Test;

LedDetailsWidgetsTests::LedDetailsWidgetsTests(QObject *parent) :
    QObject(parent)
{
}

void LedDetailsWidgetsTests::initTestCase() {
    iEngine = new Engine(this);
    iAnimation = new Animation(*iEngine);
    iAnimation->setupNew(DEFAULT_NUM_ROWS, DEFAULT_NUM_COLUMNS, DEFAULT_NUM_FRAMES, DEFAULT_FRAME_FREQUENCY);

   // QTest::mouseClick()
}

void LedDetailsWidgetsTests::listAddLed_data() {
    QCOMPARE(true, true);
}

void LedDetailsWidgetsTests::listAddLed() {
    QCOMPARE(true, true);
}
