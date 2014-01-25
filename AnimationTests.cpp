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
    QTest::addColumn<QString>("errorString");

    QTest::newRow("negative numRows") << INVALID
                                      << DEFAULT_NUM_COLUMNS
                                      << "Animation::setupNew : numRows is zero or negative";

    QTest::newRow("negative numColumns") << DEFAULT_NUM_ROWS
                                         << INVALID
                                         << "Animation::setupNew : numColumns is zero or negative";

    QTest::newRow("zero numRows") << 0
                                  << DEFAULT_NUM_COLUMNS
                                  << "Animation::setupNew : numRows is zero or negative";

    QTest::newRow("zero numColumns") << DEFAULT_NUM_ROWS
                                     << 0
                                     << "Animation::setupNew : numColumns is zero or negative";

   QTest::newRow("too big numRows") << MAX_ROWS + 1
                                     << DEFAULT_NUM_COLUMNS
                                     << "Animation::setupNew : numRows is too big";

    QTest::newRow("too big numColumns") << DEFAULT_NUM_ROWS
                                        << MAX_COLUMNS + 1
                                        << "Animation::setupNew : numColumns is too big";

    QTest::newRow("default") << DEFAULT_NUM_ROWS
                             << DEFAULT_NUM_COLUMNS
                             << "";

    QTest::newRow("maximum") << MAX_ROWS
                             << MAX_COLUMNS
                             << "";
}

void AnimationTests::newEmpty() {
    QFETCH(int, numRows);
    QFETCH(int, numColumns);
    QFETCH(QString, errorString);

    QSignalSpy newSocketSpy(iAnimation, SIGNAL(newSocket(int, int)));

    try {
        iAnimation->newAnimation(numRows, numColumns);

        QCOMPARE(newSocketSpy.count(), numRows * numColumns);

    } catch(IllegalArgumentException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    }
}

void AnimationTests::addNewLed_data() {
    QTest::addColumn<int>("numRows");
    QTest::addColumn<int>("numColumns");
    QTest::addColumn<PositionList>("ledPositions");
    QTest::addColumn<QString>("errorString");

    int numRows = 1;
    int numColumns = 1;

    PositionList gridPositions;

    gridPositions.append(Position(INVALID, 0));

    QTest::newRow("negative row") << numRows
                                  << numColumns
                                  << gridPositions
                                  << "Animation::addNewLed : row is negative";

    gridPositions.clear();
    gridPositions.append(Position(0, INVALID));

    QTest::newRow("negative column") << numRows
                                     << numColumns
                                     << gridPositions
                                     << "Animation::addNewLed : column is negative";

    gridPositions.clear();
    gridPositions.append(Position(numRows + 1, 0));

    QTest::newRow("row too big") << numRows
                                 << numColumns
                                 << gridPositions
                                 << "Animation::addNewLed : row is bigger than num rows";

    gridPositions.clear();
    gridPositions.append(Position(0, numColumns + 1));

    QTest::newRow("column too big") << numRows
                                    << numColumns
                                    << gridPositions
                                    << "Animation::addNewLed : column is bigger than num columns";

    gridPositions.clear();
    gridPositions.append(Position(0, 0));

    QTest::newRow("valid") << numRows
                           << numColumns
                           << gridPositions
                           << "";

    gridPositions.clear();
    gridPositions.append(Position(0, 0));
    gridPositions.append(Position(0, 0));

    QTest::newRow("too many leds") << numRows
                                   << numColumns
                                   << gridPositions
                                   << "Animation::addNewLed : num Leds is bigger than num sockets";

}

void AnimationTests::addNewLed() {
    QFETCH(int, numRows);
    QFETCH(int, numColumns);
    QFETCH(PositionList, ledPositions);
    QFETCH(QString, errorString);

    QSignalSpy newLedSpy(iAnimation, SIGNAL(newLed(int, int)));

    try {
        iAnimation->newAnimation(numRows, numColumns);

        for(int i = 0; i < ledPositions.count(); i++) {
            iAnimation->addNewLed(ledPositions.at(i));
        }

        QCOMPARE(newLedSpy.count(), ledPositions.count());

    } catch(IllegalArgumentException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    }
}

