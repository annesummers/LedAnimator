/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef FRAMETESTS_H
#define FRAMETESTS_H

#include <QObject>
#include <QtTest/QtTest>

namespace AnimatorModel {
class Animation;
}

using namespace AnimatorModel;

namespace AnimatorTest {

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
