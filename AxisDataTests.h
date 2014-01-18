#ifndef AXISDATATESTS_H
#define AXISDATATESTS_H

#include <QObject>
#include <QtTest/QtTest>

class Engine;

namespace AnimatorModel {
class Animation;
}

using namespace AnimatorModel;

namespace AnimatorTest {

class AxisDataTests : public QObject {
    Q_OBJECT

public:
    explicit AxisDataTests(QObject *parent = 0);

private slots:
    void initTestCase();

    void newRange_data();
    void newRange();

private:
    Engine*    iEngine;
    Animation* iAnimation;

};
}

#endif // AXISDATATESTS_H
