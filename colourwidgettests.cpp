/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "colourwidgettests.h"

#include <QMap>
#include <QString>

#include "fadecalculator.h"
#include "Animation.h"

#include "exceptions.h"
#include "constants.h"

#include "selectablegroupgroupwidget.h"

using namespace AnimatorTest;
using namespace Exception;

ColourGroupTestWidget::ColourGroupTestWidget(QWidget *parent, Animation& animation, int maxRow, int maxColumn,
                                             ColourGroupGroupWidget &groupGroupWidget, int groupNumber) :
    ColourGroupWidget(parent, maxRow, maxColumn, groupGroupWidget, groupNumber) {

    // TODO yuck this is horrilbe
    int number = 0;
    iWidgetArray = new QVector<WidgetVector*>(maxRow);
    for(int i = 0; i < maxRow; i++) {
        (*iWidgetArray)[i] = new WidgetVector(maxColumn);
        for(int j = 0; j < maxColumn; j++) {
            WidgetVector& row = *((*iWidgetArray)[i]);
            row[j] =  new ColourTestWidget(this, *this, *(new ColourTestObject(this, animation, number++)));
        }
    }
}

SelectableWidget& ColourGroupTestWidget::widgetAt(Position position) {
    return *((*(*iWidgetArray)[position.row()])[position.column()]);
}

Position ColourGroupTestWidget::widgetPosition(SelectableWidget& widget) {
    for(int i = 0; i < iWidgetArray->count(); i++) {
        WidgetVector& searchRow = *((*iWidgetArray)[i]);

        int index = searchRow.indexOf(&widget);
        if(index  != INVALID) {
            return Position(i, index);
        }
    }

    return Position();
}

bool ColourGroupTestWidget::validKeyPress(Qt::Key key) {
    return (key & Qt::Key_Right) == Qt::Key_Right ||
           (key & Qt::Key_Left) == Qt::Key_Left ||
           (key & Qt::Key_Up) == Qt::Key_Up ||
           (key & Qt::Key_Down) == Qt::Key_Down;
}

void ColourGroupTestWidget::cloneItem(int fromGroup, Position fromPosition, Position toPosition) {
    ColourTestWidget* fromWidget;

    if(fromGroup == iGroupNumber) {
        fromWidget = static_cast<ColourTestWidget*>(&widgetAt(fromPosition));
    } else {
        ColourGroupTestWidget& group = static_cast<ColourGroupTestWidget&>(iGroupGroup.group(fromGroup));
        fromWidget = static_cast<ColourTestWidget*>(&group.widgetAt(fromPosition));
    }

    ColourTestWidget& toWidget = static_cast<ColourTestWidget&>(widgetAt(toPosition));

    toWidget.setColour(fromWidget->colour());
}

ColourWidgetTests::ColourWidgetTests(QObject *parent) :
    SelectableWidgetTestBase(parent),
    iColourDialog(NULL){

    iEngine = new Engine();
    iAnimation = new Animation(*iEngine);
}

