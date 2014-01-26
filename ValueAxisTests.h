#ifndef VALUEAXISTESTS_H
#define VALUEAXISTESTS_H

#include <QtTest/QtTest>

#include "AxisTests.h"
#include "ValueAxis.h"

class Engine;

namespace AnimatorModel {
class Animation;
}

using namespace AnimatorModel;

namespace AnimatorTest {

class ValueAxisTests : public AxisTests {
    Q_OBJECT

public:
    ValueAxisTests(QObject *parent = 0);

private slots:
    void initTestCase();

    void init();
    void cleanup();

    void constructor_data();
    void constructor();

    void cleanupTestCase();

private:
    inline ValueAxis* valueAxis() { return static_cast<ValueAxis*>(iAxis); }
};

}
#endif // VALUEAXISTESTS_H
