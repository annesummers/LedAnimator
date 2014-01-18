/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "animationtests.h"

#include "Animation.h"
#include "Led.h"

#include "exceptions.h"
#include "constants.h"
#include "testconstants.h"

using namespace AnimatorTest;
using namespace Exception;
using namespace AnimatorModel;

AnimationTests::AnimationTests(QObject *parent) :
    QObject(parent) {
}

void AnimationTests::initTestCase() {
    iEngine = new Engine(this);
    iAnimation = new Animation(*(iEngine));
}

void AnimationTests::newEmpty_data() {
    QTest::addColumn<int>("numRows");
    QTest::addColumn<int>("numColumns");
    QTest::addColumn<int>("socketCount");
    QTest::addColumn<QString>("errorString");

    QTest::newRow("negative numRows") << INVALID
                                      << DEFAULT_NUM_COLUMNS
                                      << 0
                                      << "Animation::setupNew : numRows is zero or negative";

    QTest::newRow("negative numColumns") << DEFAULT_NUM_ROWS
                                         << INVALID
                                         << 0
                                         << "Animation::setupNew : numColumns is zero or negative";

    QTest::newRow("zero numRows") << 0
                                  << DEFAULT_NUM_COLUMNS
                                  << 0
                                  << "Animation::setupNew : numRows is zero or negative";

    QTest::newRow("zero numColumns") << DEFAULT_NUM_ROWS
                                     << 0
                                     << 0
                                     << "Animation::setupNew : numColumns is zero or negative";

   QTest::newRow("too big numRows") << MAX_ROWS + 1
                                     << DEFAULT_NUM_COLUMNS
                                     << 0
                                     << "Animation::setupNew : numRows is too big";

    QTest::newRow("too big numColumns") << DEFAULT_NUM_ROWS
                                        << MAX_COLUMNS + 1
                                        << 0
                                        << "Animation::setupNew : numColumns is too big";

    QTest::newRow("default") << DEFAULT_NUM_ROWS
                             << DEFAULT_NUM_COLUMNS
                             << DEFAULT_NUM_ROWS*DEFAULT_NUM_COLUMNS
                             << "";

    QTest::newRow("maximum") << MAX_ROWS
                             << MAX_COLUMNS
                             << MAX_ROWS*MAX_COLUMNS
                             << "";
}

void AnimationTests::newEmpty() {
    QFETCH(int, numRows);
    QFETCH(int, numColumns);
    QFETCH(int, socketCount);
    QFETCH(QString, errorString);

    QSignalSpy newSocketSpy(iAnimation, SIGNAL(newSocket(int, int)));

    try {
        iAnimation->newAnimation(numRows, numColumns);

        QCOMPARE(newSocketSpy.count(), socketCount);

    } catch(IllegalArgumentException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    }
}

void AnimationTests::newPopulated_data() {
    QTest::addColumn<int>("numRows");
    QTest::addColumn<int>("numColumns");
    QTest::addColumn<int>("ledCount");
    QTest::addColumn<IntList>("ledPositions");
    QTest::addColumn<QString>("errorString");

    int numRows = DEFAULT_NUM_ROWS;
    int numColumns = DEFAULT_NUM_COLUMNS;

    QList<Position> gridPositions;

    gridPositions.append(Position(0, 0));
    gridPositions.append(Position(1, 0));
    gridPositions.append(Position(1, 1));
    gridPositions.append(Position(2, 1));
    gridPositions.append(Position(2, 2));

    int numLeds = gridPositions.count();

    IntList positions;

    for(int i = 0; i < numRows * numColumns; i++) {
        positions.append(INVALID);
    }

    for(int i = 0; i < numLeds; i++) {
        positions.replace((gridPositions.at(i).row()*numColumns) + gridPositions.at(i).column(), i + INITIAL_LED);
    }

    QTest::newRow("partial") << numRows
                             << numColumns
                             << numLeds
                             << positions
                             << "";
    gridPositions.clear();

    for(int i = 0; i < numColumns; i++) {
        for(int j = 0; j < numRows; j++) {
            gridPositions.append(Position(i, j));
        }
    }

    numLeds = gridPositions.count();

    positions.clear();

    for(int i = 0; i < numRows * numColumns; i++) {
        positions.append(INVALID);
    }

    for(int i = 0; i < numLeds; i++) {
        positions.replace((gridPositions.at(i).row()*numColumns) + gridPositions.at(i).column(), i + INITIAL_LED);
    }

    QTest::newRow("all") << numRows
                         << numColumns
                         << numLeds
                         << positions
                         << "";
}

