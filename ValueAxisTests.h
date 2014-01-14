#ifndef VALUEAXISTESTS_H
#define VALUEAXISTESTS_H

#include <QObject>
#include <QtTest/QtTest>

class Engine;

namespace AnimatorModel {
class Animation;
class ValueAxis;
}

using namespace AnimatorModel;

namespace AnimatorTest {

class ValueAxisTests : public QObject {
    Q_OBJECT


private slots:
     void initTestCase();
    void cleanupTestCase();

private :
    Engine*    iEngine;
    Animation* iAnimation;
    ValueAxis* iValueAxis;
};

}
#endif // VALUEAXISTESTS_H
