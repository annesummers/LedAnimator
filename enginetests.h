/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef ENGINETESTS_H
#define ENGINETESTS_H

#include <QObject>
#include <QtTest/QtTest>

class Animation;
class Engine;
class MainWindow;

namespace Test {

class EngineTests : public QObject
{
    Q_OBJECT
public:
    explicit EngineTests(QObject *parent = 0);

private slots:
    void initTestCase();

    void start();

    void loadAnimation_data();
    void loadAnimation();

    void newAnimation_data();
    void newAnimation();

    void askSaveAnimation_data();
    void askSaveAnimation();

    void cleanupTestCase();

private:
    Engine* iEngine;
    Animation* iAnimation;
    MainWindow* iMainWindow;
};
}

#endif // ENGINETESTS_H
