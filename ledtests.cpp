/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "ledtests.h"

#include "animation.h"
#include "led.h"

#include "exceptions.h"
#include "defaults.h"

using namespace Test;
using namespace Exception;

LedTests::LedTests(QObject *parent) :
    QObject(parent){
}

void LedTests::initTestCase() {
    iEngine = new Engine(this);
}

void LedTests::constructor_data() {
    QTest::addColumn<int>("row");
    QTest::addColumn<int>("column");
    QTest::addColumn<QString>("error");

    QTest::newRow("negative row") << INVALID
                                  << 1
                                  << "Rows argument is invalid";

    QTest::newRow("negative column") << 1
                                     << INVALID
                                     << "Columns argument is invalid";

    QTest::newRow("valid") << 1
                             << 1
                             << "";
}

void LedTests::constructor() {
    QFETCH(int, row);
    QFETCH(int, column);
    QFETCH(QString, error);

    Animation* animation = NULL;

    try {
        animation = new Animation(*(iEngine));
        animation->setupNew(DEFAULT_NUM_ROWS, DEFAULT_NUM_COLUMNS, DEFAULT_NUM_FRAMES);

        iLed = new Led(animation, *animation, row, column);

        QCOMPARE(iLed->row(), row);
        QCOMPARE(iLed->column(), column);

    } catch(IllegalArgumentException& e){
        QCOMPARE(e.errorMessage(), error);
        iLed = NULL;

        delete animation;

        return;
    }

    delete iLed;
    iLed = NULL;

    delete animation;
}

void LedTests::setCurrentColour_data() {
    QTest::addColumn<bool>("animationSetup");
    QTest::addColumn<bool>("framesSetup");
    QTest::addColumn<QColor>("colour");
    QTest::addColumn<QString>("error");

    QTest::newRow("invalid") << false << true << QColor(Qt::black) << "Frame number is zero or negative";
    QTest::newRow("invalid") << true << false << QColor(Qt::black) << "Frame number is greater than led number of frames";
    QTest::newRow("invalid") << true << true << QColor() << "Invalid colour";
    QTest::newRow("valid") << true << true << QColor(Qt::black) << "";
}

void LedTests::numFramesChanged_data() {
}

void LedTests::numFramesChanged() {
    QCOMPARE(true, false);
}

void LedTests::colourChanged_data() {

}

void LedTests::colourChanged() {
    QCOMPARE(true, false);
}

void LedTests::setCurrentColour() {
    QFETCH(bool, animationSetup);
    QFETCH(bool, framesSetup);
    QFETCH(QColor, colour);
    QFETCH(QString, error);

    Animation* animation = new Animation(*(iEngine));

    try {
        if(animationSetup){
            animation->setupNew(DEFAULT_NUM_ROWS, DEFAULT_NUM_COLUMNS, DEFAULT_NUM_FRAMES);
        }

        iLed = new Led(animation, *animation, 0, 0);

        if(framesSetup) {
            iLed->numFramesChanged(DEFAULT_NUM_FRAMES);  // sets up the frame objects
        }

        iLed->setCurrentColour(colour);

        QCOMPARE(iLed->currentColour(), colour);

    } catch(IllegalFrameNumberException& e) {
        QCOMPARE(e.errorMessage(), error);
    } catch(IllegalColourException& e) {
        QCOMPARE(e.errorMessage(), error);
    }

    delete iLed;
    iLed = NULL;

    delete animation;
}

void LedTests::select_data() {
    QTest::addColumn<bool>("selection");

    QTest::newRow("true") << true;
    QTest::newRow("false") << false;
}

void LedTests::select() {
    QFETCH(bool, selection);

    Animation* animation = new Animation(*(iEngine));
    animation->setupNew(DEFAULT_NUM_ROWS, DEFAULT_NUM_COLUMNS, DEFAULT_NUM_FRAMES);

    iLed = new Led(animation, *animation, 0, 0);
    iLed->numFramesChanged(DEFAULT_NUM_FRAMES);  // sets up the frame objects

    QSignalSpy selectedSpy(iLed, SIGNAL(selected()));

    iLed->select(selection);

    QCOMPARE(selectedSpy.count(), 1);
   // QCOMPARE(selectedSpy.takeFirst().at(0).toBool(), selection);

    delete animation;
}

void LedTests::cleanupTestCase() {

}
