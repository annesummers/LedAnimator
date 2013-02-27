/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "animationtests.h"

#include "animation.h"
#include "led.h"

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

void AnimationTests::setupNewEmpty_data() {
    QTest::addColumn<int>("numRows");
    QTest::addColumn<int>("numColumns");
    QTest::addColumn<int>("numFrames");
    QTest::addColumn<int>("frameFrequency");
    QTest::addColumn<int>("socketCount");
    QTest::addColumn<int>("currentFrame");
    QTest::addColumn<int>("numFramesCount");
    QTest::addColumn<QString>("errorString");

    QTest::newRow("negative numRows") << INVALID
                                      << DEFAULT_NUM_COLUMNS
                                      << DEFAULT_NUM_FRAMES
                                      << DEFAULT_FRAME_FREQUENCY
                                      << 0
                                      << INVALID
                                      << 0
                                      << "Animation::setupNew : numRows is zero or negative";

    QTest::newRow("negative numColumns") << DEFAULT_NUM_ROWS
                                         << INVALID
                                         << DEFAULT_NUM_FRAMES
                                         << DEFAULT_FRAME_FREQUENCY
                                         << 0
                                         << INVALID
                                         << 0
                                         << "Animation::setupNew : numColumns is zero or negative";

    QTest::newRow("negative numFrames") << DEFAULT_NUM_ROWS
                                        << DEFAULT_NUM_COLUMNS
                                        << INVALID
                                        << DEFAULT_FRAME_FREQUENCY
                                        << 0
                                        << INVALID
                                        << 0
                                        << "Animation::setupNew : numFrames is zero or negative";

    QTest::newRow("zero numRows") << 0
                                  << DEFAULT_NUM_COLUMNS
                                  << DEFAULT_NUM_FRAMES
                                  << DEFAULT_FRAME_FREQUENCY
                                  << 0
                                  << INVALID
                                  << 0
                                  << "Animation::setupNew : numRows is zero or negative";

    QTest::newRow("zero numColumns") << DEFAULT_NUM_ROWS
                                     << 0
                                     << DEFAULT_NUM_FRAMES
                                     << DEFAULT_FRAME_FREQUENCY
                                     << 0
                                     << INVALID
                                     << 0
                                     << "Animation::setupNew : numColumns is zero or negative";

    QTest::newRow("zero numFrames") << DEFAULT_NUM_ROWS
                                    << DEFAULT_NUM_COLUMNS
                                    << 0
                                    << DEFAULT_FRAME_FREQUENCY
                                    << 0
                                    << INVALID
                                    << 0
                                    << "Animation::setupNew : numFrames is zero or negative";

    QTest::newRow("too big numRows") << MAX_ROWS + 1
                                     << DEFAULT_NUM_COLUMNS
                                     << DEFAULT_NUM_FRAMES
                                     << DEFAULT_FRAME_FREQUENCY
                                     << 0
                                     << INVALID
                                     << 0
                                     << "Animation::setupNew : numRows is too big";

    QTest::newRow("too big numColumns") << DEFAULT_NUM_ROWS
                                        << MAX_COLUMNS + 1
                                        << DEFAULT_NUM_FRAMES
                                        << DEFAULT_FRAME_FREQUENCY
                                        << 0
                                        << INVALID
                                        << 0
                                        << "Animation::setupNew : numColumns is too big";

    QTest::newRow("too big numFrames") << DEFAULT_NUM_ROWS
                                       << DEFAULT_NUM_COLUMNS
                                       << MAX_FRAMES + 1
                                       << DEFAULT_FRAME_FREQUENCY
                                       << 0
                                       << INVALID
                                       << 0
                                       << "Animation::setupNew : numFrames is too big";

    QTest::newRow("default") << DEFAULT_NUM_ROWS
                             << DEFAULT_NUM_COLUMNS
                             << DEFAULT_NUM_FRAMES
                             << DEFAULT_FRAME_FREQUENCY
                             << DEFAULT_NUM_ROWS*DEFAULT_NUM_COLUMNS
                             << 1
                             << DEFAULT_NUM_FRAMES
                             << "";

    QTest::newRow("maximum") << MAX_ROWS
                             << MAX_COLUMNS
                             << MAX_FRAMES
                             << DEFAULT_FRAME_FREQUENCY
                             << MAX_ROWS*MAX_COLUMNS
                             << 1
                             << MAX_FRAMES
                             << "";
}

void AnimationTests::setupNewEmpty() {
    QFETCH(int, numRows);
    QFETCH(int, numColumns);
    QFETCH(int, numFrames);
    QFETCH(int, frameFrequency);
    QFETCH(int, socketCount);
    QFETCH(int, currentFrame);
    QFETCH(int, numFramesCount);
    QFETCH(QString, errorString);

    QSignalSpy newSocketSpy(iAnimation, SIGNAL(newSocket(int, int)));
    QSignalSpy currentFrameSpy(iAnimation, SIGNAL(currentFrameChanged(int)));
    QSignalSpy numFramesSpy(iAnimation, SIGNAL(numFramesChanged(int)));

    try {
        iAnimation->setupNew(numRows, numColumns, numFrames, frameFrequency);

        QCOMPARE(newSocketSpy.count(), socketCount);
        QCOMPARE(currentFrameSpy.count(), 1);
        QCOMPARE(numFramesSpy.count(), 1);

        QCOMPARE(currentFrameSpy.takeFirst().at(0).toInt(), currentFrame);
        QCOMPARE(numFramesSpy.takeFirst().at(0).toInt(), numFramesCount);

    } catch(IllegalArgumentException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    }
}