void AnimationTests::newPopulated_data() {
    QTest::addColumn<int>("numRows");
    QTest::addColumn<int>("numColumns");
    QTest::addColumn<int>("numLeds");
    QTest::addColumn<PositionList>("ledGridPositions");
    QTest::addColumn<IntList>("ledPositions");
    QTest::addColumn<QString>("errorString");

    int numRows = DEFAULT_NUM_ROWS;
    int numColumns = DEFAULT_NUM_COLUMNS;

    PositionList gridPositions;

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
                             << gridPositions
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
                         << gridPositions
                         << positions
                         << "";
}

void AnimationTests::newPopulated() {
    QFETCH(int, numRows);
    QFETCH(int, numColumns);
    QFETCH(int, numLeds);
    QFETCH(PositionList, ledGridPositions);
    QFETCH(IntList, ledPositions);
    QFETCH(QString, errorString);

    QSignalSpy newLedSpy(iAnimation, SIGNAL(newLed(int, int)));

    try {
        iAnimation->newAnimation(numRows, numColumns, numLeds, ledPositions);

        QCOMPARE(newLedSpy.count(), numLeds);
        QCOMPARE(iAnimation->numLeds(), numLeds);

        for(int i = INITIAL_LED; i < numLeds + INITIAL_LED; i++) {
            Position position = iAnimation->ledAt(i)->position();
            QCOMPARE(ledGridPositions.contains(position), true);
        }

    } catch(IllegalArgumentException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    }
}

void AnimationTests::newPopulatedReplace_data() {
    QTest::addColumn<int>("numRows");
    QTest::addColumn<int>("numColumns");
    QTest::addColumn<int>("numLeds");
    QTest::addColumn<PositionList>("ledGridPositions");
    QTest::addColumn<IntList>("ledPositions");
    QTest::addColumn<int>("newNumRows");
    QTest::addColumn<int>("newNumColumns");
    QTest::addColumn<int>("newNumLeds");
    QTest::addColumn<PositionList>("newLedGridPositions");
    QTest::addColumn<IntList>("newLedPositions");
    QTest::addColumn<QString>("errorString");

    int numRows = 3;
    int numColumns = 3;

    PositionList gridPositions;

    gridPositions.append(Position(0, 0));
    gridPositions.append(Position(0, 1));
    gridPositions.append(Position(1, 1));

    int numLeds = gridPositions.count();

    IntList positions;

    for(int i = 0; i < numRows * numColumns; i++) {
        positions.append(INVALID);
    }

    for(int i = 0; i < numLeds; i++) {
        positions.replace((gridPositions.at(i).row()*numColumns) + gridPositions.at(i).column(), i + INITIAL_LED);
    }

    int newNumRows = 2;
    int newNumColumns = 2;

    PositionList newGridPositions;

    newGridPositions.append(Position(0, 0));
    newGridPositions.append(Position(1, 0));
    newGridPositions.append(Position(1, 1));

    int newNumLeds = newGridPositions.count();

    IntList newPositions;

    for(int i = 0; i < newNumRows * newNumColumns; i++) {
        newPositions.append(INVALID);
    }

    for(int i = 0; i < newNumLeds; i++) {
        newPositions.replace((newGridPositions.at(i).row()*newNumColumns) + newGridPositions.at(i).column(), i + INITIAL_LED);
    }

    QTest::newRow("smaller grid") << numRows
                             << numColumns
                             << numLeds
                             << gridPositions
                             << positions
                             << newNumRows
                              << newNumColumns
                              << newNumLeds
                              << newGridPositions
                              << newPositions
                             << "";

    newNumRows = 4;
    newNumColumns = 4;

    newPositions.clear();

    for(int i = 0; i < newNumRows * newNumColumns; i++) {
        newPositions.append(INVALID);
    }

    for(int i = 0; i < newNumLeds; i++) {
        newPositions.replace((newGridPositions.at(i).row()*newNumColumns) + newGridPositions.at(i).column(), i + INITIAL_LED);
    }

    QTest::newRow("larger grid") << numRows
                             << numColumns
                             << numLeds
                             << gridPositions
                             << positions
                             << newNumRows
                              << newNumColumns
                              << newNumLeds
                              << newGridPositions
                              << newPositions
                             << "";

    newNumRows = 3;
    newNumColumns = 3;

    newGridPositions.clear();
    newGridPositions.append(Position(0, 0));
    newGridPositions.append(Position(1, 0));
    newGridPositions.append(Position(1, 1));
    newGridPositions.append(Position(2, 1));

    newNumLeds = newGridPositions.count();

    newPositions.clear();

    for(int i = 0; i < newNumRows * newNumColumns; i++) {
        newPositions.append(INVALID);
    }

    for(int i = 0; i < newNumLeds; i++) {
        newPositions.replace((newGridPositions.at(i).row()*newNumColumns) + newGridPositions.at(i).column(), i + INITIAL_LED);
    }

    QTest::newRow("more leds") << numRows
                             << numColumns
                             << numLeds
                             << gridPositions
                             << positions
                             << newNumRows
                              << newNumColumns
                              << newNumLeds
                              << newGridPositions
                              << newPositions
                             << "";

    newGridPositions.clear();
    newGridPositions.append(Position(0, 0));
    newGridPositions.append(Position(1, 0));

    newNumLeds = newGridPositions.count();

    newPositions.clear();

    for(int i = 0; i < newNumRows * newNumColumns; i++) {
        newPositions.append(INVALID);
    }

    for(int i = 0; i < newNumLeds; i++) {
        newPositions.replace((newGridPositions.at(i).row()*newNumColumns) + newGridPositions.at(i).column(), i + INITIAL_LED);
    }

    QTest::newRow("fewer leds") << numRows
                             << numColumns
                             << numLeds
                             << gridPositions
                             << positions
                             << newNumRows
                              << newNumColumns
                              << newNumLeds
                              << newGridPositions
                              << newPositions
                             << "";
}

