/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef LEDDETAILSWIDGETSTESTS_H
#define LEDDETAILSWIDGETSTESTS_H

#include <QObject>
#include <QtTest/QtTest>

class Engine;
class Animation;

namespace Test {

class LedDetailsWidgetsTests : public QObject
{
    Q_OBJECT
public:
    explicit LedDetailsWidgetsTests(QObject *parent = 0);
    
private slots:
    void initTestCase();

    void listAddLed_data();
    void listAddLed();

private:
    Engine*     iEngine;
    Animation*  iAnimation;

};
}

#endif // LEDDETAILSWIDGETSTESTS_H