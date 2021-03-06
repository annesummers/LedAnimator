#ifndef AXISTESTS_H
#define AXISTESTS_H

#include <QtTest/QtTest>

#include <QObject>

class Engine;

namespace AnimatorModel {
class Animation;
class Axis;
}

using namespace AnimatorModel;

namespace AnimatorTest {

class AxisTests : public QObject {
    Q_OBJECT

public:
    AxisTests(QObject *parent = 0);

protected slots:
    void initTestCase();

    void init();
    void cleanup();

    void constructor_data();
    void constructor();

    void setCurrentFrame_data();
    void setCurrentFrame();

    void setLowValue_data();
    void setLowValue();

    void setHighValue_data();
    void setHighValue();

    void cleanupTestCase();

protected:
    Engine*    iEngine;
    Animation* iAnimation;
    Axis*       iAxis;
};
}

#endif // AXISTESTS_H
