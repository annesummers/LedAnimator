#ifndef LEDWIDGETTESTS_H
#define LEDWIDGETTESTS_H

#include <QObject>
#include <QtTest/QtTest>

namespace Test {

class LedWidgetTests : public QObject
{
    Q_OBJECT
public:
    explicit LedWidgetTests(QObject *parent = 0);
    
private slots:

    // TODO all event driven
    
};
}

#endif // LEDWIDGETTESTS_H
