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

     void setupNewEmpty();
     void setupNewEmpty_data();

     void setupNewPopulated();
     void setupNewPopulated_data();

     void setCurrentFrame_data();
     void setCurrentFrame();

     void ledAt_data();
     void ledAt();

     void play();
     void stop();

     void cleanupTestCase();

private :
     Engine*    iEngine;
     Animation* iAnimation;
};
}

#endif // ANIMATIONTESTS_H
