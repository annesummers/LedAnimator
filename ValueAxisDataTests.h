#ifndef VALUEAXISDATATESTS_H
#define VALUEAXISDATATESTS_H

#include <QtTest/QtTest>

#include "AxisDataTests.h"

class Engine;

namespace AnimatorModel {
class Animation;
class ValueAxisData;
}

using namespace AnimatorModel;

namespace AnimatorTest {

class ValueAxisDataTests : public AxisDataTests {
    Q_OBJECT

public:
    ValueAxisDataTests(QObject *parent = 0);

private slots:
    void initTestCase();

    void init();
    void cleanup();

    void constructor_data();
    void constructor();

    void setLinkedValue_data();
    void setLinkedValue();

    void copyFrames_data();
    void copyFrames();

    void cleanupTestCase();

private:
    //inline ValueAxisData* valueAxis() { return static_cast<ValueAxisData*>(iAxisData); }
};

}

#endif // VALUEAXISDATATESTS_H
