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

    void cleanupTestCase();

private:
    Engine*    iEngine;
    Animation* iAnimation;
};
}

#endif // LEDTESTS_H
