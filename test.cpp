/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "animationtests.h"
#include "ledtests.h"
#include "enginetests.h"
#include "ledanimcodectests.h"
#include "leddetailswidgetstests.h"
#include "frametests.h"
#include "colourwidgettests.h"

using namespace Test;

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    EngineTests tc1;
    AnimationTests tc2;
    FrameTests tc3;
    LedTests tc4;
    LedAnimCodecTests tc5;
    ColourWidgetTests tc6;

    return //QTest::qExec(&tc1, argc, argv) ||
           //QTest::qExec(&tc2, argc, argv) ||
            //QTest::qExec(&tc3, argc, argv) ||
            //QTest::qExec(&tc4, argc, argv) ||
            //QTest::qExec(&tc5, argc, argv) ||
            QTest::qExec(&tc6, argc, argv);
}
