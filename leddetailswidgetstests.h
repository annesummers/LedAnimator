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

namespace AnimatorModel {
class Animation;
}

using namespace AnimatorModel;

namespace AnimatorTest {

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
