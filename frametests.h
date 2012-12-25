/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef FRAMETESTS_H
#define FRAMETESTS_H

#include <QObject>
#include <QtTest/QtTest>

class Animation;

namespace Test {

class FrameTests : public QObject
{
    Q_OBJECT
public:
    explicit FrameTests(QObject *parent = 0);
    
private slots:
    void initTestCase();

    void setColour_data();
    void setColour();

private:
    Animation* iAnimation;
};
}

#endif // FRAMETESTS_H