void AnimationTests::newPopulatedReplace() {
    QFETCH(int, numRows);
    QFETCH(int, numColumns);
    QFETCH(int, numLeds);
    QFETCH(PositionList, ledGridPositions);
    QFETCH(IntList, ledPositions);
    QFETCH(int, newNumRows);
    QFETCH(int, newNumColumns);
    QFETCH(int, newNumLeds);
    QFETCH(PositionList, newLedGridPositions);
    QFETCH(IntList, newLedPositions);
    QFETCH(QString, errorString);

    try {
        iAnimation->newAnimation(numRows, numColumns, numLeds, ledPositions);
        iAnimation->newAnimation(newNumRows, newNumColumns, newNumLeds, newLedPositions);

        QCOMPARE(iAnimation->numLeds(), newNumLeds);
        QCOMPARE(iAnimation->numRows(), newNumRows);
        QCOMPARE(iAnimation->numColumns(), newNumColumns);

        for(int i = INITIAL_LED; i < newNumLeds + INITIAL_LED; i++) {
            Position position = iAnimation->ledAt(i)->position();
            QCOMPARE(newLedGridPositions.contains(position), true);
        }

    } catch(IllegalArgumentException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    }
}

void AnimationTests::newEmptyReplace_data() {
    QTest::addColumn<int>("numRows");
    QTest::addColumn<int>("numColumns");
    QTest::addColumn<int>("numLeds");
    QTest::addColumn<PositionList>("ledGridPositions");
    QTest::addColumn<IntList>("ledPositions");
    QTest::addColumn<int>("newNumRows");
    QTest::addColumn<int>("newNumColumns");
    QTest::addColumn<QString>("errorString");

    int numRows = 3;
    int numColumns = 3;

    PositionList gridPositions;

    gridPositions.append(Position(0, 0));
    gridPositions.append(Position(0, 1));
    gridPositions.append(Position(1, 1));

    int numLeds = gridPositions.count();

    IntList positions;

    for(int i = 0; i < numRows * numColumns; i++) {
        positions.append(INVALID);
    }

    for(int i = 0; i < numLeds; i++) {
        positions.replace((gridPositions.at(i).row()*numColumns) + gridPositions.at(i).column(), i + INITIAL_LED);
    }

    int newNumRows = 2;
    int newNumColumns = 2;

    QTest::newRow("smaller grid") << numRows
                             << numColumns
                             << numLeds
                             << gridPositions
                             << positions
                             << newNumRows
                              << newNumColumns
                             << "";

    newNumRows = 4;
    newNumColumns = 4;

    QTest::newRow("larger grid") << numRows
                             << numColumns
                             << numLeds
                             << gridPositions
                             << positions
                             << newNumRows
                              << newNumColumns
                             << "";
}

