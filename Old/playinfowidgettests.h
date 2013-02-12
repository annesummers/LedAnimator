/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef PLAYINFOWIDGETTESTS_H
#define PLAYINFOWIDGETTESTS_H

#include <QObject>
#include <QtTest/QtTest>

namespace Test {

class PlayInfoWidgetTests : public QObject
{
    Q_OBJECT
public:
    explicit PlayInfoWidgetTests(QObject *parent = 0);
    
private slots:
        //TODO all event driven
};
}

#endif // PLAYINFOWIDGETTESTS_H
