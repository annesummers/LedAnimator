#ifndef ENGINETESTS_H
#define ENGINETESTS_H

#include <QObject>
#include <QtTest/QtTest>

namespace Test {

class EngineTests : public QObject
{
    Q_OBJECT
public:
    explicit EngineTests(QObject *parent = 0);

private slots:
    void initTestCase();

    void start();

    void askSaveAnimation_data();
    void askSaveAnimation();

    void cleanupTestCase();
};
}

#endif // ENGINETESTS_H
