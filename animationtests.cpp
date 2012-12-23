/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "animationtests.h"

#include "animation.h"
#include "led.h"

#include "exceptions.h"
#include "defaults.h"

#include "test.h"

using namespace Test;
using namespace Exception;

AnimationTests::AnimationTests(QObject *parent) :
    QObject(parent) {
}

void AnimationTests::initTestCase() {
    iEngine = new Engine(this);
    iAnimation = new Animation(*(iEngine));
}

void AnimationTests::setupNew_data() {
    QTest::addColumn<int>("numRows");
    QTest::addColumn<int>("numColumns");
    QTest::addColumn<int>("numFrames");
    QTest::addColumn<int>("ledCount");
    QTest::addColumn<int>("currentFrame");
    QTest::addColumn<int>("numFramesCount");
    QTest::addColumn<QString>("errorString");

    QTest::newRow("negative numRows") << INVALID
                                      << DEFAULT_NUM_COLUMNS
                                      << DEFAULT_NUM_FRAMES
                                      << 0
                                      << INVALID
                                      << 0
                                      << "numRows is zero or negative";

    QTest::newRow("negative numColumns") << DEFAULT_NUM_ROWS
                                         << INVALID
                                         << DEFAULT_NUM_FRAMES
                                         << 0
                                         << INVALID
                                         << 0
                                         << "numColumns is zero or negative";

    QTest::newRow("negative numFrames") << DEFAULT_NUM_ROWS
                                        << DEFAULT_NUM_COLUMNS
                                        << INVALID
                                        << 0
                                        << INVALID
                                        << 0
                                        << "numFrames is zero or negative";

    QTest::newRow("zero numRows") << 0
                                  << DEFAULT_NUM_COLUMNS
                                  << DEFAULT_NUM_FRAMES
                                  << 0
                                  << INVALID
                                  << 0
                                  << "numRows is zero or negative";

    QTest::newRow("zero numColumns") << DEFAULT_NUM_ROWS
                                     << 0
                                     << DEFAULT_NUM_FRAMES
                                     << 0
                                     << INVALID
                                     << 0
                                     << "numColumns is zero or negative";

    QTest::newRow("zero numFrames") << DEFAULT_NUM_ROWS
                                    << DEFAULT_NUM_COLUMNS
                                    << 0
                                    << 0
                                    << INVALID
                                    << 0
                                    << "numFrames is zero or negative";

    QTest::newRow("too big numRows") << MAX_ROWS + 1
                                     << DEFAULT_NUM_COLUMNS
                                     << DEFAULT_NUM_FRAMES
                                     << 0
                                     << INVALID
                                     << 0
                                     << "numRows is too big";

    QTest::newRow("too big numColumns") << DEFAULT_NUM_ROWS
                                        << MAX_COLUMNS + 1
                                        << DEFAULT_NUM_FRAMES
                                        << 0
                                        << INVALID
                                        << 0
                                        << "numColumns is too big";

    QTest::newRow("too big numFrames") << DEFAULT_NUM_ROWS
                                       << DEFAULT_NUM_COLUMNS
                                       << MAX_FRAMES + 1
                                       << 0
                                       << INVALID
                                       << 0
                                       << "numFrames is too big";

    QTest::newRow("default") << DEFAULT_NUM_ROWS
                             << DEFAULT_NUM_COLUMNS
                             << DEFAULT_NUM_FRAMES
                             << DEFAULT_NUM_ROWS*DEFAULT_NUM_COLUMNS
                             << 1
                             << DEFAULT_NUM_FRAMES
                             << "";

    QTest::newRow("maximum") << MAX_ROWS
                             << MAX_COLUMNS
                             << MAX_FRAMES
                             << MAX_ROWS*MAX_COLUMNS
                             << 1
                             << MAX_FRAMES
                             << "";
}

void AnimationTests::setupNew() {
    QFETCH(int, numRows);
    QFETCH(int, numColumns);
    QFETCH(int, numFrames);
    QFETCH(int, ledCount);
    QFETCH(int, currentFrame);
    QFETCH(int, numFramesCount);
    QFETCH(QString, errorString);

    QSignalSpy newLedSpy(iAnimation, SIGNAL(newLed(int, int)));
    QSignalSpy currentFrameSpy(iAnimation, SIGNAL(currentFrameChanged(int)));
    QSignalSpy numFramesSpy(iAnimation, SIGNAL(numFramesChanged(int)));

    try {
        iAnimation->setupNew(numRows, numColumns, numFrames);

        QCOMPARE(newLedSpy.count(), ledCount);
        QCOMPARE(currentFrameSpy.count(), 1);
        QCOMPARE(numFramesSpy.count(), 1);

        QCOMPARE(currentFrameSpy.takeFirst().at(0).toInt(), currentFrame);
        QCOMPARE(numFramesSpy.takeFirst().at(0).toInt(), numFramesCount);

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
                                       << "Frame number is smaller than first frame";
    QTest::newRow("frame number too big") << iAnimation->numFrames() + 1
                                          << "Frame number is greater than last frame";
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
                              << "Row is negative";

    QTest::newRow("zero column") << iAnimation->numRows() - 1
                                 << 0
                                 << "Column is negative";

    QTest::newRow("row too big") << iAnimation->numRows()
                                 << iAnimation->numColumns() - 1
                                 << "Row is greater than number of rows";

    QTest::newRow("column too big") << iAnimation->numRows() - 1
                                    << iAnimation->numColumns()
                                    << "Column is greater than number of columns";

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
