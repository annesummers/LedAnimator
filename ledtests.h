/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef LEDTESTS_H
#define LEDTESTS_H

#include <QObject>
#include <QtTest/QtTest>

class Engine;

namespace AnimatorModel {
class Animation;
}

using namespace AnimatorModel;

namespace AnimatorTest {

class LedTests : public QObject {

    Q_OBJECT
public:
    explicit LedTests(QObject *parent = 0);
    
signals:
    
private slots:
    void initTestCase();

    void init();
    void cleanup();

    void constructor_data();
    void constructor();

    void numFramesChanged_data();
    void numFramesChanged();

    void setCurrentColour_data();
    void setCurrentColour();

    void select_data();
    void select();

    void copyFrames_data();
    void copyFrames();

private:
    Engine*    iEngine;
    Animation* iAnimation;

    void setupAnimation();
};
}

#endif // LEDTESTS_H
