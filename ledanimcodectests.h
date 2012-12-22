#ifndef LEDANIMCODECTESTS_H
#define LEDANIMCODECTESTS_H

#include <QObject>
#include <QtTest/QtTest>

namespace Test {

class LedAnimCodecTests : public QObject
{
    Q_OBJECT
public:
    explicit LedAnimCodecTests(QObject *parent = 0);
    
private slots:
    void initTestCase();

    void readAnimation_data();
    void readAnimation();

    void writeAnimation_data();
    void writeAnimation();
};
}

#endif // LEDANIMCODECTESTS_H