void ColourWidgetTests::fade_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<Position>("startPoint");
    QTest::addColumn<Position>("endPoint");
    QTest::addColumn<QColor>("startColour");
    QTest::addColumn<QColor>("endColour");
    QTest::addColumn<IntList>("fadeSpread");
    QTest::addColumn<PositionList>("fadePoints");

    int maxRow = 3;
    int maxColumn = 3;

    IntList fadeData;
    PositionList fadePoints;

    fadeData.clear();
    fadeData.append(1);
    fadeData.append(2);
    fadeData.append(2);
    fadeData.append(1);

    fadePoints.clear();
    fadePoints.append(Position(0,0));
    fadePoints.append(Position(0,1));
    fadePoints.append(Position(1,0));
    fadePoints.append(Position(1,1));
    fadePoints.append(Position(2,0));
    fadePoints.append(Position(2,1));

    QTest::newRow("top left bottom right 3 x 2") << maxRow
                                               << maxColumn
                                               << Position(0,0)
                                               << Position(2,1)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadePoints.clear();
    fadePoints.append(Position(2,1));
    fadePoints.append(Position(2,0));
    fadePoints.append(Position(1,1));
    fadePoints.append(Position(1,0));
    fadePoints.append(Position(0,1));
    fadePoints.append(Position(0,0));

    QTest::newRow("bottom right top left 3 x 2") << maxRow
                                               << maxColumn
                                               << Position(2,1)
                                               << Position(0,0)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadePoints.clear();
    fadePoints.append(Position(0,1));
    fadePoints.append(Position(0,0));
    fadePoints.append(Position(1,1));
    fadePoints.append(Position(1,0));
    fadePoints.append(Position(2,1));
    fadePoints.append(Position(2,0));

    QTest::newRow("top right bottom left 3 x 2") << maxRow
                                               << maxColumn
                                               << Position(0,1)
                                               << Position(2,0)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadePoints.clear();
    fadePoints.append(Position(2,0));
    fadePoints.append(Position(1,0));
    fadePoints.append(Position(2,1));
    fadePoints.append(Position(0,0));
    fadePoints.append(Position(1,1));
    fadePoints.append(Position(0,1));

    QTest::newRow("bottom left top right 3 x 2") << maxRow
                                               << maxColumn
                                               << Position(2,0)
                                               << Position(0,1)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadeData.clear();
    fadeData.append(1);
    fadeData.append(2);
    fadeData.append(2);
    fadeData.append(1);

    fadePoints.clear();
    fadePoints.append(Position(0,0));
    fadePoints.append(Position(1,0));
    fadePoints.append(Position(0,1));
    fadePoints.append(Position(2,0));
    fadePoints.append(Position(1,1));
    fadePoints.append(Position(2,1));

    QTest::newRow("top left bottom right 2 x 3") << maxRow
                                               << maxColumn
                                               << Position(0,0)
                                               << Position(2,1)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadePoints.clear();
    fadePoints.append(Position(2,1));
    fadePoints.append(Position(2,0));
    fadePoints.append(Position(1,1));
    fadePoints.append(Position(1,0));
    fadePoints.append(Position(0,1));
    fadePoints.append(Position(0,0));

    QTest::newRow("bottom right top left 2 x 3") << maxRow
                                               << maxColumn
                                               << Position(2,1)
                                               << Position(0,0)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadePoints.clear();
    fadePoints.append(Position(2,0));
    fadePoints.append(Position(1,0));
    fadePoints.append(Position(2,1));
    fadePoints.append(Position(0,0));
    fadePoints.append(Position(1,1));
    fadePoints.append(Position(0,1));

    QTest::newRow("top right bottom left 2 x 3") << maxRow
                                               << maxColumn
                                               << Position(2,0)
                                               << Position(0,1)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;


    fadePoints.clear();
    fadePoints.append(Position(0,1));
    fadePoints.append(Position(1,1));
    fadePoints.append(Position(0,0));
    fadePoints.append(Position(2,1));
    fadePoints.append(Position(1,0));
    fadePoints.append(Position(2,0));

    QTest::newRow("bottom left top right 2 x 3") << maxRow
                                               << maxColumn
                                               << Position(0,1)
                                               << Position(2,0)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadeData.clear();
    fadeData.append(1);
    fadeData.append(2);
    fadeData.append(3);
    fadeData.append(2);
    fadeData.append(1);

    fadePoints.clear();
    fadePoints.append(Position(0,0));
    fadePoints.append(Position(0,1));
    fadePoints.append(Position(1,0));
    fadePoints.append(Position(2,0));
    fadePoints.append(Position(1,1));
    fadePoints.append(Position(0,2));
    fadePoints.append(Position(2,1));
    fadePoints.append(Position(1,2));
    fadePoints.append(Position(2,2));

    QTest::newRow("top left bottom right 3 x 3") << maxRow
                                               << maxColumn
                                               << Position(0,0)
                                               << Position(2,2)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadePoints.clear();
    fadePoints.append(Position(2,2));
    fadePoints.append(Position(2,1));
    fadePoints.append(Position(1,2));
    fadePoints.append(Position(2,0));
    fadePoints.append(Position(1,1));
    fadePoints.append(Position(0,2));
    fadePoints.append(Position(1,0));
    fadePoints.append(Position(0,1));
    fadePoints.append(Position(0,0));

    QTest::newRow("bottom right top left 3 x 3") << maxRow
                                               << maxColumn
                                               << Position(2,2)
                                               << Position(0,0)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadePoints.clear();
    fadePoints.append(Position(2,0));
    fadePoints.append(Position(2,1));
    fadePoints.append(Position(1,0));
    fadePoints.append(Position(2,2));
    fadePoints.append(Position(1,1));
    fadePoints.append(Position(0,0));
    fadePoints.append(Position(1,2));
    fadePoints.append(Position(0,1));
    fadePoints.append(Position(0,2));

    QTest::newRow("top right bottom left 3 x 3") << maxRow
                                               << maxColumn
                                               << Position(2,0)
                                               << Position(0,2)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadePoints.clear();
    fadePoints.append(Position(0,2));
    fadePoints.append(Position(0,1));
    fadePoints.append(Position(1,2));
    fadePoints.append(Position(0,0));
    fadePoints.append(Position(1,1));
    fadePoints.append(Position(2,2));
    fadePoints.append(Position(1,0));
    fadePoints.append(Position(1,2));
    fadePoints.append(Position(2,0));

    QTest::newRow("bottom left top right 3 x 3") << maxRow
                                               << maxColumn
                                               << Position(0,2)
                                               << Position(2,0)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadeData.clear();
    fadeData.append(1);
    fadeData.append(2);
    fadeData.append(1);

    fadePoints.clear();
    fadePoints.append(Position(1,1));
    fadePoints.append(Position(1,2));
    fadePoints.append(Position(2,1));
    fadePoints.append(Position(2,2));

    QTest::newRow("top left bottom right 2 x 2") << maxRow
                                               << maxColumn
                                               << Position(1,1)
                                               << Position(2,2)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadePoints.clear();
    fadePoints.append(Position(2,2));
    fadePoints.append(Position(2,1));
    fadePoints.append(Position(1,2));
    fadePoints.append(Position(1,1));

    QTest::newRow("bottom right top left 2 x 2") << maxRow
                                               << maxColumn
                                               << Position(2,2)
                                               << Position(1,1)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadePoints.clear();
    fadePoints.append(Position(2,1));
    fadePoints.append(Position(1,1));
    fadePoints.append(Position(2,2));
    fadePoints.append(Position(1,2));

    QTest::newRow("top right bottom left 2 x 2") << maxRow
                                               << maxColumn
                                               << Position(2,1)
                                               << Position(1,2)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadePoints.clear();
    fadePoints.append(Position(1,2));
    fadePoints.append(Position(1,1));
    fadePoints.append(Position(2,2));
    fadePoints.append(Position(2,1));

    QTest::newRow("bottom left top right 2 x 2") << maxRow
                                               << maxColumn
                                               << Position(1,2)
                                               << Position(2,1)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;


    fadeData.clear();
    fadeData.append(1);
    fadeData.append(1);
    fadeData.append(1);

    fadePoints.clear();
    fadePoints.append(Position(0,0));
    fadePoints.append(Position(0,1));
    fadePoints.append(Position(0,2));

    QTest::newRow("top bottom first column 3 x 1") << maxRow
                                               << maxColumn
                                               << Position(0,0)
                                               << Position(0,2)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;


    fadePoints.clear();
    fadePoints.append(Position(0,2));
    fadePoints.append(Position(0,1));
    fadePoints.append(Position(0,0));

    QTest::newRow("bottom top first column 3 x 1") << maxRow
                                               << maxColumn
                                               << Position(0,2)
                                               << Position(0,0)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;


    fadePoints.clear();
    fadePoints.append(Position(1,0));
    fadePoints.append(Position(1,1));
    fadePoints.append(Position(1,2));

    QTest::newRow("top bottom middle column 3 x 1") << maxRow
                                               << maxColumn
                                               << Position(1,0)
                                               << Position(1,2)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;


    fadePoints.clear();
    fadePoints.append(Position(1,2));
    fadePoints.append(Position(1,1));
    fadePoints.append(Position(1,0));

    QTest::newRow("bottom top middle column 3 x 1") << maxRow
                                               << maxColumn
                                               << Position(1,2)
                                               << Position(1,0)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;


    fadePoints.clear();
    fadePoints.append(Position(2,0));
    fadePoints.append(Position(2,1));
    fadePoints.append(Position(2,2));

    QTest::newRow("top bottom last column 3 x 1") << maxRow
                                               << maxColumn
                                               << Position(2,0)
                                               << Position(2,2)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;


    fadePoints.clear();
    fadePoints.append(Position(2,2));
    fadePoints.append(Position(2,1));
    fadePoints.append(Position(2,0));

    QTest::newRow("bottom top last column 3 x 1") << maxRow
                                               << maxColumn
                                               << Position(2,2)
                                               << Position(2,0)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadePoints.clear();
    fadePoints.append(Position(0,0));
    fadePoints.append(Position(1,0));
    fadePoints.append(Position(2,0));

    QTest::newRow("top bottom first row 1 x 3") << maxRow
                                               << maxColumn
                                               << Position(0,0)
                                               << Position(2,0)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;


    fadePoints.clear();
    fadePoints.append(Position(2,0));
    fadePoints.append(Position(1,0));
    fadePoints.append(Position(0,0));

    QTest::newRow("bottom top first row 1 x 3") << maxRow
                                               << maxColumn
                                               << Position(2,0)
                                               << Position(0,0)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;


    fadePoints.clear();
    fadePoints.append(Position(0,1));
    fadePoints.append(Position(1,1));
    fadePoints.append(Position(2,1));

    QTest::newRow("top bottom middle row 1 x 3") << maxRow
                                               << maxColumn
                                               << Position(0,1)
                                               << Position(2,1)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;


    fadePoints.clear();
    fadePoints.append(Position(2,1));
    fadePoints.append(Position(1,1));
    fadePoints.append(Position(0,1));

    QTest::newRow("bottom top middle row 1 x 3") << maxRow
                                               << maxColumn
                                               << Position(2,1)
                                               << Position(0,1)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;


    fadePoints.clear();
    fadePoints.append(Position(0,2));
    fadePoints.append(Position(1,2));
    fadePoints.append(Position(2,2));

    QTest::newRow("top bottom last row 1 x 3") << maxRow
                                               << maxColumn
                                               << Position(0,2)
                                               << Position(2,2)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;


    fadePoints.clear();
    fadePoints.append(Position(2,2));
    fadePoints.append(Position(1,2));
    fadePoints.append(Position(0,2));

    QTest::newRow("bottom top last row 1 x 3") << maxRow
                                               << maxColumn
                                               << Position(2,2)
                                               << Position(0,2)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

}

