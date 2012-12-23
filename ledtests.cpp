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
    Led* led = NULL;

    try {
        animation = new Animation(*(iEngine));
        animation->setupNew(DEFAULT_NUM_ROWS, DEFAULT_NUM_COLUMNS, DEFAULT_NUM_FRAMES);

        led = new Led(animation, *animation, row, column);

        QCOMPARE(led->row(), row);
        QCOMPARE(led->column(), column);

    } catch(IllegalArgumentException& e){
        QCOMPARE(e.errorMessage(), error);

        delete animation;
        return;
    }

    delete animation; // deletes the led
}

void LedTests::numFramesChanged_data() {
    QTest::addColumn<int>("numFrames");
    QTest::addColumn<QString>("errorString");

    QTest::newRow("zero frames") << 0
                                 << "numFrames is zero or negative";

    QTest::newRow("negative frames") << INVALID
                                     << "numFrames is zero or negative";
}

void LedTests::numFramesChanged() {
    QFETCH(int, numFrames);
    QFETCH(QString, errorString);

    Animation* animation = new Animation(*(iEngine));

    try {
        animation->setupNew(DEFAULT_NUM_ROWS, DEFAULT_NUM_COLUMNS, DEFAULT_NUM_FRAMES);
        Led& led = animation->ledAt(0, 0);

        led.numFramesChanged(numFrames); // should always throw an exception

    } catch(IllegalArgumentException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    } catch(IllegalFrameNumberException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    }
}

void LedTests::setCurrentColour_data() {
    QTest::addColumn<bool>("animationSetup");
    QTest::addColumn<bool>("framesSetup");
    QTest::addColumn<QColor>("colour");
    QTest::addColumn<QString>("error");

    QTest::newRow("invalid current frame") << false
                                           << true
                                           << QColor(Qt::black)
                                           << "Frame number is zero or negative";
    QTest::newRow("invalid frame") << true
                                   << false
                                   << QColor(Qt::black)
                                   << "Frame number is greater than led number of frames";
    QTest::newRow("invalid colour") << true
                                    << true
                                    << QColor()
                                    << "Invalid colour";
    QTest::newRow("valid") << true
                           << true
                           << QColor(Qt::black)
                           << "";
}

void LedTests::setCurrentColour() {
    QFETCH(bool, animationSetup);
    QFETCH(bool, framesSetup);
    QFETCH(QColor, colour);
    QFETCH(QString, error);

    Animation* animation = new Animation(*(iEngine));
    Led* led = NULL;

    try {
        if(animationSetup){
            animation->setupNew(DEFAULT_NUM_ROWS, DEFAULT_NUM_COLUMNS, DEFAULT_NUM_FRAMES);
        }

        led = new Led(animation, *animation, 0, 0);

        if(framesSetup) {
            led->numFramesChanged(DEFAULT_NUM_FRAMES);  // sets up the frame objects
        }

        led->setCurrentColour(colour);

        QCOMPARE(led->currentColour(), colour);

    } catch(IllegalFrameNumberException& e) {
        QCOMPARE(e.errorMessage(), error);
    } catch(IllegalColourException& e) {
        QCOMPARE(e.errorMessage(), error);
    }

    delete led;
    delete animation; // deletes the led
}

void LedTests::colourChanged_data() {

}

void LedTests::colourChanged() {
    QCOMPARE(true, true);  // TODO
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

    Led* led = new Led(animation, *animation, 0, 0);
    led->numFramesChanged(DEFAULT_NUM_FRAMES);  // sets up the frame objects

    QSignalSpy selectedSpy(led, SIGNAL(selected()));

    led->select(selection);

    QCOMPARE(selectedSpy.count(), 1);
   // QCOMPARE(selectedSpy.takeFirst().at(0).toBool(), selection);
    //TODO why is this not working?

    delete animation; // deletes the led
}

void LedTests::cleanupTestCase() {

}
