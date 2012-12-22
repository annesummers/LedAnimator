#ifndef FRAMETESTS_H
#define FRAMETESTS_H

#include <QObject>
#include <QtTest/QtTest>

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
};
}

#endif // FRAMETESTS_H
