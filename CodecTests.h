/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef LEDANIMCODECTESTS_H
#define LEDANIMCODECTESTS_H

#include <QObject>
#include <QtTest/QtTest>

namespace AnimatorTest {

class LedAnimCodecTests : public QObject
{
    Q_OBJECT
public:
    explicit LedAnimCodecTests(QObject *parent = 0);
    
private slots:
    void initTestCase();

    void readByteArrayAnimation_data();
    void readByteArrayAnimation();

    void writeByteArrayAnimation_data();
    void writeByteArrayAnimation();

    void writeSimpleByteArrayAnimation_data();
    void writeSimpleByteArrayAnimation();

    void writeStringAnimation_data();
    void writeStringAnimation();
};
}

#endif // LEDANIMCODECTESTS_H
