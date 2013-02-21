#include "ledsocketinteractiontests.h"

#include "animation.h"
#include "ledwidget.h"
#include "socketwidget.h"

LedSocketInteractionTests::LedSocketInteractionTests(QObject *parent) :
    QObject(parent)
{
}

void LedSocketInteractionTests::copyPasteOneLedToSocket_data() {

}

void LedSocketInteractionTests::copyPasteOneLedToSocket() {
    Engine* engine = new Engine(this);
    Animation* animation = new Animation(*engine);
    TestLedGridWidget* gridWidget = new TestLedGridWidget(NULL, *animation);

    QList<QPoint> gridPositions;

    gridPositions.append(QPoint(0, 0));
    gridPositions.append(QPoint(0, 1));
    gridPositions.append(QPoint(1, 1));
    gridPositions.append(QPoint(1, 2));
    gridPositions.append(QPoint(2, 2));

    setupAnimation(*animation, gridPositions);

    LedWidget& copyWidget = static_cast<LedWidget&>(gridWidget->widgetAt(0,0));
    copyWidget.setColour(Qt::magenta);
    QTest::keyClick(&copyWidget, Qt::Key_C, Qt::ControlModifier);

    SocketWidget& pasteWidget = static_cast<SocketWidget&>(gridWidget->widgetAt(3,3));
    QTest::keyClick(&pasteWidget, Qt::Key_V, Qt::ControlModifier);

    QCOMPARE(copyWidget.colour(), pasteWidget.colour());
}

void LedSocketInteractionTests::copyPasteManyLedToSocket_data() {

}

void LedSocketInteractionTests::copyPasteManyLedToSocket() {

}

void LedSocketInteractionTests::cutPasteOneLedToSocket_data() {

}

void LedSocketInteractionTests::cutPasteOneLedToSocket() {

}

void LedSocketInteractionTests::cutPasteManyLedToSocket_data() {

}

void LedSocketInteractionTests::cutPasteManyLedToSocket() {

}

void LedSocketInteractionTests::setupAnimation(Animation& animation, QList<QPoint> gridPositions) {
    QList<int> positions;

    int numRows = DEFAULT_NUM_ROWS;
    int numColumns = DEFAULT_NUM_COLUMNS;

    for(int i = 0; i < numRows * numColumns; i++) {
        positions.append(INVALID);
    }

    int numLeds = gridPositions.count();

    for(int i = 0; i < numLeds; i++) {
        positions.replace((gridPositions.at(i).y()*numColumns) + gridPositions.at(i).x(), i);
    }

    animation.setupNew(numRows, numColumns, DEFAULT_NUM_FRAMES, DEFAULT_FRAME_FREQUENCY, numLeds, positions);
}
