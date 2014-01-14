#include "TimeAxisTests.h"

using namespace AnimatorTest;

TimeAxisTests::TimeAxisTests()
{
}

void TimeAxisTests::setupTestCase() {

}

void TimeAxisTests::play() {
    iTimeAxis->play(false);

    QCOMPARE(iTimeAxis->isPlaying(), true);

    //Hmm how can we check the timer is firing correctly?
}

void TimeAxisTests::stop() {
    iTimeAxis->stop();

    QCOMPARE(iAnimation->isPlaying(), false);
}

void TimeAxisTests::setCurrentFrame_data() {
    QTest::addColumn<int>("currentFrame");
    QTest::addColumn<QString>("errorString");

    QTest::newRow("frame number too small") << 0
                                       << "Animation::setCurrentFrame : Frame number is smaller than first frame";
    QTest::newRow("frame number too big") << iAnimation->numFrames() + 1
                                          << "Animation::setCurrentFrame : Frame number is greater than last frame";
    QTest::newRow("valid") << INITIAL_FRAME
                           << "";
}

void TimeAxisTests::setCurrentFrame() {
    QFETCH(int, currentFrame);
    QFETCH(QString, errorString);

    try {
        iTimeAxis->setCurrentFrame(currentFrame);
    } catch(IllegalArgumentException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    }
}


void TimeAxisTests::cleanupTestCase() {

}
