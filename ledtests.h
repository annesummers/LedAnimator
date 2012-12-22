/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef LEDTESTS_H
#define LEDTESTS_H

#include <QObject>
#include <QtTest/QtTest>

class Engine;
class Animation;
class Led;

namespace Test {

class LedTests : public QObject
{
    Q_OBJECT
public:
    explicit LedTests(QObject *parent = 0);
    
signals:
    
private slots:
    void initTestCase();

    void constructor_data();
    void constructor();

    void numFramesChanged_data();
    void numFramesChanged();

    void setCurrentColour_data();
    void setCurrentColour();

    void colourChanged_data();
    void colourChanged();

    void select_data();
    void select();

    void cleanupTestCase();

private:
    Engine*    iEngine;
    Animation* iAnimation;

    Led*        iLed;
};
}

#endif // LEDTESTS_H
