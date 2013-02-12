/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef FRAMEWIDGETTESTS_H
#define FRAMEWIDGETTESTS_H

#include <QObject>
#include <QtTest/QtTest>

namespace Test {

class FrameWidgetTests : public QObject
{
    Q_OBJECT
public:
    explicit FrameWidgetTests(QObject *parent = 0);
    
private slots:
    //void initTestCase();

    // TODO all event driven
};
}

#endif // FRAMEWIDGETTESTS_H