void ColourWidgetTests::fade() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(Position, startPoint);
    QFETCH(Position, endPoint);
    QFETCH(QColor, startColour);
    QFETCH(QColor, endColour);
    QFETCH(IntList, fadeSpread);
    QFETCH(PositionList, fadePoints);

    try {
        iFadeData = new FadeData();

        int fadeLine;
        foreach(fadeLine, fadeSpread) {
            iFadeData->fadeSpread.append(fadeLine);
        }

        Position fadePoint;
        foreach(fadePoint, fadePoints) {
            iFadeData->fadePoints.append(fadePoint);
        }

        ColourGroupGroupTestWidget* groupGroupWidget =  new ColourGroupGroupTestWidget(NULL, *iEngine);
        iGroupWidget = new ColourGroupTestWidget(NULL, *iAnimation, maxRow, maxColumn, *groupGroupWidget, 0);

        iGroupWidget->selectOne(iGroupWidget->widgetAt(startPoint));
        iGroupWidget->setColour(startColour);

        iGroupWidget->selectArea(iGroupWidget->widgetAt(endPoint), false);

        try {
            iGroupWidget->setupFade(endColour);
        } catch(IllegalStateException e) {

        }

        connect(iGroupWidget->fadeCalculator(), SIGNAL(colourCalculated(QColor)), this, SLOT(fadeColourCalculated(QColor)));

        iAsyncTimeoutTimer.setInterval(1000);
        iAsyncTimeoutTimer.setSingleShot(true);

        iAsyncEventLoop.connect(iGroupWidget->fadeCalculator(), SIGNAL(fadeComplete()), this, SLOT(fadeComplete()));
        iAsyncEventLoop.connect(&iAsyncTimeoutTimer, SIGNAL(timeout()), this, SLOT(fadeTimeout()));

        iGroupWidget->startFade();

        iAsyncEventLoop.exec();
    } catch(AnimatorException e) {
        qDebug("exception");
    }
}

