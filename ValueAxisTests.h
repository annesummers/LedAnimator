#ifndef VALUEAXISTESTS_H
#define VALUEAXISTESTS_H

#include <QtTest/QtTest>

#include "AxisTests.h"

class Engine;

namespace AnimatorModel {
class Animation;
class ValueAxis;
}

using namespace AnimatorModel;

namespace AnimatorTest {

class ValueAxisTests : public AxisTests {
    Q_OBJECT

public:
    ValueAxisTests(QObject *parent = 0);

private slots:
    void initTestCase();
    void cleanupTestCase();

private :
};

}
#endif // VALUEAXISTESTS_H
