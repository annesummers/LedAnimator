/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "frametests.h"

#include "animation.h"
#include "led.h"
#include "frame.h"

#include "exceptions.h"
#include "defaults.h"

using namespace Test;
using namespace Exception;

FrameTests::FrameTests(QObject *parent) :
    QObject(parent)
{
}

void FrameTests::initTestCase() {
    iAnimation = new Animation(*(new Engine(this)));
    iAnimation->setupNew(DEFAULT_NUM_ROWS, DEFAULT_NUM_COLUMNS, DEFAULT_NUM_FRAMES);
}

void FrameTests::setColour_data() {
    QTest::addColumn<QColor>("colour");
    QTest::addColumn<QString>("errorString");

    QTest::newRow("invalid colour") << QColor()
                                    << "Invalid colour";

    QTest::newRow("valid") << QColor(Qt::black)
                           << "";
}

void FrameTests::setColour() {
    QFETCH(QColor, colour);
    QFETCH(QString, errorString);

    Led& led = iAnimation->ledAt(0, 0);

    try {
        Frame& frame = led.frameAt(INITIAL_FRAME);
        QSignalSpy colourSpy(&frame, SIGNAL(colourChanged()));

        frame.setColour(colour);

        QCOMPARE(frame.colour(), colour);
        QCOMPARE(colourSpy.count(), 1);

    } catch(IllegalColourException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    }
}