void AnimationTests::newPopulated() {
    QFETCH(int, numRows);
    QFETCH(int, numColumns);
    QFETCH(int, ledCount);
    QFETCH(IntList, ledPositions);
    QFETCH(QString, errorString);

    QSignalSpy newLedSpy(iAnimation, SIGNAL(newLed(int, int)));

    try {
        iAnimation->newAnimation(numRows, numColumns, ledCount, ledPositions);

        QCOMPARE(newLedSpy.count(), ledCount);

    } catch(IllegalArgumentException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    }
}

void AnimationTests::addNewLed_data() {

}

void AnimationTests::addNewLed() {
    QCOMPARE(true, true);
}

void AnimationTests::deleteLed_data() {

}

void AnimationTests::deleteLed() {
    QCOMPARE(true, true);
}

void AnimationTests::moveLed_data() {

}

void AnimationTests::moveLed() {
    QCOMPARE(true, true);
}

void AnimationTests::cloneLed_data() {

}

void AnimationTests::cloneLed() {
    QCOMPARE(true, true);
}

void AnimationTests::pasteLed_data() {

}

void AnimationTests::pasteLed() {
    QCOMPARE(true, true);
}

void AnimationTests::copyLedTimeAxis_data() {

}

void AnimationTests::copyLedTimeAxis() {
    QCOMPARE(true, true);
}

void AnimationTests::copyLedValueAxis_data() {

}

void AnimationTests::copyLedValueAxis() {
    QCOMPARE(true, true);
}

void AnimationTests::copyLedCurrentFrame_data() {

}

void AnimationTests::copyLedCurrentFrame() {
    QCOMPARE(true, true);
}

void AnimationTests::renumberLed_data() {

}

void AnimationTests::renumberLed() {
    QCOMPARE(true, true);
}

void AnimationTests::ledAt_data() {
    QTest::addColumn<Position>("position");
    QTest::addColumn<QString>("errorString");

    QTest::newRow("zero row") << Position(0, iAnimation->numColumns() - 1)
                              << "Animation::ledAt : Row is negative";

    QTest::newRow("zero column") << Position(iAnimation->numRows() - 1, 0)
                                 << "Animation::ledAt : Column is negative";

    QTest::newRow("row too big") << Position(iAnimation->numRows(), iAnimation->numColumns() - 1)
                                 << "Animation::ledAt : Row is greater than number of rows";

    QTest::newRow("column too big") << Position(iAnimation->numRows() - 1, iAnimation->numColumns())
                                    << "Animation::ledAt : Column is greater than number of columns";

    QTest::newRow("valid") << Position(iAnimation->numRows() - 1, iAnimation->numColumns() - 1)
                            << "";
}

void AnimationTests::ledAt() {
    QFETCH(Position, position);
    QFETCH(QString, errorString);

    try {
        iAnimation->ledAt(position);

    } catch(IllegalArgumentException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    }
}

void AnimationTests::addValueAxis_data() {

}

void AnimationTests::addValueAxis() {
    QCOMPARE(true, true);
}

void AnimationTests::addTimeAxis_data() {

}

void AnimationTests::addTimeAxis() {
    QCOMPARE(true, true);
}

void AnimationTests::axisAt_data() {

}

void AnimationTests::axisAt() {
    QCOMPARE(true, true);
}

void AnimationTests::timeAxisAt_data() {

}

void AnimationTests::timeAxisAt() {
    QCOMPARE(true, true);
}

void AnimationTests::currentTimeAxis_data() {

}

void AnimationTests::currentTimeAxis() {
    QCOMPARE(true, true);
}

void AnimationTests::switchTimeAxis_data() {

}

void AnimationTests::switchTimeAxis() {
    QCOMPARE(true, true);
}

void AnimationTests::addFunction_data() {

}

void AnimationTests::addFunction() {
    QCOMPARE(true, true);
}

void AnimationTests::functionAt_data() {

}

void AnimationTests::functionAt() {
    QCOMPARE(true, true);
}

void AnimationTests::cleanupTestCase() {

}