void ColourWidgetTests::fadeColourCalculated(QColor colour) {
    iFadeData->fadeColours.append(colour);
}

void ColourWidgetTests::fadeComplete() {
   // qDebug("fadeComplete");
    int pointCounter = 0;

    for(int i = 0; i < iFadeData->fadeSpread.count(); i++) {
        int lineCount = iFadeData->fadeSpread[i];
        QColor colour = iFadeData->fadeColours[i];

        for(int j = pointCounter; j < pointCounter + lineCount; j++) {
            Position point = iFadeData->fadePoints[pointCounter];
            QCOMPARE(static_cast<ColourWidget&>(iGroupWidget->widgetAt(point)).colour(), colour);
        }

        pointCounter += lineCount;
    }

    delete iFadeData;

    iAsyncTimeoutTimer.stop();
    iAsyncEventLoop.exit();
}

void ColourWidgetTests::fadeTimeout() {
    //QCOMPARE(true, false);
}

void ColourWidgetTests::doubleClickOne_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<Position>("selectedPoint");

    int maxRows = 2;
    int maxColumns = 3;

    QTest::newRow("double click one") << maxRows
                                    << maxColumns
                                    << Position(0,0);

}

void ColourWidgetTests::doubleClickOne() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(Position, selectedPoint);

    ColourGroupGroupTestWidget* groupGroup = new ColourGroupGroupTestWidget(NULL, *iEngine);
    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, *iAnimation, maxRow, maxColumn, *groupGroup, 0);

    doubleClickWidgetAndDismissDialog(*groupWidget, selectedPoint);

    QList<Position> selPoint;
    selPoint.append(selectedPoint);

    compareAreaPoints(groupWidget, selPoint);

    delete groupWidget;
}

