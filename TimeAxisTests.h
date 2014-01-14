#ifndef TIMEAXISTESTS_H
#define TIMEAXISTESTS_H

#include <QObject>
#include <QtTest/QtTest>

class Engine;

namespace AnimatorModel {
class Animation;
class TimeAxis;
}

using namespace AnimatorModel;

namespace AnimatorTest {

class TimeAxisTests : public QObject {
    Q_OBJECT

public:
    TimeAxisTests(QObject *parent = 0);

private slots:
     void initTestCase();

    void setCurrentFrame_data();
    void setCurrentFrame();

    void play();
    void stop();

    void cleanupTestCase();

private :
    Engine*    iEngine;
    Animation* iAnimation;
    TimeAxis*  iTimeAxis;
};
}

#endif // TIMEAXISTESTS_H