void AnimationTests::setupNewPopulated_data() {
    QTest::addColumn<int>("numRows");
    QTest::addColumn<int>("numColumns");
    QTest::addColumn<int>("numFrames");
    QTest::addColumn<int>("frameFrequency");
    QTest::addColumn<int>("ledCount");
    QTest::addColumn<IntList>("ledPositions");
    QTest::addColumn<QString>("errorString");

    int numRows = DEFAULT_NUM_ROWS;
    int numColumns = DEFAULT_NUM_COLUMNS;

    QList<QPoint> gridPositions;

    gridPositions.append(QPoint(0, 0));
    gridPositions.append(QPoint(0, 1));
    gridPositions.append(QPoint(1, 1));
    gridPositions.append(QPoint(1, 2));
    gridPositions.append(QPoint(2, 2));

    int numLeds = gridPositions.count();

    IntList positions;

    for(int i = 0; i < numRows * numColumns; i++) {
        positions.append(INVALID);
    }

    for(int i = 0; i < numLeds; i++) {
        positions.replace((gridPositions.at(i).y()*numColumns) + gridPositions.at(i).x(), i + INITIAL_LED);
    }

    QTest::newRow("partial") << numRows
                             << numColumns
                             << DEFAULT_NUM_FRAMES
                             << DEFAULT_FRAME_FREQUENCY
                             << numLeds
                             << positions
                             << "";
    gridPositions.clear();

    for(int i = 0; i < numColumns; i++) {
        for(int j = 0; j < numRows; j++) {
            gridPositions.append(QPoint(i, j));
        }
    }

    numLeds = gridPositions.count();

    positions.clear();

    for(int i = 0; i < numRows * numColumns; i++) {
        positions.append(INVALID);
    }

    for(int i = 0; i < numLeds; i++) {
        positions.replace((gridPositions.at(i).y()*numColumns) + gridPositions.at(i).x(), i + INITIAL_LED);
    }

    QTest::newRow("all") << numRows
                         << numColumns
                         << DEFAULT_NUM_FRAMES
                         << DEFAULT_FRAME_FREQUENCY
                         << numLeds
                         << positions
                         << "";
}

void AnimationTests::setupNewPopulated() {
    QFETCH(int, numRows);
    QFETCH(int, numColumns);
    QFETCH(int, numFrames);
    QFETCH(int, frameFrequency);
    QFETCH(int, ledCount);
    QFETCH(IntList, ledPositions);
    QFETCH(QString, errorString);

    QSignalSpy newLedSpy(iAnimation, SIGNAL(newLed(int, int)));

    try {
        iAnimation->setupNew(numRows, numColumns, numFrames, frameFrequency, ledCount, ledPositions);

        QCOMPARE(newLedSpy.count(), ledCount);

    } catch(IllegalArgumentException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    }
}


void AnimationTests::play() {
    iAnimation->play();

    QCOMPARE(iAnimation->isPlaying(), true);

    //Hmm how can we check the timer is firing correctly?
}

void AnimationTests::stop() {
    iAnimation->stop();

    QCOMPARE(iAnimation->isPlaying(), false);
}

void AnimationTests::setCurrentFrame_data() {
    QTest::addColumn<int>("currentFrame");
    QTest::addColumn<QString>("errorString");

    QTest::newRow("frame number too small") << 0
                                       << "Animation::setCurrentFrame : Frame number is smaller than first frame";
    QTest::newRow("frame number too big") << iAnimation->numFrames() + 1
                                          << "Animation::setCurrentFrame : Frame number is greater than last frame";
    QTest::newRow("valid") << INITIAL_FRAME
                           << "";
}

void AnimationTests::setCurrentFrame() {
    QFETCH(int, currentFrame);
    QFETCH(QString, errorString);

    try {
        iAnimation->setCurrentFrame(currentFrame);
    } catch(IllegalArgumentException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    }
}

void AnimationTests::ledAt_data() {
    QTest::addColumn<int>("row");
    QTest::addColumn<int>("column");
    QTest::addColumn<QString>("errorString");

    QTest::newRow("zero row") << 0
                              << iAnimation->numColumns() - 1
                              << "Animation::ledAt : Row is negative";

    QTest::newRow("zero column") << iAnimation->numRows() - 1
                                 << 0
                                 << "Animation::ledAt : Column is negative";

    QTest::newRow("row too big") << iAnimation->numRows()
                                 << iAnimation->numColumns() - 1
                                 << "Animation::ledAt : Row is greater than number of rows";

    QTest::newRow("column too big") << iAnimation->numRows() - 1
                                    << iAnimation->numColumns()
                                    << "Animation::ledAt : Column is greater than number of columns";

    QTest::newRow("valid") << iAnimation->numRows() - 1
                           << iAnimation->numColumns() - 1
                            << "";
}

void AnimationTests::ledAt() {
    QFETCH(int, row);
    QFETCH(int, column);
    QFETCH(QString, errorString);

    try {
        iAnimation->ledAt(row, column);

    } catch(IllegalArgumentException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    }
}

void AnimationTests::cleanupTestCase() {

}
