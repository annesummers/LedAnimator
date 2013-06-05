/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "ledsocketinteractiontests.h"

#include "colourwidgettests.h"

#include "animation.h"
#include "ledwidget.h"
#include "socketwidget.h"

#include "constants.h"

using namespace AnimatorTest;

LedSocketInteractionTests::LedSocketInteractionTests(QObject *parent) :
    QObject(parent)
{
}

void LedSocketInteractionTests::copyPasteOneLedToOneSocket_data() {

}

void LedSocketInteractionTests::copyPasteOneLedToOneSocket() {
    Engine* engine = new Engine(this);
    Animation* animation = new Animation(*engine);


    ColourGroupGroupTestWidget* groupGroupWidget = new ColourGroupGroupTestWidget();

    TestLedGridWidget* gridWidget = new TestLedGridWidget(groupGroupWidget, *animation, *groupGroupWidget);

    QList<Position> gridPositions;

    gridPositions.append(Position(0, 0));
    gridPositions.append(Position(0, 1));
    gridPositions.append(Position(1, 1));
    gridPositions.append(Position(1, 2));
    gridPositions.append(Position(2, 2));

    setupAnimation(*animation, gridPositions);

    LedWidget& copyWidget = static_cast<LedWidget&>(gridWidget->widgetAt(Position(0,0)));
    copyWidget.setColour(Qt::magenta);
    QTest::mouseClick(&copyWidget, Qt::LeftButton);
    QTest::keyClick(&copyWidget, Qt::Key_C, Qt::ControlModifier);

    SocketWidget& pasteWidget = static_cast<SocketWidget&>(gridWidget->widgetAt(Position(3,3)));
    QTest::mouseClick(&pasteWidget, Qt::LeftButton);
    QTest::keyClick(&pasteWidget, Qt::Key_V, Qt::ControlModifier);

    LedWidget& pastedWidget = static_cast<LedWidget&>(gridWidget->widgetAt(Position(3,3)));

    for(int i = INITIAL_FRAME; i < animation->numFrames() + INITIAL_FRAME; i++) {
        QCOMPARE(copyWidget.led().frameAt(i).colour(), pastedWidget.led().frameAt(i).colour());
    }

    delete engine;
}

void LedSocketInteractionTests::copyPasteOneLedToOneLed_data() {

}

void LedSocketInteractionTests::copyPasteOneLedToOneLed() {
    Engine* engine = new Engine(this);
    Animation* animation = new Animation(*engine);
    ColourGroupGroupTestWidget* groupGroupWidget = new ColourGroupGroupTestWidget();

    TestLedGridWidget* gridWidget = new TestLedGridWidget(groupGroupWidget, *animation, *groupGroupWidget);

    QList<Position> gridPositions;

    gridPositions.append(Position(0, 0));
    gridPositions.append(Position(0, 1));
    gridPositions.append(Position(1, 1));
    gridPositions.append(Position(1, 2));
    gridPositions.append(Position(2, 2));

    setupAnimation(*animation, gridPositions);

    LedWidget& copyWidget = static_cast<LedWidget&>(gridWidget->widgetAt(Position(0,0)));
    copyWidget.setColour(Qt::magenta);
    QTest::mouseClick(&copyWidget, Qt::LeftButton);
    QTest::keyClick(&copyWidget, Qt::Key_C, Qt::ControlModifier);

    LedWidget& pasteWidget = static_cast<LedWidget&>(gridWidget->widgetAt(Position(1,1)));
    QTest::mouseClick(&pasteWidget, Qt::LeftButton);
    QTest::keyClick(&pasteWidget, Qt::Key_V, Qt::ControlModifier);

    LedWidget& pastedWidget = static_cast<LedWidget&>(gridWidget->widgetAt(Position(1,1)));

    for(int i = INITIAL_FRAME; i < animation->numFrames() + INITIAL_FRAME; i++) {
        QCOMPARE(copyWidget.led().frameAt(i).colour(), pastedWidget.led().frameAt(i).colour());
    }

    delete engine;
}


void LedSocketInteractionTests::copyPasteOneLedToManySockets_data() {

}