void ColourWidgetTests::doubleClickOneClickOne_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<Position>("firstPoint");
    QTest::addColumn<Position>("secondPoint");
    QTest::addColumn<Position>("selectedPoint");

    int maxRows = 2;
    int maxColumns = 3;

    QTest::newRow("double click one click same") << maxRows
                                << maxColumns
                                << Position(0,0)
                                << Position(0,0)
                                << Position(0,0);

    QTest::newRow("double click one click different") << maxRows
                                << maxColumns
                                << Position(0,0)
                                << Position(0,1)
                                << Position(0,1);
}

void ColourWidgetTests::doubleClickOneClickOne() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(Position, firstPoint);
    QFETCH(Position, secondPoint);
    QFETCH(Position, selectedPoint);

    ColourGroupGroupTestWidget* groupGroup = new ColourGroupGroupTestWidget(NULL, *iEngine);
    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, *iAnimation, maxRow, maxColumn, *groupGroup, 0);

    doubleClickWidgetAndDismissDialog(*groupWidget, firstPoint);

    QTest::mouseClick(&(groupWidget->widgetAt(secondPoint)), Qt::LeftButton);

    QList<Position> selPoint;
    if(selectedPoint.isValid()) {
        selPoint.append(selectedPoint);
    }

    compareAreaPoints(groupWidget, selPoint);

    delete groupWidget;
}

void ColourWidgetTests::clickOneDoubleClickOne_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<Position>("firstPoint");
    QTest::addColumn<Position>("secondPoint");
    QTest::addColumn<Position>("selectedPoint");

    int maxRows = 2;
    int maxColumns = 3;

    QTest::newRow("click one double click same") << maxRows
                                << maxColumns
                                << Position(0,0)
                                << Position(0,0)
                                << Position(0,0);

    QTest::newRow("click one double click different") << maxRows
                                << maxColumns
                                << Position(0,0)
                                << Position(0,1)
                                << Position(0,1);
}

