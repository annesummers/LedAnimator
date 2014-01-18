#include "TimeAxisTests.h"

#include "TimeAxis.h"
#include "engine.h"
#include "Animation.h"

using namespace AnimatorTest;

TimeAxisTests::TimeAxisTests(QObject *parent) :
    AxisTests(parent)
{
}

void TimeAxisTests::initTestCase() {
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

    iAnimation->newAnimation(numRows,
                             numColumns,
                             numLeds,
                             positions);
}

void TimeAxisTests::play() {
    iAnimation->addTimeAxis(0, 2, 1, kPriorityMed, false);
    iAxis = iAnimation->timeAxis();

    timeAxis()->play(false);

    QCOMPARE(timeAxis()->isPlaying(), true);

    //Hmm how can we check the timer is firing correctly?
}

void TimeAxisTests::stop() {
    iAnimation->addTimeAxis(0, 2, 1, kPriorityMed, false);
    iAxis = iAnimation->timeAxis();

    timeAxis()->stop();

    QCOMPARE(timeAxis()->isPlaying(), false);
}

void TimeAxisTests::setRepeating_data() {

}

void TimeAxisTests::setRepeating() {
    QCOMPARE(true, true);
}

void TimeAxisTests::setSpeed_data() {

}
void TimeAxisTests::setSpeed() {
    QCOMPARE(true, true);
}

void TimeAxisTests::setLowValue_data() {

}
void TimeAxisTests::setLowValue() {
    QCOMPARE(true, true);
}

void TimeAxisTests::setHighValue_data() {

}
void TimeAxisTests::setHighValue() {
    QCOMPARE(true, true);
}

void TimeAxisTests::cleanupTestCase() {

}
