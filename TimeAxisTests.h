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

    void play();
    void stop();

    void setRepeating_data();
    void setRepeating();

    void setSpeed_data();
    void setSpeed();

    void setLowValue_data();
    void setLowValue();

    void setHighValue_data();
    void setHighValue();

    void cleanupTestCase();

private:
    inline TimeAxis* timeAxis() { return static_cast<TimeAxis*>(iAxis); }
};
}

#endif // TIMEAXISTESTS_H