void ColourWidgetTests::clickOneDoubleClickOne() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(Position, firstPoint);
    QFETCH(Position, secondPoint);
    QFETCH(Position, selectedPoint);

    ColourGroupGroupTestWidget* groupGroup = new ColourGroupGroupTestWidget(NULL, *iEngine);
    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, *iAnimation, maxRow, maxColumn, *groupGroup, 0);

    QTest::mouseClick(&(groupWidget->widgetAt(firstPoint)), Qt::LeftButton);

    doubleClickWidgetAndDismissDialog(*groupWidget, secondPoint);

    QList<Position> selPoint;
    selPoint.append(selectedPoint);

    compareAreaPoints(groupWidget, selPoint);

    delete groupWidget;
}

void ColourWidgetTests::doubleClickOneDoubleClickOne_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<Position>("firstPoint");
    QTest::addColumn<Position>("secondPoint");
    QTest::addColumn<Position>("selectedPoint");

    int maxRows;
    int maxColumns;

    maxRows = 2;
    maxColumns = 3;

    QTest::newRow("double click one double click same") << maxRows
                                << maxColumns
                                << Position(0,0)
                                << Position(0,0)
                                << Position(0,0);

    QTest::newRow("double click one double click different") << maxRows
                                << maxColumns
                                << Position(0,0)
                                << Position(0,1)
                                << Position(0,1);
}

void ColourWidgetTests::doubleClickOneDoubleClickOne() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(Position, firstPoint);
    QFETCH(Position, secondPoint);
    QFETCH(Position, selectedPoint);

    ColourGroupGroupTestWidget* groupGroup = new ColourGroupGroupTestWidget(NULL, *iEngine);
    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, *iAnimation, maxRow, maxColumn, *groupGroup, 0);

    doubleClickWidgetAndDismissDialog(*groupWidget, firstPoint);
    doubleClickWidgetAndDismissDialog(*groupWidget, secondPoint);

    QList<Position> selPoint;
    selPoint.append(selectedPoint);

    compareAreaPoints(groupWidget, selPoint);

    delete groupWidget;
}

void ColourWidgetTests::copyPasteOneInternal_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<Position>("copyPoint");
    QTest::addColumn<Position>("pastePoint");
    QTest::addColumn<QColor>("copyColour");

    int maxRows = 3;
    int maxColumns = 3;

    QTest::newRow("copy one paste different") << maxRows
                                 << maxColumns
                                 << Position(0,0)
                                 << Position(0,1)
                                 << QColor(Qt::magenta);

    QTest::newRow("copy one paste same") << maxRows
                                 << maxColumns
                                 << Position(0,0)
                                 << Position(0,0)
                                 << QColor(Qt::magenta);
}

void ColourWidgetTests::copyPasteOneInternal() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(Position, copyPoint);
    QFETCH(Position, pastePoint);
    QFETCH(QColor, copyColour);

    ColourGroupGroupTestWidget* groupGroup = new ColourGroupGroupTestWidget(NULL, *iEngine);
    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, *iAnimation, maxRow, maxColumn, *groupGroup, 0);

    ColourTestWidget& copyWidget = (ColourTestWidget&)groupWidget->widgetAt(copyPoint);
    copyWidget.setColour(copyColour);

    groupWidget->selectOne(copyWidget);
    QTest::keyClick(&copyWidget, Qt::Key_C, Qt::ControlModifier);

    ColourTestWidget& pasteWidget = (ColourTestWidget&)groupWidget->widgetAt(pastePoint);

    groupWidget->selectOne(pasteWidget);
    QTest::keyClick(&pasteWidget, Qt::Key_V, Qt::ControlModifier);

    QCOMPARE(copyWidget.colour(), pasteWidget.colour());

    QList<Position> selectedPoints;
    selectedPoints.append(pastePoint);

    compareAreaPoints(groupWidget, selectedPoints);

    delete groupWidget;
}

