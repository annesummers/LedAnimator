/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "AnimationTests.h"
#include "LedTests.h"
#include "enginetests.h"
//#include "ledanimcodectests.h"
#include "FrameTests.h"
#include "TimeAxisTests.h"
#include "ValueAxisTests.h"
#include "AxisDataTests.h"
#include "ValueAxisDataTests.h"
#include "selectablewidgettests.h"
//#include "colourwidgettests.h"
//#include "ledsocketinteractiontests.h"

using namespace AnimatorTest;

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    EngineTests engineTests;
    AnimationTests animationTests;
    AxisTests axisTests;
    TimeAxisTests timeAxisTests;
    ValueAxisTests valueAxisTests;
    AxisDataTests axisDataTests;
    ValueAxisDataTests valueAxisDataTests;
    FrameTests frameTests;
    LedTests ledTests;
    //LedAnimCodecTests tc5;
    SelectableWidgetTests selectionTests;
    //ColourWidgetTests tc7;
    //LedSocketInteractionTests tc8;

    return QTest::qExec(&engineTests, argc, argv) ||
           QTest::qExec(&animationTests, argc, argv) ||
            QTest::qExec(&axisTests, argc, argv) ||
            QTest::qExec(&timeAxisTests, argc, argv) ||
            QTest::qExec(&valueAxisTests, argc, argv) ||
            QTest::qExec(&axisDataTests, argc, argv) ||
            QTest::qExec(&valueAxisDataTests, argc, argv)||
            QTest::qExec(&frameTests, argc, argv)||
            QTest::qExec(&ledTests, argc, argv) ||
            QTest::qExec(&selectionTests, argc, argv);
}
