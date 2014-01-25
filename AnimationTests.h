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

     void addNewLed_data();
     void addNewLed();

     void newPopulated();
     void newPopulated_data();

     void newPopulatedReplace();
     void newPopulatedReplace_data();

     void newEmptyReplace();
     void newEmptyReplace_data();

     void ledAt_data();
     void ledAt();

     void deleteLed_data();
     void deleteLed();

     // led functions

     void moveLed_data();
     void moveLed();

     void cloneLed_data();
     void cloneLed();

     void cutAndPasteLed_data();
     void cutAndPasteLed();

     void renumberLed_data();
     void renumberLed();

     // axes

     void addValueAxis_data();
     void addValueAxis();

     void addTimeAxis_data();
     void addTimeAxis();

     void currentTimeAxis_data();
     void currentTimeAxis();

     void switchTimeAxis_data();
     void switchTimeAxis();

     // functions

     void addFunction_data();
     void addFunction();

     void cleanupTestCase();

private :
     Engine*    iEngine;
     Animation* iAnimation;
};
}

#endif // ANIMATIONTESTS_H
