#ifndef LEDDETAILSWIDGETSTESTS_H
#define LEDDETAILSWIDGETSTESTS_H

#include <QObject>
#include <QtTest/QtTest>

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
};
}

#endif // LEDDETAILSWIDGETSTESTS_H
