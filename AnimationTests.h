/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef ANIMATIONTESTS_H
#define ANIMATIONTESTS_H

#include <QObject>
#include <QtTest/QtTest>

class Engine;

namespace AnimatorModel {
class Animation;
}

using namespace AnimatorModel;

namespace AnimatorTest {

class AnimationTests : public QObject {
    Q_OBJECT

public:
    explicit AnimationTests(QObject *parent = 0);
    
signals:
    
private slots:
     void initTestCase();

     void newEmpty();
     void newEmpty_data();

     void newPopulated();
     void newPopulated_data();

     // led functions

     void addNewLed_data();
     void addNewLed();

     void deleteLed_data();
     void deleteLed();

     void moveLed_data();
     void moveLed();

     void cloneLed_data();
     void cloneLed();

     void pasteLed_data();
     void pasteLed();

     void copyLedTimeAxis_data();
     void copyLedTimeAxis();

     void copyLedValueAxis_data();
     void copyLedValueAxis();

     void copyLedCurrentFrame_data();
     void copyLedCurrentFrame();

     void renumberLed_data();
     void renumberLed();

     void ledAt_data();
     void ledAt();

     // axes

     void addValueAxis_data();
     void addValueAxis();

     void addTimeAxis_data();
     void addTimeAxis();

     void axisAt_data();
     void axisAt();

     void timeAxisAt_data();
     void timeAxisAt();

     void currentTimeAxis_data();
     void currentTimeAxis();

     void switchTimeAxis_data();
     void switchTimeAxis();

     // functions

     void addFunction_data();
     void addFunction();

     void functionAt_data();
     void functionAt();

     void cleanupTestCase();

private :
     Engine*    iEngine;
     Animation* iAnimation;
};
}

#endif // ANIMATIONTESTS_H
