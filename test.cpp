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
//#include "selectablewidgettests.h"
//#include "colourwidgettests.h"
//#include "ledsocketinteractiontests.h"

using namespace AnimatorTest;

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    EngineTests tc1;
    AnimationTests tc2;
    FrameTests tc3;
    LedTests tc4;
    TimeAxisTests tc5;
    ValueAxisTests tc6;
    AxisDataTests tc7;
    //LedAnimCodecTests tc5;
    //SelectableWidgetTests tc6;
    //ColourWidgetTests tc7;
    //LedSocketInteractionTests tc8;

    return QTest::qExec(&tc1, argc, argv) ||
           QTest::qExec(&tc2, argc, argv) ||
            QTest::qExec(&tc3, argc, argv) ||
            QTest::qExec(&tc4, argc, argv) ||
            QTest::qExec(&tc5, argc, argv) ||
            QTest::qExec(&tc6, argc, argv) ||
            QTest::qExec(&tc7, argc, argv);
}