void LedSocketInteractionTests::copyPasteOneLedToManySockets() {
    Engine* engine = new Engine(this);
    Animation* animation = new Animation(*engine);
    ColourGroupGroupTestWidget* groupGroupWidget = new ColourGroupGroupTestWidget();

    TestLedGridWidget* gridWidget = new TestLedGridWidget(groupGroupWidget, *animation, *groupGroupWidget);

    QList<Position> gridPositions;

    gridPositions.append(Position(0, 0));
    gridPositions.append(Position(0, 1));
    gridPositions.append(Position(1, 1));
    gridPositions.append(Position(1, 2));
    gridPositions.append(Position(2, 2));

    setupAnimation(*animation, gridPositions);

    LedWidget& copyWidget = static_cast<LedWidget&>(gridWidget->widgetAt(Position(0,0)));
    copyWidget.setColour(Qt::magenta);
    QTest::mouseClick(&copyWidget, Qt::LeftButton);
    QTest::keyClick(&copyWidget, Qt::Key_C, Qt::ControlModifier);

    SocketWidget& pasteWidget = static_cast<SocketWidget&>(gridWidget->widgetAt(Position(2,3)));
    QTest::mouseClick(&pasteWidget, Qt::LeftButton);
    QTest::keyClick(&pasteWidget, Qt::Key_V, Qt::ControlModifier);

    LedWidget& pastedWidget = static_cast<LedWidget&>(gridWidget->widgetAt(Position(2,3)));

    for(int i = INITIAL_FRAME; i < animation->numFrames() + INITIAL_FRAME; i++) {
        QCOMPARE(copyWidget.led().frameAt(i).colour(), pastedWidget.led().frameAt(i).colour());
    }

    SocketWidget& pasteWidget2 = static_cast<SocketWidget&>(gridWidget->widgetAt(Position(3,3)));
    QTest::mouseClick(&pasteWidget2, Qt::LeftButton);
    QTest::keyClick(&pasteWidget2, Qt::Key_V, Qt::ControlModifier);

    LedWidget& pastedWidget2 = static_cast<LedWidget&>(gridWidget->widgetAt(Position(3,3)));

    for(int i = INITIAL_FRAME; i < animation->numFrames() + INITIAL_FRAME; i++) {
        QCOMPARE(copyWidget.led().frameAt(i).colour(), pastedWidget2.led().frameAt(i).colour());
    }

    delete engine;
}

void LedSocketInteractionTests::copyPasteOneLedToManyLeds_data() {

}

void LedSocketInteractionTests::copyPasteOneLedToManyLeds() {
    Engine* engine = new Engine(this);
    Animation* animation = new Animation(*engine);
    ColourGroupGroupTestWidget* groupGroupWidget = new ColourGroupGroupTestWidget();

    TestLedGridWidget* gridWidget = new TestLedGridWidget(groupGroupWidget, *animation, *groupGroupWidget);

    QList<Position> gridPositions;

    gridPositions.append(Position(0, 0));
    gridPositions.append(Position(0, 1));
    gridPositions.append(Position(1, 1));
    gridPositions.append(Position(1, 2));
    gridPositions.append(Position(2, 2));

    setupAnimation(*animation, gridPositions);

    LedWidget& copyWidget = static_cast<LedWidget&>(gridWidget->widgetAt(Position(0,0)));
    copyWidget.setColour(Qt::magenta);
    QTest::mouseClick(&copyWidget, Qt::LeftButton);
    QTest::keyClick(&copyWidget, Qt::Key_C, Qt::ControlModifier);

    LedWidget& pasteWidget = static_cast<LedWidget&>(gridWidget->widgetAt(Position(1,1)));
    QTest::mouseClick(&pasteWidget, Qt::LeftButton);
    QTest::keyClick(&pasteWidget, Qt::Key_V, Qt::ControlModifier);

    LedWidget& pastedWidget = static_cast<LedWidget&>(gridWidget->widgetAt(Position(1,1)));

    for(int i = INITIAL_FRAME; i < animation->numFrames() + INITIAL_FRAME; i++) {
        QCOMPARE(copyWidget.led().frameAt(i).colour(), pastedWidget.led().frameAt(i).colour());
    }

    LedWidget& pasteWidget2 = static_cast<LedWidget&>(gridWidget->widgetAt(Position(1,1)));
    QTest::mouseClick(&pasteWidget2, Qt::LeftButton);
    QTest::keyClick(&pasteWidget2, Qt::Key_V, Qt::ControlModifier);

    LedWidget& pastedWidget2 = static_cast<LedWidget&>(gridWidget->widgetAt(Position(1,1)));

    for(int i = INITIAL_FRAME; i < animation->numFrames() + INITIAL_FRAME; i++) {
        QCOMPARE(copyWidget.led().frameAt(i).colour(), pastedWidget2.led().frameAt(i).colour());
    }

    delete engine;
}

