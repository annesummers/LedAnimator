/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "frametests.h"

#include "Animation.h"
#include "Led.h"
#include "Frame.h"

#include "exceptions.h"
#include "constants.h"

using namespace AnimatorTest;
using namespace Exception;

FrameTests::FrameTests(QObject *parent) :
    QObject(parent)
{
}

void FrameTests::initTestCase() {
    iAnimation = new Animation(*(new Engine(this)));

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

    iAnimation->setupNew(numRows, numColumns, DEFAULT_NUM_FRAMES, DEFAULT_FRAME_FREQUENCY, numLeds, positions);
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

    Led* led = iAnimation->ledAt(Position(0, 0));

    try {
        Frame& frame = led->frameAt(INITIAL_FRAME);
        QSignalSpy colourSpy(&frame, SIGNAL(colourChanged()));

        frame.setColour(colour);

        QCOMPARE(frame.colour(), colour);
        QCOMPARE(colourSpy.count(), 1);

    } catch(IllegalColourException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    }
}
