/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef ANIMATIONTESTS_H
#define ANIMATIONTESTS_H

#include <QObject>
#include <QtTest/QtTest>

class Engine;
class Animation;

namespace Test {

class AnimationTests : public QObject {
    Q_OBJECT

public:
    explicit AnimationTests(QObject *parent = 0);
    
signals:
    
private slots:
     void initTestCase();

     void setupNew();
     void setupNew_data();

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