void ColourWidgetTests::copyPasteManyInternal_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<Position>("copyFirstPoint");
    QTest::addColumn<Position>("copySecondPoint");
    QTest::addColumn<Position>("pastePoint");
    QTest::addColumn<Position>("pasteSecondPoint");
    QTest::addColumn<QColor>("copyColour");

    int maxRows = 4;
    int maxColumns = 4;

    QTest::newRow("copy many paste different") << maxRows
                                 << maxColumns
                                 << Position(0,0)
                                 << Position(1,1)
                                 << Position(2,2)
                                 << Position(3,3)
                                 << QColor(Qt::magenta);

    QTest::newRow("copy many paste same") << maxRows
                                 << maxColumns
                                 << Position(0,0)
                                 << Position(1,1)
                                 << Position(0,0)
                                 << Position(1,1)
                                 << QColor(Qt::magenta);

    QTest::newRow("copy many paste overlap") << maxRows
                                 << maxColumns
                                 << Position(0,0)
                                 << Position(1,1)
                                 << Position(1,1)
                                 << Position(2,2)
                                 << QColor(Qt::magenta);
}

void ColourWidgetTests::copyPasteManyInternal() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(Position, copyFirstPoint);
    QFETCH(Position, copySecondPoint);
    QFETCH(Position, pastePoint);
    QFETCH(Position, pasteSecondPoint);
    QFETCH(QColor, copyColour);

    ColourGroupGroupTestWidget* groupGroup = new ColourGroupGroupTestWidget(NULL, *iEngine);
    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, *iAnimation, maxRow, maxColumn, *groupGroup, 0);

    ColourWidget& copyFirstWidget = static_cast<ColourWidget&>(groupWidget->widgetAt(copyFirstPoint));

    groupWidget->selectOne(copyFirstWidget);
    groupWidget->selectArea(groupWidget->widgetAt(copySecondPoint), false);
    groupWidget->setColour(copyColour);

    QTest::keyPress(&copyFirstWidget, Qt::Key_C, Qt::ControlModifier);

    ColourWidget& pastePointWidget = static_cast<ColourWidget&>(groupWidget->widgetAt(pastePoint));

    groupWidget->selectOne(pastePointWidget);
    QTest::keyPress(&pastePointWidget, Qt::Key_V, Qt::ControlModifier);

    QList<Position> testPoints;
    calculateAreaPoints(testPoints, pastePoint, pasteSecondPoint);

    for(int i = 0; i < testPoints.count(); i++) {
        QCOMPARE(static_cast<ColourWidget&>(groupWidget->widgetAt(testPoints[i])).colour(), copyColour);
    }

    compareAreaPoints(groupWidget, testPoints);

    delete groupWidget;
}

void ColourWidgetTests::copyPasteOneExternal_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<Position>("copyPoint");
    QTest::addColumn<Position>("pastePoint");
    QTest::addColumn<QColor>("copyColour");

    int maxRows = 3;
    int maxColumns = 3;

    QTest::newRow("copy one paste different") << maxRows
                                 << maxColumns
                                 << Position(0,0)
                                 << Position(0,1)
                                 << QColor(Qt::magenta);

    QTest::newRow("copy one paste same") << maxRows
                                 << maxColumns
                                 << Position(0,0)
                                 << Position(0,0)
                                 << QColor(Qt::magenta);
}

void ColourWidgetTests::copyPasteOneExternal() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(Position, copyPoint);
    QFETCH(Position, pastePoint);
    QFETCH(QColor, copyColour);

    ColourGroupGroupTestWidget* groupGroupWidget = new ColourGroupGroupTestWidget(NULL, *iEngine);
    ColourGroupTestWidget* groupWidget1 = new ColourGroupTestWidget(NULL, *iAnimation, maxRow, maxColumn, *groupGroupWidget, 0);
    ColourGroupTestWidget* groupWidget2 = new ColourGroupTestWidget(NULL, *iAnimation, maxRow, maxColumn, *groupGroupWidget, 1);

    ColourTestWidget& copyWidget = (ColourTestWidget&)groupWidget1->widgetAt(copyPoint);
    copyWidget.setColour(copyColour);

    groupWidget1->selectOne(copyWidget);
    QTest::keyClick(&copyWidget, Qt::Key_C, Qt::ControlModifier);

    ColourTestWidget& pasteWidget = (ColourTestWidget&)groupWidget2->widgetAt(pastePoint);

    groupWidget2->selectOne(pasteWidget);
    QTest::keyClick(&pasteWidget, Qt::Key_V, Qt::ControlModifier);

    QCOMPARE(copyWidget.colour(), pasteWidget.colour());

    delete groupWidget1;
    delete groupWidget2;
}

