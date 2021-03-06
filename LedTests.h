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

    void copyConstructor_data();
    void copyConstructor();

    void setTimeAxisCurrentValue_data();
    void setTimeAxisCurrentValue();

    void addTimeAxis_data();
    void addTimeAxis();

    void addValueAxis_data();
    void addValueAxis();

    /*void clone_data();
    void clone();

    void move_data();
    void move();

    void paste_data();
    void paste();*/

    void copyAxes_data();
    void copyAxes();

    void currentColour_data();
    void currentColour();

    void currentFrameChanged_data();
    void currentFrameChanged();

    void colourChanged_data();
    void colourChanged();

private:
    Engine*    iEngine;
    Animation* iAnimation;

    void setupAnimation();
};
}

#endif // LEDTESTS_H
