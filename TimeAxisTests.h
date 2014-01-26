#ifndef TIMEAXISTESTS_H
#define TIMEAXISTESTS_H

#include <QtTest/QtTest>

#include "AxisTests.h"
#include "TimeAxis.h"

class Engine;

namespace AnimatorModel {
class Animation;
}

using namespace AnimatorModel;

namespace AnimatorTest {

class TimeAxisTests : public AxisTests {
    Q_OBJECT

public:
    TimeAxisTests(QObject *parent = 0);

private slots:
    void initTestCase();

    void init();
    void cleanup();

    void constructor_data();
    void constructor();

    void play();
    void stop();

    void setBackgroundColour_data();
    void setBackgroundColour();

    void setFrequency_data();
    void setFrequency();

    void nextFrame_data();
    void nextFrame();

    void cleanupTestCase();

private:
    inline TimeAxis* timeAxis() { return static_cast<TimeAxis*>(iAxis); }
};
}

#endif // TIMEAXISTESTS_H
