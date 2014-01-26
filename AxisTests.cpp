#include "AxisTests.h"

#include "Animation.h"
#include "exceptions.h"

using namespace AnimatorTest;
using namespace Exception;

AxisTests::AxisTests(QObject *parent) :
    QObject(parent) { }

void AxisTests::initTestCase() {
    iEngine = new Engine(this);
}

void AxisTests::init() {
    iAnimation = new Animation(*iEngine);
}

void AxisTests::cleanup() {
    delete iAnimation;
}

void AxisTests::constructor_data() {
    QTest::addColumn<int>("lowValue");
    QTest::addColumn<int>("highValue");
    QTest::addColumn<int>("zeroValue");
    QTest::addColumn<int>("priority");
    QTest::addColumn<bool>("isOpaque");
    QTest::addColumn<QString>("errorString");

    QTest::newRow("low value greater than high value") << 10
                                                   << -10
                                                   << 0
                                                   << kPriorityHigh
                                                   << false
                                                   << "";

    QTest::newRow("zero value greater than high value") << -10
                                                       << 10
                                                       << -11
                                                       << kPriorityHigh
                                                       << false
                                                       << "";

    QTest::newRow("zero value smaller than low value") << -10
                                                       << 10
                                                       << 11
                                                       << kPriorityHigh
                                                       << false
                                                       << "";

    QTest::newRow("priority higher than highest priority") << -10
                                                       << 10
                                                       << 0
                                                       << kPriorityHigh + 1
                                                       << false
                                                       << "";

    QTest::newRow("priority lower than lowest priority") << -10
                                                       << 10
                                                       << 0
                                                       << kPriorityLow - 1
                                                       << false
                                                       << "";

    QTest::newRow("valid") << -10
                           << 10
                           << 0
                           << kPriorityLow
                           << false
                           << "";

}

void AxisTests::constructor() {
    QFETCH(int, lowValue);
    QFETCH(int, highValue);
    QFETCH(int, zeroValue);
    QFETCH(int, priority);
    QFETCH(bool, isOpaque);
    QFETCH(QString, errorString);

    try {
        Axis* axis = new Axis(iAnimation,
                              *iAnimation,
                              lowValue,
                              highValue,
                              zeroValue,
                              priority,
                              isOpaque);

        QCOMPARE(axis->lowValue(), lowValue);
        QCOMPARE(axis->highValue(), highValue);
        QCOMPARE(axis->zeroValue(), zeroValue);
        QCOMPARE(axis->priority(), priority);
        QCOMPARE(axis->isOpaque(), isOpaque);

    } catch(IllegalArgumentException& e){
        QCOMPARE(e.errorMessage(), errorString);

        return;
    }
}

void AxisTests::setCurrentFrame_data() {
    QTest::addColumn<int>("currentFrame");
    QTest::addColumn<QString>("errorString");

    QTest::newRow("frame number too small") << iAxis->lowValue() - 1
                                            << "Axis::setCurrentFrame : Frame number is smaller than first frame";

    QTest::newRow("frame number too big") << iAxis->highValue() + 1
                                          << "Axis::setCurrentFrame : Frame number is greater than last frame";

    QTest::newRow("valid") << iAxis->lowValue()
                           << "";
}

void AxisTests::setCurrentFrame() {
    QFETCH(int, currentFrame);
    QFETCH(QString, errorString);

    try {
        iAxis->setCurrentFrame(currentFrame);
    } catch(IllegalArgumentException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    }
}

void AxisTests::setLowValue_data() {
    QTest::addColumn<int>("lowValue");
    QTest::addColumn<int>("highValue");
    QTest::addColumn<int>("zeroValue");
    QTest::addColumn<int>("newLowValue");
    QTest::addColumn<QString>("errorString");

    QTest::newRow("greater than zero value") << -10
                           << 10
                           << 0
                           << 1
                           << "Axis:setLowValue : zero value is smaller than low value";

    QTest::newRow("valid") << -10
                           << 10
                           << 0
                           << -1
                           << "";
}

void AxisTests::setLowValue() {
    QFETCH(int, lowValue);
    QFETCH(int, highValue);
    QFETCH(int, zeroValue);
    QFETCH(int, newLowValue);
    QFETCH(QString, errorString);

    try {
        Axis* axis = new Axis(iAnimation,
                              *iAnimation,
                              lowValue,
                              highValue,
                              zeroValue,
                              kPriorityHigh,
                              false);

        axis->setLowValue(newLowValue);

    } catch(IllegalArgumentException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    }
}

void AxisTests::setHighValue_data() {
    QTest::addColumn<int>("lowValue");
    QTest::addColumn<int>("highValue");
    QTest::addColumn<int>("zeroValue");
    QTest::addColumn<int>("newHighValue");
    QTest::addColumn<QString>("errorString");

    QTest::newRow("smaller than zero value") << -10
                           << 10
                           << 0
                           << -1
                           << "Axis:setHighValue : zero value is greater than high value";

    QTest::newRow("valid") << -10
                           << 10
                           << 0
                           << 1
                           << "";

}

void AxisTests::setHighValue() {
    QFETCH(int, lowValue);
    QFETCH(int, highValue);
    QFETCH(int, zeroValue);
    QFETCH(int, newHighValue);
    QFETCH(QString, errorString);

    try {
        Axis* axis = new Axis(iAnimation,
                              *iAnimation,
                              lowValue,
                              highValue,
                              zeroValue,
                              kPriorityHigh,
                              false);

        axis->setHighValue(newHighValue);

    } catch(IllegalArgumentException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    }
}

void AxisTests::cleanupTestCase() {

}