void LedSocketInteractionTests::copyPasteManyLedToSocket_data() {

}

void LedSocketInteractionTests::copyPasteManyLedToSocket() {
    Engine* engine = new Engine(this);
    Animation* animation = new Animation(*engine);
    ColourGroupGroupTestWidget* groupGroupWidget = new ColourGroupGroupTestWidget();

    TestLedGridWidget* gridWidget = new TestLedGridWidget(groupGroupWidget, *animation, *groupGroupWidget);

    QList<Position> gridPositions;

    gridPositions.append(Position(0, 0));
    gridPositions.append(Position(0, 1));
    gridPositions.append(Position(1, 0));
    gridPositions.append(Position(1, 1));

    setupAnimation(*animation, gridPositions);

    LedWidget& copyWidget = static_cast<LedWidget&>(gridWidget->widgetAt(Position(0,0)));
    copyWidget.setColour(Qt::magenta);

    LedWidget& copyWidget2 = static_cast<LedWidget&>(gridWidget->widgetAt(Position(0,1)));
    copyWidget2.setColour(Qt::green);

    LedWidget& copyWidget3 = static_cast<LedWidget&>(gridWidget->widgetAt(Position(1,0)));
    copyWidget3.setColour(Qt::yellow);

    LedWidget& copyWidget4 = static_cast<LedWidget&>(gridWidget->widgetAt(Position(1,1)));
    copyWidget4.setColour(Qt::blue);

    QTest::mouseClick(&copyWidget, Qt::LeftButton);
    QTest::mouseClick(&copyWidget4, Qt::LeftButton, Qt::ShiftModifier);

    QTest::keyClick(&copyWidget, Qt::Key_C, Qt::ControlModifier);

    SocketWidget& pasteWidget = static_cast<SocketWidget&>(gridWidget->widgetAt(Position(2,2)));
    QTest::mouseClick(&pasteWidget, Qt::LeftButton);
    QTest::keyClick(&pasteWidget, Qt::Key_V, Qt::ControlModifier);

    LedWidget& pastedWidget = static_cast<LedWidget&>(gridWidget->widgetAt(Position(2,2)));

    for(int i = INITIAL_FRAME; i < animation->numFrames() + INITIAL_FRAME; i++) {
        QCOMPARE(copyWidget.led().frameAt(i).colour(), pastedWidget.led().frameAt(i).colour());
    }

    LedWidget& pastedWidget2 = static_cast<LedWidget&>(gridWidget->widgetAt(Position(2,3)));

    for(int i = INITIAL_FRAME; i < animation->numFrames() + INITIAL_FRAME; i++) {
        QCOMPARE(copyWidget2.led().frameAt(i).colour(), pastedWidget2.led().frameAt(i).colour());
    }

    LedWidget& pastedWidget3 = static_cast<LedWidget&>(gridWidget->widgetAt(Position(3,2)));

    for(int i = INITIAL_FRAME; i < animation->numFrames() + INITIAL_FRAME; i++) {
        QCOMPARE(copyWidget3.led().frameAt(i).colour(), pastedWidget3.led().frameAt(i).colour());
    }

    LedWidget& pastedWidget4 = static_cast<LedWidget&>(gridWidget->widgetAt(Position(3,3)));

    for(int i = INITIAL_FRAME; i < animation->numFrames() + INITIAL_FRAME; i++) {
        QCOMPARE(copyWidget4.led().frameAt(i).colour(), pastedWidget4.led().frameAt(i).colour());
    }

    delete engine;
}