void ColourWidgetTests::copyPasteManyExternal_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<Position>("copyFirstPoint");
    QTest::addColumn<Position>("copySecondPoint");
    QTest::addColumn<Position>("pastePoint");
    QTest::addColumn<Position>("pasteSecondPoint");
    QTest::addColumn<QColor>("copyColour");

    int maxRows = 4;
    int maxColumns = 4;

    QTest::newRow("copy many paste many") << maxRows
                                 << maxColumns
                                 << Position(0,0)
                                 << Position(1,1)
                                 << Position(2,2)
                                 << Position(3,3)
                                 << QColor(Qt::magenta);
}

void ColourWidgetTests::copyPasteManyExternal() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(Position, copyFirstPoint);
    QFETCH(Position, copySecondPoint);
    QFETCH(Position, pastePoint);
    QFETCH(Position, pasteSecondPoint);
    QFETCH(QColor, copyColour);

    ColourGroupGroupTestWidget* groupGroupWidget = new ColourGroupGroupTestWidget(NULL, *iEngine);
    ColourGroupTestWidget* groupWidget1 = new ColourGroupTestWidget(NULL, *iAnimation, maxRow, maxColumn, *groupGroupWidget, 0);
    ColourWidget& copyFirstWidget = static_cast<ColourWidget&>(groupWidget1->widgetAt(copyFirstPoint));

    groupWidget1->selectOne(copyFirstWidget);
    groupWidget1->selectArea(groupWidget1->widgetAt(copySecondPoint), false);
    groupWidget1->setColour(copyColour);

    QTest::keyPress(&copyFirstWidget, Qt::Key_C, Qt::ControlModifier);

    ColourGroupTestWidget* groupWidget2 = new ColourGroupTestWidget(NULL, *iAnimation, maxRow, maxColumn, *groupGroupWidget, 1);

    ColourWidget& pastePointWidget = static_cast<ColourWidget&>(groupWidget2->widgetAt(pastePoint));

    groupWidget2->selectOne(pastePointWidget);
    QTest::keyPress(&pastePointWidget, Qt::Key_V, Qt::ControlModifier);

    QList<Position> selectedPoints;
    calculateAreaPoints(selectedPoints, pastePoint, pasteSecondPoint);

    for(int i = 0; i < selectedPoints.count(); i++) {
        QCOMPARE(static_cast<ColourWidget&>(groupWidget2->widgetAt(selectedPoints[i])).colour(), copyColour);
    }

    compareAreaPoints(groupWidget2, selectedPoints);

    delete groupWidget1;
    delete groupWidget2;
}

void ColourWidgetTests::doubleClickWidgetAndDismissDialog(ColourGroupTestWidget& groupWidget, Position widgetPoint) {
    ColourTestWidget& widget = (ColourTestWidget&)(groupWidget.widgetAt(widgetPoint));

    iColourDialog = &(groupWidget.colourDialog());

    iAsyncTimeoutTimer.setInterval(20);
    iAsyncTimeoutTimer.setSingleShot(true);

    iAsyncEventLoop.connect(&iAsyncTimeoutTimer, SIGNAL(timeout()), this, SLOT(dismissDialog()));
    iAsyncTimeoutTimer.start();

    QTest::mouseDClick(&widget, Qt::LeftButton);

    iAsyncEventLoop.exec();
}

void ColourWidgetTests::dismissDialog() {
    if(iColourDialog != NULL) {
        QTest::keyClick(iColourDialog, Qt::Key_Escape);
        iColourDialog = NULL;

        iAsyncTimeoutTimer.start();
    } else {
        iAsyncEventLoop.exit();
    }
}

bool ColourWidgetTests::waitForSignal(QObject *sender,
                                      const char *signal,
                                      const char* slot,
                                      const char* timeoutSlot,
                                      int timeout) {
    QEventLoop loop;
    QTimer timer;
    timer.setInterval(timeout);
    timer.setSingleShot(true);

    loop.connect(sender, signal, this, slot);
    loop.connect(&timer, SIGNAL(timeout()), this, timeoutSlot);
    timer.start();
    loop.exec();

    return timer.isActive();
}