void AnimationTests::newEmptyReplace() {
    QFETCH(int, numRows);
    QFETCH(int, numColumns);
    QFETCH(int, numLeds);
    QFETCH(PositionList, ledGridPositions);
    QFETCH(IntList, ledPositions);
    QFETCH(int, newNumRows);
    QFETCH(int, newNumColumns);
    QFETCH(QString, errorString);

    try {
        iAnimation->newAnimation(numRows, numColumns, numLeds, ledPositions);
        iAnimation->newAnimation(newNumRows, newNumColumns);

        QCOMPARE(iAnimation->numLeds(), 0);
        QCOMPARE(iAnimation->numRows(), newNumRows);
        QCOMPARE(iAnimation->numColumns(), newNumColumns);

    } catch(IllegalArgumentException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    }
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

void AnimationTests::deleteLed_data() {
    QTest::addColumn<int>("numRows");
    QTest::addColumn<int>("numColumns");
    QTest::addColumn<PositionList>("ledAddPositions");
    QTest::addColumn<PositionList>("ledDeletePositions");
    QTest::addColumn<QString>("errorString");

    int numRows = 1;
    int numColumns = 1;

    PositionList addPositions;

    addPositions.append(Position(0, 0));

    PositionList deletePositions;

    deletePositions.append(Position(0, 0));

    QTest::newRow("valid") << numRows
                           << numColumns
                           << addPositions
                           << deletePositions
                           << "";

    addPositions.clear();

    QTest::newRow("led doesn't exist") << numRows
                                   << numColumns
                                   << addPositions
                                   << deletePositions
                                   << "Animation::deleteLed : led does not exist";

}

void AnimationTests::deleteLed() {
    QFETCH(int, numRows);
    QFETCH(int, numColumns);
    QFETCH(PositionList, ledAddPositions);
    QFETCH(PositionList, ledDeletePositions);
    QFETCH(QString, errorString);

    QSignalSpy deleteLedSpy(iAnimation, SIGNAL(ledDeleted(int, int, int)));

    try {
        iAnimation->newAnimation(numRows, numColumns);

        for(int i = 0; i < ledAddPositions.count(); i++) {
            iAnimation->addNewLed(ledAddPositions.at(i));
        }

        for(int i = 0; i < ledDeletePositions.count(); i++) {
            Led* deleteLed = iAnimation->ledAt(ledDeletePositions.at(i));

            if(deleteLed == NULL) {
                deleteLed = new Led(iAnimation,
                                    *iAnimation,
                                    1,
                                    ledDeletePositions.at(i),
                                    NULL);
            }

            iAnimation->deleteLed(*deleteLed, true);
        }

        QCOMPARE(deleteLedSpy.count(), ledDeletePositions.count());

    } catch(InvalidPositionException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    }
}

void AnimationTests::moveLed_data() {
    QTest::addColumn<PositionList>("ledAddPositions");
    QTest::addColumn<Position>("ledFromPosition");
    QTest::addColumn<Position>("ledToPosition");
    QTest::addColumn<QString>("errorString");

    PositionList addPositions;

    addPositions.append(Position(0, 0));

    QTest::newRow("valid") << addPositions
                           << Position(0, 0)
                           << Position(1, 0)
                           << "";

    QTest::newRow("from doesn't exist") << addPositions
                                       << Position(1, 0)
                                       << Position(0, 0)
                                       << "Animation::moveLed : led does not exist";

    addPositions.append(Position(1, 0));

    QTest::newRow("to already exists") << addPositions
                                       << Position(0, 0)
                                       << Position(1, 0)
                                       << "Animation::moveLed : led already in this position";
}

void AnimationTests::moveLed() {
    QFETCH(PositionList, ledAddPositions);
    QFETCH(Position, ledFromPosition);
    QFETCH(Position, ledToPosition);
    QFETCH(QString, errorString);

    QSignalSpy moveLedSpy(iAnimation, SIGNAL(ledMoved(int, int, int, int)));

    try {
        iAnimation->newAnimation(2, 2);

        for(int i = 0; i < ledAddPositions.count(); i++) {
            iAnimation->addNewLed(ledAddPositions.at(i));
        }

        int ledNum = INVALID;
        Led* fromLed = iAnimation->ledAt(ledFromPosition);
        if(fromLed != NULL) {
            ledNum = fromLed->number();
        }

        iAnimation->moveLed(ledFromPosition, ledToPosition);

        Led* movedLed = iAnimation->ledAt(ledToPosition);

        QCOMPARE(movedLed->number(), ledNum);
        QCOMPARE(movedLed->position(), ledToPosition);

        QCOMPARE(moveLedSpy.count(), 1);

    } catch(InvalidPositionException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    }
}

void AnimationTests::cloneLed_data() {
    QTest::addColumn<PositionList>("ledAddPositions");
    QTest::addColumn<Position>("ledFromPosition");
    QTest::addColumn<Position>("ledToPosition");
    QTest::addColumn<QString>("errorString");

    PositionList addPositions;

    addPositions.append(Position(0, 0));

    QTest::newRow("to empty position") << addPositions
                           << Position(0, 0)
                           << Position(1, 0)
                           << "";

    addPositions.append(Position(1, 0));

    QTest::newRow("to populated position") << addPositions
                                       << Position(0, 0)
                                       << Position(1, 0)
                                       << "";

    QTest::newRow("from doesn't exist") << addPositions
                                       << Position(0, 1)
                                       << Position(1, 0)
                                       << "Animation::cloneLed : led does not exist";
}

void AnimationTests::cloneLed() {
    QFETCH(PositionList, ledAddPositions);
    QFETCH(Position, ledFromPosition);
    QFETCH(Position, ledToPosition);
    QFETCH(QString, errorString);

    try {
        iAnimation->newAnimation(2, 2);

        for(int i = 0; i < ledAddPositions.count(); i++) {
            iAnimation->addNewLed(ledAddPositions.at(i));
        }

        int fromLedNum = INVALID;
        Led* fromLed = iAnimation->ledAt(ledFromPosition);
        if(fromLed != NULL) {
            fromLedNum = fromLed->number();
        }

        int toLedNum = INVALID;
        Led* toLed = iAnimation->ledAt(ledToPosition);
        if(toLed != NULL) {
            toLedNum = toLed->number();
        }

        iAnimation->cloneLed(ledFromPosition, ledToPosition);

        Led* clonedLed = iAnimation->ledAt(ledToPosition);
        QCOMPARE(clonedLed->position(), ledToPosition);

        if(toLed == NULL) {
            QCOMPARE(clonedLed->number(), iAnimation->numLeds());
        } else {
            QCOMPARE(clonedLed->number(), toLedNum);
        }

    } catch(InvalidPositionException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    }
}

void AnimationTests::cutAndPasteLed_data() {
    QTest::addColumn<PositionList>("ledAddPositions");
    QTest::addColumn<Position>("ledFromPosition");
    QTest::addColumn<Position>("ledToPosition");
    QTest::addColumn<bool>("moveToClipboard");
    QTest::addColumn<QString>("errorString");

    PositionList addPositions;

    addPositions.append(Position(0, 0));

    QTest::newRow("to empty position") << addPositions
                           << Position(0, 0)
                           << Position(1, 0)
                           << true
                           << "";

    addPositions.append(Position(1, 0));

    QTest::newRow("to populated position") << addPositions
                                       << Position(0, 0)
                                       << Position(1, 0)
                                       << true
                                       << "";

    QTest::newRow("from not on clipboard") << addPositions
                                       << Position(0, 0)
                                       << Position(1, 0)
                                       << false
                                       << "Animation::pasteLed : cannot find the led on the clipboard";
}

void AnimationTests::cutAndPasteLed() {
    QFETCH(PositionList, ledAddPositions);
    QFETCH(Position, ledFromPosition);
    QFETCH(Position, ledToPosition);
    QFETCH(bool, moveToClipboard);
    QFETCH(QString, errorString);

    try {
        iAnimation->newAnimation(2, 2);

        for(int i = 0; i < ledAddPositions.count(); i++) {
            iAnimation->addNewLed(ledAddPositions.at(i));
        }

        int fromLedNum = INVALID;
        Led* fromLed = iAnimation->ledAt(ledFromPosition);
        if(fromLed != NULL) {
            fromLedNum = fromLed->number();
        }

        int toLedNum = INVALID;
        Led* toLed = iAnimation->ledAt(ledToPosition);
        if(toLed != NULL) {
            toLedNum = toLed->number();
        }

        if(moveToClipboard) {
            iAnimation->moveLedToClipboard(ledFromPosition);
        }

        iAnimation->pasteClipboardLed(ledFromPosition, ledToPosition);

        Led* pastedLed = iAnimation->ledAt(ledToPosition);
        QCOMPARE(pastedLed->position(), ledToPosition);
        QCOMPARE(pastedLed->number(), fromLed->number());

    } catch(InvalidPositionException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    }
}

void AnimationTests::renumberLed_data() {

}

void AnimationTests::renumberLed() {
    QCOMPARE(true, false);
}

void AnimationTests::addValueAxis_data() {
    QTest::addColumn<int>("lowValue");
    QTest::addColumn<int>("highValue");
    QTest::addColumn<int>("zeroValue");
    QTest::addColumn<int>("priority");
    QTest::addColumn<bool>("isOpaque");
}

void AnimationTests::addValueAxis() {
    QFETCH(int, lowValue);
    QFETCH(int, highValue);
    QFETCH(int, zeroValue);
    QFETCH(int, priority);
    QFETCH(bool, isOpaque);

    try {
        iAnimation->newAnimation(2, 2);

        int axisNum = iAnimation->addValueAxis(lowValue, highValue, zeroValue, priority, isOpaque);

        QCOMPARE(iAnimation->numValueAxes(), axisNum - 1);

        QCOMPARE(iAnimation->axisAt(axisNum).lowValue(), lowValue);
        QCOMPARE(iAnimation->axisAt(axisNum).highValue(), highValue);
        QCOMPARE(iAnimation->axisAt(axisNum).zeroValue(), zeroValue);
        QCOMPARE(iAnimation->axisAt(axisNum).priority(), priority);
        QCOMPARE(iAnimation->axisAt(axisNum).isOpaque(), isOpaque);
    } catch(IllegalArgumentException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    }
}

void AnimationTests::addTimeAxis_data() {
    QTest::addColumn<int>("lowValue");
    QTest::addColumn<int>("highValue");
    QTest::addColumn<int>("frequency");
    QTest::addColumn<int>("priority");
    QTest::addColumn<bool>("isOpaque");
}

void AnimationTests::addTimeAxis() {
    QFETCH(int, lowValue);
    QFETCH(int, highValue);
    QFETCH(int, frequency);
    QFETCH(int, priority);
    QFETCH(bool, isOpaque);

    try {
        iAnimation->newAnimation(2, 2);

        iAnimation->addTimeAxis(lowValue, highValue, zeroValue, priority, isOpaque);

        QVERIFY(iAnimation->timeAxis() != NULL);

        QCOMPARE(iAnimation->timeAxis()->lowValue(), lowValue);
        QCOMPARE(iAnimation->timeAxis()->highValue(), highValue);
        QCOMPARE(iAnimation->timeAxis()-frequency(), zeroValue);
        QCOMPARE(iAnimation->timeAxis()->priority(), priority);
        QCOMPARE(iAnimation->timeAxis()->isOpaque(), isOpaque);
    } catch(IllegalArgumentException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    }
}

void AnimationTests::currentTimeAxis_data() {

}

void AnimationTests::currentTimeAxis() {
    QCOMPARE(true, false);
}

void AnimationTests::switchTimeAxis_data() {

}

void AnimationTests::switchTimeAxis() {
    QCOMPARE(true, false);
}

void AnimationTests::addFunction_data() {

}

void AnimationTests::addFunction() {
    QCOMPARE(true, false);
}

void AnimationTests::cleanupTestCase() {

}