void LedSocketInteractionTests::copyPasteManyLedToLed_data() {

}

void LedSocketInteractionTests::copyPasteManyLedToLed() {
    Engine* engine = new Engine(this);
    Animation* animation = new Animation(*engine);
    ColourGroupGroupTestWidget* groupGroupWidget = new ColourGroupGroupTestWidget();

    TestLedGridWidget* gridWidget = new TestLedGridWidget(groupGroupWidget, *animation, *groupGroupWidget);

    QList<Position> gridPositions;

    gridPositions.append(Position(0, 0));
    gridPositions.append(Position(0, 1));
    gridPositions.append(Position(1, 0));
    gridPositions.append(Position(1, 1));
    gridPositions.append(Position(2, 2));
    gridPositions.append(Position(3, 2));
    gridPositions.append(Position(2, 3));
    gridPositions.append(Position(3, 3));

    setupAnimation(*animation, gridPositions);

    LedWidget& copyWidget = static_cast<LedWidget&>(gridWidget->widgetAt(Position(0,0)));
    copyWidget.setColour(Qt::magenta);

    LedWidget& copyWidget2 = static_cast<LedWidget&>(gridWidget->widgetAt(Position(0,1)));
    copyWidget2.setColour(Qt::green);

    LedWidget& copyWidget3 = static_cast<LedWidget&>(gridWidget->widgetAt(Position(1,0)));
    copyWidget3.setColour(Qt::yellow);

    LedWidget& copyWidget4 = static_cast<LedWidget&>(gridWidget->widgetAt(Position(1,1)));
    copyWidget4.setColour(Qt::blue);

    QTest::mouseClick(&copyWidget, Qt::LeftButton);
    QTest::mouseClick(&copyWidget4, Qt::LeftButton, Qt::ShiftModifier);

    QTest::keyClick(&copyWidget, Qt::Key_C, Qt::ControlModifier);

    LedWidget& pasteWidget = static_cast<LedWidget&>(gridWidget->widgetAt(Position(2,2)));
    QTest::mouseClick(&pasteWidget, Qt::LeftButton);
    QTest::keyClick(&pasteWidget, Qt::Key_V, Qt::ControlModifier);

    LedWidget& pastedWidget = static_cast<LedWidget&>(gridWidget->widgetAt(Position(2,2)));

    for(int i = INITIAL_FRAME; i < animation->numFrames() + INITIAL_FRAME; i++) {
        QCOMPARE(copyWidget.led().frameAt(i).colour(), pastedWidget.led().frameAt(i).colour());
    }

    LedWidget& pastedWidget2 = static_cast<LedWidget&>(gridWidget->widgetAt(Position(2,3)));

    for(int i = INITIAL_FRAME; i < animation->numFrames() + INITIAL_FRAME; i++) {
        QCOMPARE(copyWidget2.led().frameAt(i).colour(), pastedWidget2.led().frameAt(i).colour());
    }

    LedWidget& pastedWidget3 = static_cast<LedWidget&>(gridWidget->widgetAt(Position(3,2)));

    for(int i = INITIAL_FRAME; i < animation->numFrames() + INITIAL_FRAME; i++) {
        QCOMPARE(copyWidget3.led().frameAt(i).colour(), pastedWidget3.led().frameAt(i).colour());
    }

    LedWidget& pastedWidget4 = static_cast<LedWidget&>(gridWidget->widgetAt(Position(3,3)));

    for(int i = INITIAL_FRAME; i < animation->numFrames() + INITIAL_FRAME; i++) {
        QCOMPARE(copyWidget4.led().frameAt(i).colour(), pastedWidget4.led().frameAt(i).colour());
    }

    delete engine;
}

void LedSocketInteractionTests::copyPasteManyLedToMixed_data() {

}

