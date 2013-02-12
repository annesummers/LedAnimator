/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef LEDGRIDWIDGETTESTS_H
#define LEDGRIDWIDGETTESTS_H

#include <QObject>
#include <QtTest/QtTest>

namespace Test {

class LedGridWidgetTests : public QObject {
    Q_OBJECT

public:
    LedGridWidgetTests(QObject *parent = 0);

private slots:
    void selectSingleLed_data();
    void selectSingleLed();

    void selectMultipleLeds_data();
    void selectMultipleLeds();
};
}

#endif // LEDGRIDWIDGETTESTS_H
