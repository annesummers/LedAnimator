/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "ledtests.h"

#include <QUndoStack>

#include "Animation.h"
#include "Led.h"

#include "exceptions.h"
#include "constants.h"

using namespace AnimatorTest;
using namespace Exception;
using namespace AnimatorModel;

LedTests::LedTests(QObject *parent) :
    QObject(parent){
}

void LedTests::initTestCase() {
    iEngine = new Engine(this);
}

void LedTests::init() {
    iAnimation = new Animation(*iEngine);
}

void LedTests::cleanup() {
    delete iAnimation;
}

void LedTests::constructor_data() {
    QTest::addColumn<Position>("position");
    QTest::addColumn<QString>("error");

    QTest::newRow("negative row") << Position(INVALID, 1)
                                  << "Rows argument is invalid";

    QTest::newRow("negative column") << Position(1, INVALID)
                                     << "Columns argument is invalid";

    QTest::newRow("valid") << Position(1, 1)
                             << "";
}

void LedTests::constructor() {
    QFETCH(Position, position);
    QFETCH(QString, error);

    Led* led = NULL;

    try {
        iAnimation->newAnimation(DEFAULT_NUM_ROWS, DEFAULT_NUM_COLUMNS);

        led = new Led(iAnimation,
                      *iAnimation,
                      INITIAL_LED,
                      position,
                      NULL);

        QCOMPARE(led->position(), position);

    } catch(IllegalArgumentException& e){
        QCOMPARE(e.errorMessage(), error);

        return;
    }
}

void LedTests::setCurrentValue_data() {
}

void LedTests::setCurrentValue() {
    QCOMPARE(true, true);
}

void LedTests::addTimeAxis_data() {
}

void LedTests::addTimeAxis() {
    QCOMPARE(true, true);
}

void LedTests::addValueAxis_data() {

}

void LedTests::addValueAxis() {
    QCOMPARE(true, true);
}

void LedTests::move_data() {

}

void LedTests::move() {
    QCOMPARE(true, true);
}

void LedTests::paste_data() {

}

void LedTests::paste() {
    QCOMPARE(true, true);
}

void LedTests::copyAxes_data() {

}

void LedTests::copyAxes() {
    QCOMPARE(true, true);
}

void LedTests::copyTimeAxis_data() {

}

void LedTests::copyTimeAxis() {
    QCOMPARE(true, true);
}

void LedTests::copyValueAxis_data() {

}

void LedTests::copyValueAxis() {
    QCOMPARE(true, true);
}

void LedTests::copyCurrentFrame_data() {

}

void LedTests::copyCurrentFrame() {
    QCOMPARE(true, true);
}

void LedTests::assigment_data() {

}

void LedTests::assigment() {
    QCOMPARE(true, true);
}

void LedTests::currentColour_data() {

}

void LedTests::currentColour() {
    QCOMPARE(true, true);
}


/*
void LedTests::numFramesChanged_data() {
    QTest::addColumn<int>("numFrames");
    QTest::addColumn<QString>("errorString");

    QTest::newRow("zero frames") << 0
                                 << "Led::numFramesChanged : numFrames is zero or negative";

    QTest::newRow("negative frames") << INVALID
                                     << "Led::numFramesChanged : numFrames is zero or negative";
}

void LedTests::numFramesChanged() {
    QFETCH(int, numFrames);
    QFETCH(QString, errorString);

    try {
        setupAnimation();
        Led* led = iAnimation->ledAt(INITIAL_LED);

        led->numFramesChanged(numFrames); // should always throw an exception

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
                                           << "Led::frameAt : Frame number is zero or negative";
    QTest::newRow("invalid frame") << true
                                   << false
                                   << QColor(Qt::black)
                                   << "Led::frameAt : Frame number is greater than led number of frames";
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

    Led* led = NULL;

    try {
        if(animationSetup){
            iAnimation->setupNew(DEFAULT_NUM_ROWS, DEFAULT_NUM_COLUMNS, DEFAULT_NUM_FRAMES, DEFAULT_FRAME_FREQUENCY);
        }

        led = new Led(iAnimation, *iAnimation, 0, Position(0, 0));

        if(framesSetup) {
            led->numFramesChanged(DEFAULT_NUM_FRAMES);  // sets up the frame objects
        }

        QSignalSpy colourSpy(&(led->frameAt(iAnimation->currentFrame())), SIGNAL(colourChanged()));
        QSignalSpy currentColourSpy(led, SIGNAL(ledUpdated()));

        led->setCurrentColour(colour);

        QCOMPARE(led->currentColour(), colour);

        QCOMPARE(colourSpy.count(), 1);
        QCOMPARE(currentColourSpy.count(), 1);

    } catch(IllegalFrameNumberException& e) {
        QCOMPARE(e.errorMessage(), error);
    } catch(IllegalColourException& e) {
        QCOMPARE(e.errorMessage(), error);
    }

    delete led;
    //delete iAnimation; // deletes the led
}

void LedTests::select_data() {
    QTest::addColumn<bool>("selection");

    QTest::newRow("true") << true;
    QTest::newRow("false") << false;
}

void LedTests::select() {
    QFETCH(bool, selection);

    setupAnimation();

    Led* led = new Led(iAnimation, *iAnimation, 0, Position(0, 0));
    led->numFramesChanged(DEFAULT_NUM_FRAMES);  // sets up the frame objects

    QSignalSpy selectedSpy(led, SIGNAL(selected()));

    led->select(selection);

    QCOMPARE(selectedSpy.count(), 1);
    QCOMPARE(led->isSelected(), selection);
}

void LedTests::copyFrames_data() {

}

void LedTests::copyFrames() {

}*/

void LedTests::setupAnimation() {
    QList<Position> gridPositions;
    QList<int> positions;

    int numRows = DEFAULT_NUM_ROWS;
    int numColumns = DEFAULT_NUM_COLUMNS;

    for(int i = 0; i < numColumns; i++) {
        for(int j = 0; j < numRows; j++) {
            gridPositions.append(Position(i, j));
        }
    }

    for(int i = 0; i < numRows * numColumns; i++) {
        positions.append(INVALID);
    }

    int numLeds = gridPositions.count();

    for(int i = 0; i < numLeds; i++) {
        positions.replace((gridPositions.at(i).row()*numColumns) + gridPositions.at(i).column(), i + INITIAL_LED);
    }

    iAnimation->newAnimation(numRows,
                             numColumns,
                             numLeds,
                             positions);
}