void LedSocketInteractionTests::copyPasteManyLedToMixed() {
    Engine* engine = new Engine(this);
    Animation* animation = new Animation(*engine);
    ColourGroupGroupTestWidget* groupGroupWidget = new ColourGroupGroupTestWidget();

    TestLedGridWidget* gridWidget = new TestLedGridWidget(groupGroupWidget, *animation, *groupGroupWidget);

    QList<Position> gridPositions;

    gridPositions.append(Position(0, 0));
    gridPositions.append(Position(0, 1));
    gridPositions.append(Position(1, 0));
    gridPositions.append(Position(1, 1));
    gridPositions.append(Position(2, 3));
    gridPositions.append(Position(3, 3));

    setupAnimation(*animation, gridPositions);

    LedWidget& copyWidget = static_cast<LedWidget&>(gridWidget->widgetAt(Position(0,0)));
    copyWidget.setColour(Qt::magenta);

    LedWidget& copyWidget2 = static_cast<LedWidget&>(gridWidget->widgetAt(Position(0,1)));
    copyWidget2.setColour(Qt::green);

    LedWidget& copyWidget3 = static_cast<LedWidget&>(gridWidget->widgetAt(Position(1,0)));
    copyWidget3.setColour(Qt::yellow);

    LedWidget& copyWidget4 = static_cast<LedWidget&>(gridWidget->widgetAt(Position(1,1)));
    copyWidget4.setColour(Qt::blue);

    QTest::mouseClick(&copyWidget, Qt::LeftButton);
    QTest::mouseClick(&copyWidget4, Qt::LeftButton, Qt::ShiftModifier);

    QTest::keyClick(&copyWidget, Qt::Key_C, Qt::ControlModifier);

    SocketWidget& pasteWidget = static_cast<SocketWidget&>(gridWidget->widgetAt(Position(2,2)));
    QTest::mouseClick(&pasteWidget, Qt::LeftButton);
    QTest::keyClick(&pasteWidget, Qt::Key_V, Qt::ControlModifier);

    LedWidget& pastedWidget = static_cast<LedWidget&>(gridWidget->widgetAt(Position(2,2)));

    for(int i = INITIAL_FRAME; i < animation->numFrames() + INITIAL_FRAME; i++) {
        QCOMPARE(copyWidget.led().frameAt(i).colour(), pastedWidget.led().frameAt(i).colour());
    }

    LedWidget& pastedWidget2 = static_cast<LedWidget&>(gridWidget->widgetAt(Position(2,3)));

    for(int i = INITIAL_FRAME; i < animation->numFrames() + INITIAL_FRAME; i++) {
        QCOMPARE(copyWidget2.led().frameAt(i).colour(), pastedWidget2.led().frameAt(i).colour());
    }

    LedWidget& pastedWidget3 = static_cast<LedWidget&>(gridWidget->widgetAt(Position(3,2)));

    for(int i = INITIAL_FRAME; i < animation->numFrames() + INITIAL_FRAME; i++) {
        QCOMPARE(copyWidget3.led().frameAt(i).colour(), pastedWidget3.led().frameAt(i).colour());
    }

    LedWidget& pastedWidget4 = static_cast<LedWidget&>(gridWidget->widgetAt(Position(3,3)));

    for(int i = INITIAL_FRAME; i < animation->numFrames() + INITIAL_FRAME; i++) {
        QCOMPARE(copyWidget4.led().frameAt(i).colour(), pastedWidget4.led().frameAt(i).colour());
    }

    delete engine;
}

void LedSocketInteractionTests::setupAnimation(Animation& animation, QList<Position> gridPositions) {
    QList<int> positions;

    int numRows = DEFAULT_NUM_ROWS;
    int numColumns = DEFAULT_NUM_COLUMNS;

    for(int i = 0; i < numRows * numColumns; i++) {
        positions.append(INVALID);
    }

    int numLeds = gridPositions.count();

    for(int i = 0; i < numLeds; i++) {
        positions.replace((gridPositions.at(i).row()*numColumns) + gridPositions.at(i).column(), i + INITIAL_LED);
    }

    animation.setupNew(numRows, numColumns, DEFAULT_NUM_FRAMES, DEFAULT_FRAME_FREQUENCY, numLeds, positions);
}
