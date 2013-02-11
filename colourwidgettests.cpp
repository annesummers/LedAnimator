#include "colourwidgettests.h"

#include <QMap>
#include <QString>

#include "fadecalculator.h"

#include "exceptions.h"
#include "defaults.h"

#include "colourgroupgroupwidget.h"

using namespace Test;
using namespace Exception;

ColourGroupTestWidget::ColourGroupTestWidget(QWidget *parent, int maxRow, int maxColumn, ColourGroupGroupWidget *groupGroupWidget = 0) :
    ColourGroupWidget(parent, maxRow, maxColumn, groupGroupWidget) {

    // TODO yuck this is horrilbe

    iWidgetArray = new QVector<WidgetVector*>(maxRow);
    for(int i = 0; i < maxRow; i++) {
        (*iWidgetArray)[i] = new WidgetVector(maxColumn);
        for(int j = 0; j < maxColumn; j++) {
            WidgetVector& row = *((*iWidgetArray)[i]);
            row[j] =  new ColourTestWidget(this, *this, *(new SelectableTestObject(this)));
        }
    }
}

ColourWidget& ColourGroupTestWidget::widgetAt(int row, int column) {
    return *((*(*iWidgetArray)[row])[column]);
}

void ColourGroupTestWidget::getWidgetPosition(ColourWidget& widget, int* row, int* column) {
    for(int i = 0; i < iWidgetArray->count(); i++) {
        WidgetVector& searchRow = *((*iWidgetArray)[i]);

        int index = searchRow.indexOf(&widget);
        if(index  != -1) {
            *row = i;
            *column = index;
            return;
        }
    }
}

bool ColourGroupTestWidget::validKeyPress(Qt::Key key) {
    return (key & Qt::Key_Right) != 0 ||
           (key & Qt::Key_Left) != 0 ||
           (key & Qt::Key_Up) != 0 ||
           (key & Qt::Key_Down) != 0;
}

ColourWidgetTests::ColourWidgetTests(QObject *parent) :
    QObject(parent),
    iColourDialog(NULL){
}

void ColourWidgetTests::select_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<PointList>("selectedPoints");

    PointList points;
    int maxRows;
    int maxColumns;

    maxRows = 2;
    maxColumns = 3;

    for(int i = 0; i < maxRows; i++) {
        for(int j = 0; j < maxColumns; j++) {
            points.append(QPoint(j, i));
        }
    }

    QTest::newRow("select all") << maxRows
                                << maxColumns
                                << points;
}

void ColourWidgetTests::select() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(PointList, selectedPoints);

    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    for(int i = 0; i < selectedPoints.count(); i++) {
        groupWidget->toggle(groupWidget->widgetAt(selectedPoints.at(i).y(), selectedPoints.at(i).x()));
    }

    compareAreaPoints(groupWidget, selectedPoints);
}

void ColourWidgetTests::selectOne_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("selectedPoint");

    int maxRows;
    int maxColumns;

    maxRows = 2;
    maxColumns = 3;

    QTest::newRow("select one") << maxRows
                                << maxColumns
                                << QPoint(0,0);
}

void ColourWidgetTests::selectOne() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, selectedPoint);

    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    groupWidget->selectOne(groupWidget->widgetAt(selectedPoint.y(), selectedPoint.x()));

    QList<QPoint> selPoint;
    selPoint.append(selectedPoint);

    compareAreaPoints(groupWidget, selPoint);
}

void ColourWidgetTests::selectArea_data() {
    areaData();
}

void ColourWidgetTests::selectArea() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstSelected);
    QFETCH(QPoint, secondSelected);

    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    groupWidget->selectOne(groupWidget->widgetAt(firstSelected.y(), firstSelected.x()));
    groupWidget->selectArea(groupWidget->widgetAt(secondSelected.y(), secondSelected.x()));

    QList<QPoint> selectedPoints;

    calculateAreaPoints(selectedPoints, firstSelected, secondSelected);
    compareAreaPoints(groupWidget, selectedPoints);

    delete groupWidget;
}

void ColourWidgetTests::selectDirection_data() {
    directionData();
}

void ColourWidgetTests::selectDirection() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstSelected);
    QFETCH(IntList, selectedDirections);
    QFETCH(PointList, selectedPoints);

    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    groupWidget->selectOne(groupWidget->widgetAt(firstSelected.y(), firstSelected.x()));

    int direction;
    foreach(direction, selectedDirections) {
        groupWidget->selectDirection((Qt::Key)direction);
    }

    compareAreaPoints(groupWidget, selectedPoints);

    delete groupWidget;
}

void ColourWidgetTests::selectOneSelectExternal_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("selectedPoint");

    int maxRows = 2;
    int maxColumns = 3;

    QTest::newRow("select one select external") << maxRows
                                                << maxColumns
                                                << QPoint(0,0);

}

void ColourWidgetTests::selectOneSelectExternal() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, selectedPoint);

    ColourGroupGroupWidget* groupGroupWidget = new ColourGroupGroupWidget(NULL);

    ColourGroupTestWidget* groupWidget1 = new ColourGroupTestWidget(NULL, maxRow, maxColumn, groupGroupWidget);
    ColourGroupTestWidget* groupWidget2 = new ColourGroupTestWidget(NULL, maxRow, maxColumn, groupGroupWidget);

    groupWidget1->selectOne(groupWidget1->widgetAt(selectedPoint.y(), selectedPoint.x()));
    groupWidget2->selectOne(groupWidget2->widgetAt(selectedPoint.y(), selectedPoint.x()));

    QCOMPARE(groupWidget1->selectedItems().count(), 0);
    QCOMPARE(groupWidget2->selectedItems().count(), 1);

    bool contains = groupWidget2->selectedItems().contains(&(groupWidget2->widgetAt(selectedPoint.y(), selectedPoint.x())));
    QCOMPARE(contains, true);

    delete groupWidget1;
    delete groupWidget2;
}

void ColourWidgetTests::fade_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("startPoint");
    QTest::addColumn<QPoint>("endPoint");
    QTest::addColumn<QColor>("startColour");
    QTest::addColumn<QColor>("endColour");
    QTest::addColumn<IntList>("fadeSpread");
    QTest::addColumn<PointList>("fadePoints");

    int maxRow = 3;
    int maxColumn = 3;

    IntList fadeData;
    PointList fadePoints;

    fadeData.append(1);
    fadeData.append(2);
    fadeData.append(1);

    fadePoints.append(QPoint(0,0));
    fadePoints.append(QPoint(0,1));
    fadePoints.append(QPoint(1,0));
    fadePoints.append(QPoint(1,1));

    QTest::newRow("top left bottom right 2 x 2") << maxRow
                                               << maxColumn
                                               << QPoint(0,0)
                                               << QPoint(1,1)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadePoints.clear();
    fadePoints.append(QPoint(1,1));
    fadePoints.append(QPoint(1,0));
    fadePoints.append(QPoint(0,1));
    fadePoints.append(QPoint(0,0));

    QTest::newRow("bottom right top left 2 x 2") << maxRow
                                               << maxColumn
                                               << QPoint(1,1)
                                               << QPoint(0,0)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadePoints.clear();
    fadePoints.append(QPoint(1,0));
    fadePoints.append(QPoint(0,0));
    fadePoints.append(QPoint(1,1));
    fadePoints.append(QPoint(0,1));

    QTest::newRow("top right bottom left 2 x 2") << maxRow
                                               << maxColumn
                                               << QPoint(1,0)
                                               << QPoint(0,1)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadePoints.clear();
    fadePoints.append(QPoint(0,1));
    fadePoints.append(QPoint(0,0));
    fadePoints.append(QPoint(1,1));
    fadePoints.append(QPoint(1,0));

    QTest::newRow("bottom left top right 2 x 2") << maxRow
                                               << maxColumn
                                               << QPoint(0,1)
                                               << QPoint(1,0)
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
    fadePoints.append(QPoint(0,0));
    fadePoints.append(QPoint(1,0));
    fadePoints.append(QPoint(0,1));
    fadePoints.append(QPoint(1,1));
    fadePoints.append(QPoint(0,2));
    fadePoints.append(QPoint(1,2));

    QTest::newRow("top left bottom right 3 x 2") << maxRow
                                               << maxColumn
                                               << QPoint(0,0)
                                               << QPoint(1,2)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadePoints.clear();
    fadePoints.append(QPoint(1,2));
    fadePoints.append(QPoint(0,2));
    fadePoints.append(QPoint(1,1));
    fadePoints.append(QPoint(0,1));
    fadePoints.append(QPoint(1,0));
    fadePoints.append(QPoint(0,0));

    QTest::newRow("bottom right top left 3 x 2") << maxRow
                                               << maxColumn
                                               << QPoint(1,2)
                                               << QPoint(0,0)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadePoints.clear();
    fadePoints.append(QPoint(1,0));
    fadePoints.append(QPoint(0,0));
    fadePoints.append(QPoint(1,1));
    fadePoints.append(QPoint(0,1));
    fadePoints.append(QPoint(1,2));
    fadePoints.append(QPoint(0,2));

    QTest::newRow("top right bottom left 3 x 2") << maxRow
                                               << maxColumn
                                               << QPoint(1,0)
                                               << QPoint(0,2)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadePoints.clear();
    fadePoints.append(QPoint(0,2));
    fadePoints.append(QPoint(0,1));
    fadePoints.append(QPoint(1,2));
    fadePoints.append(QPoint(0,0));
    fadePoints.append(QPoint(1,1));
    fadePoints.append(QPoint(1,0));

    QTest::newRow("bottom left top right 3 x 2") << maxRow
                                               << maxColumn
                                               << QPoint(0,2)
                                               << QPoint(1,0)
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
    fadePoints.append(QPoint(0,0));
    fadePoints.append(QPoint(1,0));
    fadePoints.append(QPoint(0,1));
    fadePoints.append(QPoint(2,0));
    fadePoints.append(QPoint(1,1));
    fadePoints.append(QPoint(2,1));

    QTest::newRow("top left bottom right 2 x 3") << maxRow
                                               << maxColumn
                                               << QPoint(0,0)
                                               << QPoint(2,1)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadePoints.clear();
    fadePoints.append(QPoint(2,1));
    fadePoints.append(QPoint(2,0));
    fadePoints.append(QPoint(1,1));
    fadePoints.append(QPoint(1,0));
    fadePoints.append(QPoint(0,1));
    fadePoints.append(QPoint(0,0));

    QTest::newRow("bottom right top left 2 x 3") << maxRow
                                               << maxColumn
                                               << QPoint(2,1)
                                               << QPoint(0,0)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadePoints.clear();
    fadePoints.append(QPoint(2,0));
    fadePoints.append(QPoint(1,0));
    fadePoints.append(QPoint(2,1));
    fadePoints.append(QPoint(0,0));
    fadePoints.append(QPoint(1,1));
    fadePoints.append(QPoint(0,1));

    QTest::newRow("top right bottom left 2 x 3") << maxRow
                                               << maxColumn
                                               << QPoint(2,0)
                                               << QPoint(0,1)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;


    fadePoints.clear();
    fadePoints.append(QPoint(0,1));
    fadePoints.append(QPoint(1,1));
    fadePoints.append(QPoint(0,0));
    fadePoints.append(QPoint(2,1));
    fadePoints.append(QPoint(1,0));
    fadePoints.append(QPoint(2,0));

    QTest::newRow("bottom left top right 2 x 3") << maxRow
                                               << maxColumn
                                               << QPoint(0,1)
                                               << QPoint(2,0)
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
    fadePoints.append(QPoint(0,0));
    fadePoints.append(QPoint(0,1));
    fadePoints.append(QPoint(1,0));
    fadePoints.append(QPoint(2,0));
    fadePoints.append(QPoint(1,1));
    fadePoints.append(QPoint(0,2));
    fadePoints.append(QPoint(2,1));
    fadePoints.append(QPoint(1,2));
    fadePoints.append(QPoint(2,2));

    QTest::newRow("top left bottom right 3 x 3") << maxRow
                                               << maxColumn
                                               << QPoint(0,0)
                                               << QPoint(2,2)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadePoints.clear();
    fadePoints.append(QPoint(2,2));
    fadePoints.append(QPoint(2,1));
    fadePoints.append(QPoint(1,2));
    fadePoints.append(QPoint(2,0));
    fadePoints.append(QPoint(1,1));
    fadePoints.append(QPoint(0,2));
    fadePoints.append(QPoint(1,0));
    fadePoints.append(QPoint(0,1));
    fadePoints.append(QPoint(0,0));

    QTest::newRow("bottom right top left 3 x 3") << maxRow
                                               << maxColumn
                                               << QPoint(2,2)
                                               << QPoint(0,0)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadePoints.clear();
    fadePoints.append(QPoint(2,0));
    fadePoints.append(QPoint(2,1));
    fadePoints.append(QPoint(1,0));
    fadePoints.append(QPoint(2,2));
    fadePoints.append(QPoint(1,1));
    fadePoints.append(QPoint(0,0));
    fadePoints.append(QPoint(1,2));
    fadePoints.append(QPoint(0,1));
    fadePoints.append(QPoint(0,2));

    QTest::newRow("top right bottom left 3 x 3") << maxRow
                                               << maxColumn
                                               << QPoint(2,0)
                                               << QPoint(0,2)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadePoints.clear();
    fadePoints.append(QPoint(0,2));
    fadePoints.append(QPoint(0,1));
    fadePoints.append(QPoint(1,2));
    fadePoints.append(QPoint(0,0));
    fadePoints.append(QPoint(1,1));
    fadePoints.append(QPoint(2,2));
    fadePoints.append(QPoint(1,0));
    fadePoints.append(QPoint(1,2));
    fadePoints.append(QPoint(2,0));

    QTest::newRow("bottom left top right 3 x 3") << maxRow
                                               << maxColumn
                                               << QPoint(0,2)
                                               << QPoint(2,0)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;


    fadeData.clear();
    fadeData.append(1);
    fadeData.append(1);
    fadeData.append(1);

    fadePoints.clear();
    fadePoints.append(QPoint(0,0));
    fadePoints.append(QPoint(0,1));
    fadePoints.append(QPoint(0,2));

    QTest::newRow("top bottom first column 3 x 1") << maxRow
                                               << maxColumn
                                               << QPoint(0,0)
                                               << QPoint(0,2)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;


    fadePoints.clear();
    fadePoints.append(QPoint(0,2));
    fadePoints.append(QPoint(0,1));
    fadePoints.append(QPoint(0,0));

    QTest::newRow("bottom top first column 3 x 1") << maxRow
                                               << maxColumn
                                               << QPoint(0,2)
                                               << QPoint(0,0)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;


    fadePoints.clear();
    fadePoints.append(QPoint(1,0));
    fadePoints.append(QPoint(1,1));
    fadePoints.append(QPoint(1,2));

    QTest::newRow("top bottom middle column 3 x 1") << maxRow
                                               << maxColumn
                                               << QPoint(1,0)
                                               << QPoint(1,2)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;


    fadePoints.clear();
    fadePoints.append(QPoint(1,2));
    fadePoints.append(QPoint(1,1));
    fadePoints.append(QPoint(1,0));

    QTest::newRow("bottom top middle column 3 x 1") << maxRow
                                               << maxColumn
                                               << QPoint(1,2)
                                               << QPoint(1,0)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;


    fadePoints.clear();
    fadePoints.append(QPoint(2,0));
    fadePoints.append(QPoint(2,1));
    fadePoints.append(QPoint(2,2));

    QTest::newRow("top bottom last column 3 x 1") << maxRow
                                               << maxColumn
                                               << QPoint(2,0)
                                               << QPoint(2,2)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;


    fadePoints.clear();
    fadePoints.append(QPoint(2,2));
    fadePoints.append(QPoint(2,1));
    fadePoints.append(QPoint(2,0));

    QTest::newRow("bottom top last column 3 x 1") << maxRow
                                               << maxColumn
                                               << QPoint(2,2)
                                               << QPoint(2,0)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

    fadePoints.clear();
    fadePoints.append(QPoint(0,0));
    fadePoints.append(QPoint(1,0));
    fadePoints.append(QPoint(2,0));

    QTest::newRow("top bottom first row 1 x 3") << maxRow
                                               << maxColumn
                                               << QPoint(0,0)
                                               << QPoint(2,0)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;


    fadePoints.clear();
    fadePoints.append(QPoint(2,0));
    fadePoints.append(QPoint(1,0));
    fadePoints.append(QPoint(0,0));

    QTest::newRow("bottom top first row 1 x 3") << maxRow
                                               << maxColumn
                                               << QPoint(2,0)
                                               << QPoint(0,0)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;


    fadePoints.clear();
    fadePoints.append(QPoint(0,1));
    fadePoints.append(QPoint(1,1));
    fadePoints.append(QPoint(2,1));

    QTest::newRow("top bottom middle row 1 x 3") << maxRow
                                               << maxColumn
                                               << QPoint(0,1)
                                               << QPoint(2,1)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;


    fadePoints.clear();
    fadePoints.append(QPoint(2,1));
    fadePoints.append(QPoint(1,1));
    fadePoints.append(QPoint(0,1));

    QTest::newRow("bottom top middle row 1 x 3") << maxRow
                                               << maxColumn
                                               << QPoint(2,1)
                                               << QPoint(0,1)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;


    fadePoints.clear();
    fadePoints.append(QPoint(0,2));
    fadePoints.append(QPoint(1,2));
    fadePoints.append(QPoint(2,2));

    QTest::newRow("top bottom last row 1 x 3") << maxRow
                                               << maxColumn
                                               << QPoint(0,2)
                                               << QPoint(2,2)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;


    fadePoints.clear();
    fadePoints.append(QPoint(2,2));
    fadePoints.append(QPoint(1,2));
    fadePoints.append(QPoint(0,2));

    QTest::newRow("bottom top last row 1 x 3") << maxRow
                                               << maxColumn
                                               << QPoint(2,2)
                                               << QPoint(0,2)
                                               << QColor(Qt::magenta)
                                               << QColor(Qt::green)
                                               << fadeData
                                               << fadePoints;

}

void ColourWidgetTests::fade() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, startPoint);
    QFETCH(QPoint, endPoint);
    QFETCH(QColor, startColour);
    QFETCH(QColor, endColour);
    QFETCH(IntList, fadeSpread);
    QFETCH(PointList, fadePoints);

    try {
        iFadeData = new FadeData();

        int fadeLine;
        foreach(fadeLine, fadeSpread) {
            iFadeData->fadeSpread.append(fadeLine);
        }

        QPoint fadePoint;
        foreach(fadePoint, fadePoints) {
            iFadeData->fadePoints.append(fadePoint);
        }

        iGroupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

        iGroupWidget->selectOne(iGroupWidget->widgetAt(startPoint.y(), startPoint.x()));
        iGroupWidget->setColour(startColour);

        iGroupWidget->selectArea(iGroupWidget->widgetAt(endPoint.y(), endPoint.x()));

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
    qDebug("fadeComplete");
    int pointCounter = 0;

    for(int i = 0; i < iFadeData->fadeSpread.count(); i++) {
        int lineCount = iFadeData->fadeSpread[i];
        QColor colour = iFadeData->fadeColours[i];

        for(int j = pointCounter; j < pointCounter + lineCount; j++) {
            QPoint point = iFadeData->fadePoints[pointCounter];
            QCOMPARE(iGroupWidget->widgetAt(point.y(), point.x()).colour(), colour);
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

void ColourWidgetTests::clickOne_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("selectedPoint");

    int maxRows = 2;
    int maxColumns = 3;

    QTest::newRow("click one") << maxRows
                                << maxColumns
                                << QPoint(0,0);
}

void ColourWidgetTests::clickOne() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, selectedPoint);

    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    QTest::mouseClick(&(groupWidget->widgetAt(selectedPoint.y(), selectedPoint.x())), Qt::LeftButton);

    QList<QPoint> selPoint;
    selPoint.append(selectedPoint);

    compareAreaPoints(groupWidget, selPoint);

    delete groupWidget;
}


void ColourWidgetTests::clickOneClickOne_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("firstPoint");
    QTest::addColumn<QPoint>("secondPoint");
    QTest::addColumn<QPoint>("selectedPoint");

    int maxRows = 2;
    int maxColumns = 3;

    QTest::newRow("click one click same") << maxRows
                                << maxColumns
                                << QPoint(1,1)
                                << QPoint(1,1)
                                << QPoint(INVALID, INVALID);

    QTest::newRow("click one click different") << maxRows
                                << maxColumns
                                << QPoint(1,1)
                                << QPoint(0,1)
                                << QPoint(0,1);
}

void ColourWidgetTests::clickOneClickOne() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstPoint);
    QFETCH(QPoint, secondPoint);
    QFETCH(QPoint, selectedPoint);

    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    QTest::mouseClick(&(groupWidget->widgetAt(firstPoint.y(), firstPoint.x())), Qt::LeftButton);
    QTest::mouseClick(&(groupWidget->widgetAt(secondPoint.y(), secondPoint.x())), Qt::LeftButton);

    QList<QPoint> selectedPoints;
    if(selectedPoint.x() != INVALID && selectedPoint.y() != INVALID) {
        selectedPoints.append(selectedPoint);
    }

    compareAreaPoints(groupWidget, selectedPoints);

    delete groupWidget;
}

void ColourWidgetTests::doubleClickOne_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("selectedPoint");

    int maxRows = 2;
    int maxColumns = 3;

    QTest::newRow("double click one") << maxRows
                                    << maxColumns
                                    << QPoint(0,0);

}

void ColourWidgetTests::doubleClickOne() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, selectedPoint);

    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    doubleClickWidgetAndDismissDialog(*groupWidget, selectedPoint);

    QList<QPoint> selPoint;
    selPoint.append(selectedPoint);

    compareAreaPoints(groupWidget, selPoint);

    delete groupWidget;
}

void ColourWidgetTests::doubleClickOneClickOne_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("firstPoint");
    QTest::addColumn<QPoint>("secondPoint");
    QTest::addColumn<QPoint>("selectedPoint");

    int maxRows = 2;
    int maxColumns = 3;

    QTest::newRow("double click one click same") << maxRows
                                << maxColumns
                                << QPoint(0,0)
                                << QPoint(0,0)
                                << QPoint(0,0);

    QTest::newRow("double click one click different") << maxRows
                                << maxColumns
                                << QPoint(0,0)
                                << QPoint(0,1)
                                << QPoint(0,1);
}

void ColourWidgetTests::doubleClickOneClickOne() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstPoint);
    QFETCH(QPoint, secondPoint);
    QFETCH(QPoint, selectedPoint);

    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    doubleClickWidgetAndDismissDialog(*groupWidget, firstPoint);

    QTest::mouseClick(&(groupWidget->widgetAt(secondPoint.y(), secondPoint.x())), Qt::LeftButton);

    QList<QPoint> selPoint;
    selPoint.append(selectedPoint);

    compareAreaPoints(groupWidget, selPoint);

    delete groupWidget;
}

void ColourWidgetTests::clickOneDoubleClickOne_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("firstPoint");
    QTest::addColumn<QPoint>("secondPoint");
    QTest::addColumn<QPoint>("selectedPoint");

    int maxRows = 2;
    int maxColumns = 3;

    QTest::newRow("click one double click same") << maxRows
                                << maxColumns
                                << QPoint(0,0)
                                << QPoint(0,0)
                                << QPoint(0,0);

    QTest::newRow("click one double click different") << maxRows
                                << maxColumns
                                << QPoint(0,0)
                                << QPoint(0,1)
                                << QPoint(0,1);
}

void ColourWidgetTests::clickOneDoubleClickOne() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstPoint);
    QFETCH(QPoint, secondPoint);
    QFETCH(QPoint, selectedPoint);

    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    QTest::mouseClick(&(groupWidget->widgetAt(firstPoint.y(), firstPoint.x())), Qt::LeftButton);

    doubleClickWidgetAndDismissDialog(*groupWidget, secondPoint);

    QList<QPoint> selPoint;
    selPoint.append(selectedPoint);

    compareAreaPoints(groupWidget, selPoint);

    delete groupWidget;
}

void ColourWidgetTests::doubleClickOneDoubleClickOne_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("firstPoint");
    QTest::addColumn<QPoint>("secondPoint");
    QTest::addColumn<QPoint>("selectedPoint");

    int maxRows;
    int maxColumns;

    maxRows = 2;
    maxColumns = 3;

    QTest::newRow("double click one double click same") << maxRows
                                << maxColumns
                                << QPoint(0,0)
                                << QPoint(0,0)
                                << QPoint(0,0);

    QTest::newRow("double click one double click different") << maxRows
                                << maxColumns
                                << QPoint(0,0)
                                << QPoint(0,1)
                                << QPoint(0,1);
}

void ColourWidgetTests::doubleClickOneDoubleClickOne() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstPoint);
    QFETCH(QPoint, secondPoint);
    QFETCH(QPoint, selectedPoint);

    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    doubleClickWidgetAndDismissDialog(*groupWidget, firstPoint);
    doubleClickWidgetAndDismissDialog(*groupWidget, secondPoint);

    QList<QPoint> selPoint;
    selPoint.append(selectedPoint);

    compareAreaPoints(groupWidget, selPoint);

    delete groupWidget;
}

void ColourWidgetTests::ctrlClickMany_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("firstPoint");
    QTest::addColumn<QPoint>("secondPoint");
    QTest::addColumn<QPoint>("thirdPoint");
    QTest::addColumn<PointList>("selectedPoints");

    int maxRows;
    int maxColumns;

    maxRows = 2;
    maxColumns = 3;

    PointList selectedPoints;

    selectedPoints.append(QPoint(0,0));
    selectedPoints.append(QPoint(0,1));
    selectedPoints.append(QPoint(2,1));

    QTest::newRow("select three") << maxRows
                                << maxColumns
                                << QPoint(0,0)
                                << QPoint(0,1)
                                << QPoint(2,1)
                                << selectedPoints;

    selectedPoints.clear();

    selectedPoints.append(QPoint(0,0));

    QTest::newRow("select two unselect one") << maxRows
                                                << maxColumns
                                                << QPoint(0,0)
                                                << QPoint(0,1)
                                                << QPoint(0,1)
                                                << selectedPoints;
}

void ColourWidgetTests::ctrlClickMany() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstPoint);
    QFETCH(QPoint, secondPoint);
    QFETCH(QPoint, thirdPoint);
    QFETCH(PointList, selectedPoints);

    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    QTest::mouseClick(&(groupWidget->widgetAt(firstPoint.y(), firstPoint.x())), Qt::LeftButton, Qt::ControlModifier);
    QTest::mouseClick(&(groupWidget->widgetAt(secondPoint.y(), secondPoint.x())), Qt::LeftButton, Qt::ControlModifier);
    QTest::mouseClick(&(groupWidget->widgetAt(thirdPoint.y(), thirdPoint.x())), Qt::LeftButton, Qt::ControlModifier);

    compareAreaPoints(groupWidget, selectedPoints);

    delete groupWidget;
}

void ColourWidgetTests::clickOneCtrlClickMany_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("firstPoint");
    QTest::addColumn<QPoint>("secondPoint");
    QTest::addColumn<QPoint>("thirdPoint");
    QTest::addColumn<PointList>("selectedPoints");

    int maxRows;
    int maxColumns;

    maxRows = 2;
    maxColumns = 3;

    PointList selectedPoints;

    selectedPoints.append(QPoint(0,0));
    selectedPoints.append(QPoint(0,1));
    selectedPoints.append(QPoint(2,1));

    QTest::newRow("select three") << maxRows
                                << maxColumns
                                << QPoint(0,0)
                                << QPoint(0,1)
                                << QPoint(2,1)
                                << selectedPoints;

    selectedPoints.clear();

    selectedPoints.append(QPoint(0,0));

    QTest::newRow("select two unselect one") << maxRows
                                                << maxColumns
                                                << QPoint(0,0)
                                                << QPoint(0,1)
                                                << QPoint(0,1)
                                                << selectedPoints;
}

void ColourWidgetTests::clickOneCtrlClickMany() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstPoint);
    QFETCH(QPoint, secondPoint);
    QFETCH(QPoint, thirdPoint);
    QFETCH(PointList, selectedPoints);

    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    QTest::mouseClick(&(groupWidget->widgetAt(firstPoint.y(), firstPoint.x())), Qt::LeftButton);
    QTest::mouseClick(&(groupWidget->widgetAt(secondPoint.y(), secondPoint.x())), Qt::LeftButton, Qt::ControlModifier);
    QTest::mouseClick(&(groupWidget->widgetAt(thirdPoint.y(), thirdPoint.x())), Qt::LeftButton, Qt::ControlModifier);

    compareAreaPoints(groupWidget, selectedPoints);
}

void ColourWidgetTests::ctrlClickManyClickOne_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("firstPoint");
    QTest::addColumn<QPoint>("secondPoint");
    QTest::addColumn<QPoint>("thirdPoint");
    QTest::addColumn<PointList>("selectedPoints");

    int maxRows;
    int maxColumns;

    maxRows = 2;
    maxColumns = 3;

    PointList selectedPoints;

    selectedPoints.append(QPoint(2,1));

    QTest::newRow("select two select different") << maxRows
                                << maxColumns
                                << QPoint(0,0)
                                << QPoint(0,1)
                                << QPoint(2,1)
                                << selectedPoints;

    selectedPoints.clear();

    selectedPoints.append(QPoint(0,1));

    QTest::newRow("select two select same") << maxRows
                                                << maxColumns
                                                << QPoint(0,0)
                                                << QPoint(0,1)
                                                << QPoint(0,1)
                                                << selectedPoints;
}

void ColourWidgetTests::ctrlClickManyClickOne() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstPoint);
    QFETCH(QPoint, secondPoint);
    QFETCH(QPoint, thirdPoint);
    QFETCH(PointList, selectedPoints);

    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    QTest::mouseClick(&(groupWidget->widgetAt(firstPoint.y(), firstPoint.x())), Qt::LeftButton, Qt::ControlModifier);
    QTest::mouseClick(&(groupWidget->widgetAt(secondPoint.y(), secondPoint.x())), Qt::LeftButton, Qt::ControlModifier);
    QTest::mouseClick(&(groupWidget->widgetAt(thirdPoint.y(), thirdPoint.x())), Qt::LeftButton);

    compareAreaPoints(groupWidget, selectedPoints);
}

void ColourWidgetTests::clickShiftClick_data() {
    areaData();
}

void ColourWidgetTests::clickShiftClick() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstSelected);
    QFETCH(QPoint, secondSelected);

    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    QTest::mouseClick(&(groupWidget->widgetAt(firstSelected.y(), firstSelected.x())), Qt::LeftButton);
    QTest::mouseClick(&(groupWidget->widgetAt(secondSelected.y(), secondSelected.x())), Qt::LeftButton, Qt::ShiftModifier);

    QList<QPoint> selectedPoints;

    calculateAreaPoints(selectedPoints, firstSelected, secondSelected);
    compareAreaPoints(groupWidget, selectedPoints);

    delete groupWidget;
}

void ColourWidgetTests::clickShiftDirection_data() {
    directionData();
}

void ColourWidgetTests::clickShiftDirection() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstSelected);
    QFETCH(IntList, selectedDirections);
    QFETCH(PointList, selectedPoints);

    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);
    ColourTestWidget& widget = (ColourTestWidget&)(groupWidget->widgetAt(firstSelected.y(), firstSelected.x()));

    QTest::mouseClick(&widget, Qt::LeftButton);

    int direction;
    foreach(direction, selectedDirections) {
        Qt::Key key = (Qt::Key)direction;
        QTest::keyClick(groupWidget, key, Qt::ShiftModifier);

    }

    compareAreaPoints(groupWidget, selectedPoints);

    delete groupWidget;
}

void ColourWidgetTests::clickShiftDirectionClick_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("firstSelected");
    QTest::addColumn<IntList>("selectedDirections");
    QTest::addColumn<QPoint>("secondSelected");
    QTest::addColumn<QPoint>("selectedPoint");

    int maxRows = 3;
    int maxColumns = 3;

    IntList directions;

    directions.append(Qt::Key_Down);

    QTest::newRow("select down select different") << maxRows
                                 << maxColumns
                                 << QPoint(1,1)
                                 << directions
                                 << QPoint(0,0)
                                 << QPoint(0,0);

    directions.clear();

    directions.append(Qt::Key_Down);

    QTest::newRow("select down select same") << maxRows
                                 << maxColumns
                                 << QPoint(1,1)
                                 << directions
                                 << QPoint(1,1)
                                 << QPoint(1,1);
}

void ColourWidgetTests::clickShiftDirectionClick() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstSelected);
    QFETCH(IntList, selectedDirections);
    QFETCH(QPoint, secondSelected);
    QFETCH(QPoint, selectedPoint);

    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);
    ColourTestWidget& widget = (ColourTestWidget&)(groupWidget->widgetAt(firstSelected.y(), firstSelected.x()));

    QTest::mouseClick(&widget, Qt::LeftButton);

    int direction;
    foreach(direction, selectedDirections) {
        Qt::Key key = (Qt::Key)direction;
        QTest::keyClick(groupWidget, key, Qt::ShiftModifier);

    }

    QTest::mouseClick(&(groupWidget->widgetAt(secondSelected.y(), secondSelected.x())), Qt::LeftButton);

    QList<QPoint> selectedPoints;
    selectedPoints.append(selectedPoint);

    compareAreaPoints(groupWidget, selectedPoints);

    delete groupWidget;
}

void ColourWidgetTests::clickShiftClickRightClick_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("firstSelected");
    QTest::addColumn<QPoint>("secondSelected");

    int maxRows = 2;
    int maxColumns = 3;

    QTest::newRow("select two right click") << maxRows
                                << maxColumns
                                << QPoint(0,0)
                                << QPoint(0,1);
}

void ColourWidgetTests::clickShiftClickRightClick() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstSelected);
    QFETCH(QPoint, secondSelected);

    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    QTest::mouseClick(&(groupWidget->widgetAt(firstSelected.y(), firstSelected.x())), Qt::LeftButton);
    QTest::mouseClick(&(groupWidget->widgetAt(secondSelected.y(), secondSelected.x())), Qt::LeftButton, Qt::ShiftModifier);

    QList<QPoint> selectedPoints;

    QTest::mouseClick(&(groupWidget->widgetAt(firstSelected.y(), firstSelected.x())), Qt::RightButton);

    calculateAreaPoints(selectedPoints, firstSelected, secondSelected);
    compareAreaPoints(groupWidget, selectedPoints);

    delete groupWidget;
}

/*void ColourWidgetTests::dragDropOneInternal_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("dragOrigin");
    QTest::addColumn<QPoint>("dropTarget");
    QTest::addColumn<QColor>("originColour");

    int maxRows;
    int maxColumns;

    // single

    maxRows = 3;
    maxColumns = 3;

    IntList directions;

    directions.append(Qt::Key_Down);

    QTest::newRow("drag one drop different") << maxRows
                                 << maxColumns
                                 << QPoint(0,0)
                                 << QPoint(0,1)
                                 << QColor(Qt::magenta);

    directions.clear();

    directions.append(Qt::Key_Down);

    QTest::newRow("select down select same") << maxRows
                                 << maxColumns
                                 << QPoint(0,0)
                                 << QPoint(0,0)
                                 << QColor(Qt::magenta);
}

void ColourWidgetTests::dragDropOneInternal() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, dragOrigin);
    QFETCH(QPoint, dropTarget);
    QFETCH(QColor, originColour);

    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    ColourTestWidget& origin = (ColourTestWidget&)(groupWidget->widgetAt(dragOrigin.y(), dragOrigin.x()));
    origin.setColour(originColour);

    QTest::mousePress(&origin, Qt::LeftButton);
    QMouseEvent mouseMoveEvent(QEvent::MouseMove, origin.pos(), Qt::NoButton, Qt::LeftButton, Qt::NoModifier);
    QCoreApplication::sendEvent(&origin, &mouseMoveEvent);

    ColourTestWidget& target = (ColourTestWidget&)(groupWidget->widgetAt(dropTarget.y(), dropTarget.x()));

    QDragEnterEvent dragEnterEvent(target.pos(), Qt::CopyAction, origin.mimeData(), Qt::LeftButton, Qt::NoModifier);
    QCoreApplication::sendEvent(&target, &dragEnterEvent);

    QCoreApplication::processEvents();
    Sleeper::sleep(50);

    QDragMoveEvent dragMoveEvent(target.pos(), Qt::CopyAction, origin.mimeData(), Qt::LeftButton, Qt::NoModifier);
    QCoreApplication::sendEvent(&target, &dragMoveEvent);

    QCoreApplication::processEvents();
    Sleeper::sleep(50);

    QDropEvent dropEvent(target.pos(), Qt::CopyAction, origin.mimeData(), Qt::LeftButton, Qt::NoModifier);
    QCoreApplication::sendEvent(&target, &dropEvent);

    QCoreApplication::processEvents();
    Sleeper::sleep(50);

    QTest::mouseRelease(&target, Qt::LeftButton);

    QCOMPARE(target.colour(), origin.colour());

    delete groupWidget;
}
void ColourWidgetTests::dragDropManyInternal_data() {

}

void ColourWidgetTests::dragDropManyInternal() {

}

void ColourWidgetTests::dragDropOneExternal_data() {

}

void ColourWidgetTests::dragDropOneExternal() {

}

void ColourWidgetTests::dragDropManyExternal_data() {

}

void ColourWidgetTests::dragDropManyExternal() {

}
*/
void ColourWidgetTests::copyPasteOneInternal_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("copyPoint");
    QTest::addColumn<QPoint>("pastePoint");
    QTest::addColumn<QColor>("copyColour");

    int maxRows = 3;
    int maxColumns = 3;

    QTest::newRow("copy one paste different") << maxRows
                                 << maxColumns
                                 << QPoint(0,0)
                                 << QPoint(0,1)
                                 << QColor(Qt::magenta);

    QTest::newRow("copy one paste same") << maxRows
                                 << maxColumns
                                 << QPoint(0,0)
                                 << QPoint(0,0)
                                 << QColor(Qt::magenta);
}

void ColourWidgetTests::copyPasteOneInternal() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, copyPoint);
    QFETCH(QPoint, pastePoint);
    QFETCH(QColor, copyColour);

    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    ColourTestWidget& copyWidget = (ColourTestWidget&)groupWidget->widgetAt(copyPoint.y(), copyPoint.x());
    copyWidget.setColour(copyColour);
    QTest::keyClick(&copyWidget, Qt::Key_C, Qt::ControlModifier);

    ColourTestWidget& pasteWidget = (ColourTestWidget&)groupWidget->widgetAt(pastePoint.y(), pastePoint.x());
    QTest::keyClick(&pasteWidget, Qt::Key_V, Qt::ControlModifier);

    QCOMPARE(copyWidget.colour(), pasteWidget.colour());

    QList<QPoint> selectedPoints;
    selectedPoints.append(pastePoint);

    compareAreaPoints(groupWidget, selectedPoints);

    delete groupWidget;
}

void ColourWidgetTests::copyPasteManyInternal_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("copyFirstPoint");
    QTest::addColumn<QPoint>("copySecondPoint");
    QTest::addColumn<QPoint>("pastePoint");
    QTest::addColumn<QPoint>("pasteSecondPoint");
    QTest::addColumn<QColor>("copyColour");

    int maxRows = 4;
    int maxColumns = 4;

    QTest::newRow("copy many paste different") << maxRows
                                 << maxColumns
                                 << QPoint(0,0)
                                 << QPoint(1,1)
                                 << QPoint(2,2)
                                 << QPoint(3,3)
                                 << QColor(Qt::magenta);

    QTest::newRow("copy many paste same") << maxRows
                                 << maxColumns
                                 << QPoint(0,0)
                                 << QPoint(1,1)
                                 << QPoint(0,0)
                                 << QPoint(1,1)
                                 << QColor(Qt::magenta);

    QTest::newRow("copy many paste overlap") << maxRows
                                 << maxColumns
                                 << QPoint(0,0)
                                 << QPoint(1,1)
                                 << QPoint(1,1)
                                 << QPoint(2,2)
                                 << QColor(Qt::magenta);
}

void ColourWidgetTests::copyPasteManyInternal() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, copyFirstPoint);
    QFETCH(QPoint, copySecondPoint);
    QFETCH(QPoint, pastePoint);
    QFETCH(QPoint, pasteSecondPoint);
    QFETCH(QColor, copyColour);

    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    ColourWidget& copyFirstWidget = groupWidget->widgetAt(copyFirstPoint.y(), copyFirstPoint.x());

    groupWidget->selectOne(copyFirstWidget);
    groupWidget->selectArea(groupWidget->widgetAt(copySecondPoint.y(), copySecondPoint.x()));
    groupWidget->setColour(copyColour);

    QTest::keyPress(&copyFirstWidget, Qt::Key_C, Qt::ControlModifier);

    ColourWidget& pastePointWidget = groupWidget->widgetAt(pastePoint.y(), pastePoint.x());

    QTest::keyPress(&pastePointWidget, Qt::Key_V, Qt::ControlModifier);

    QList<QPoint> testPoints;
    calculateAreaPoints(testPoints, pastePoint, pasteSecondPoint);

    for(int i = 0; i < testPoints.count(); i++) {
        QCOMPARE(groupWidget->widgetAt(testPoints[i].y(), testPoints[i].x()).colour(), copyColour);
    }

    compareAreaPoints(groupWidget, testPoints);

    delete groupWidget;
}

void ColourWidgetTests::copyPasteOneExternal_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("copyPoint");
    QTest::addColumn<QPoint>("pastePoint");
    QTest::addColumn<QColor>("copyColour");

    int maxRows = 3;
    int maxColumns = 3;

    QTest::newRow("copy one paste different") << maxRows
                                 << maxColumns
                                 << QPoint(0,0)
                                 << QPoint(0,1)
                                 << QColor(Qt::magenta);

    QTest::newRow("copy one paste same") << maxRows
                                 << maxColumns
                                 << QPoint(0,0)
                                 << QPoint(0,0)
                                 << QColor(Qt::magenta);
}

void ColourWidgetTests::copyPasteOneExternal() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, copyPoint);
    QFETCH(QPoint, pastePoint);
    QFETCH(QColor, copyColour);

    ColourGroupTestWidget* groupWidget1 = new ColourGroupTestWidget(NULL, maxRow, maxColumn);
    ColourGroupTestWidget* groupWidget2 = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    ColourTestWidget& copyWidget = (ColourTestWidget&)groupWidget1->widgetAt(copyPoint.y(), copyPoint.x());
    copyWidget.setColour(copyColour);
    QTest::keyClick(&copyWidget, Qt::Key_C, Qt::ControlModifier);

    ColourTestWidget& pasteWidget = (ColourTestWidget&)groupWidget2->widgetAt(pastePoint.y(), pastePoint.x());
    QTest::keyClick(&pasteWidget, Qt::Key_V, Qt::ControlModifier);

    QCOMPARE(copyWidget.colour(), pasteWidget.colour());

    delete groupWidget1;
    delete groupWidget2;
}

void ColourWidgetTests::copyPasteManyExternal_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("copyFirstPoint");
    QTest::addColumn<QPoint>("copySecondPoint");
    QTest::addColumn<QPoint>("pastePoint");
    QTest::addColumn<QPoint>("pasteSecondPoint");
    QTest::addColumn<QColor>("copyColour");

    int maxRows = 4;
    int maxColumns = 4;

    QTest::newRow("copy many paste many") << maxRows
                                 << maxColumns
                                 << QPoint(0,0)
                                 << QPoint(1,1)
                                 << QPoint(2,2)
                                 << QPoint(3,3)
                                 << QColor(Qt::magenta);
}

void ColourWidgetTests::copyPasteManyExternal() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, copyFirstPoint);
    QFETCH(QPoint, copySecondPoint);
    QFETCH(QPoint, pastePoint);
    QFETCH(QPoint, pasteSecondPoint);
    QFETCH(QColor, copyColour);

    ColourGroupTestWidget* groupWidget1 = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    ColourWidget& copyFirstWidget = groupWidget1->widgetAt(copyFirstPoint.y(), copyFirstPoint.x());

    groupWidget1->selectOne(copyFirstWidget);
    groupWidget1->selectArea(groupWidget1->widgetAt(copySecondPoint.y(), copySecondPoint.x()));
    groupWidget1->setColour(copyColour);

    QTest::keyPress(&copyFirstWidget, Qt::Key_C, Qt::ControlModifier);

    ColourGroupTestWidget* groupWidget2 = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    ColourWidget& pastePointWidget = groupWidget2->widgetAt(pastePoint.y(), pastePoint.x());

    QTest::keyPress(&pastePointWidget, Qt::Key_V, Qt::ControlModifier);

    QList<QPoint> selectedPoints;
    calculateAreaPoints(selectedPoints, pastePoint, pasteSecondPoint);

    for(int i = 0; i < selectedPoints.count(); i++) {
        QCOMPARE(groupWidget2->widgetAt(selectedPoints[i].y(), selectedPoints[i].x()).colour(), copyColour);
    }

    compareAreaPoints(groupWidget2, selectedPoints);

    delete groupWidget1;
    delete groupWidget2;
}

void ColourWidgetTests::doubleClickWidgetAndDismissDialog(ColourGroupTestWidget& groupWidget, QPoint widgetPoint) {
    ColourTestWidget& widget = (ColourTestWidget&)(groupWidget.widgetAt(widgetPoint.y(), widgetPoint.x()));

    iColourDialog = &(widget.colourDialog());

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

void ColourWidgetTests::calculateAreaPoints(QList<QPoint>& selectedPoints, QPoint firstSelected, QPoint secondSelected) {
    int leftColumn;
    int rightColumn;
    int topRow;
    int bottomRow;

    if(firstSelected.y() > secondSelected.y()) {
        topRow = secondSelected.y();
        bottomRow = firstSelected.y();
    } else {
        topRow = firstSelected.y();
        bottomRow = secondSelected.y();
    }

    if(firstSelected.x() > secondSelected.x()) {
        leftColumn = secondSelected.x();
        rightColumn = firstSelected.x();
    } else {
        leftColumn = firstSelected.x();
        rightColumn = secondSelected.x();
    }

    for(int i = topRow; i < bottomRow + 1; i++) {
        for(int j = leftColumn; j < rightColumn + 1; j++) {
            selectedPoints.append(QPoint(j, i));
        }
    }
}

void ColourWidgetTests::compareAreaPoints(ColourGroupTestWidget* groupWidget, QList<QPoint> selectedPoints) {
    QList<ColourWidget*> selectedList = groupWidget->selectedItems();

    QCOMPARE(selectedList.count(), selectedPoints.count());

    for(int i = 0; i < selectedPoints.count(); i++) {
        ColourWidget& widget = groupWidget->widgetAt(selectedPoints.at(i).y(), selectedPoints.at(i).x());

        bool contains = selectedList.contains(&widget);
        QCOMPARE(contains, true);
    }

}

void ColourWidgetTests::areaData() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("firstSelected");
    QTest::addColumn<QPoint>("secondSelected");
    QTest::addColumn<QString>("errorMessage");

    int maxRows;
    int maxColumns;

    maxRows = 2;
    maxColumns = 3;

    QTest::newRow("select same") << maxRows
                                << maxColumns
                                << QPoint(0,0)
                                << QPoint(0,0);

    QTest::newRow("select two") << maxRows
                                << maxColumns
                                << QPoint(0,0)
                                << QPoint(0,1);

    QTest::newRow("select four") << maxRows
                                << maxColumns
                                << QPoint(0,0)
                                << QPoint(1,1);

    QTest::newRow("select all") << maxRows
                                << maxColumns
                                << QPoint(0,0)
                                << QPoint(maxColumns - 1, maxRows - 1);
}

void ColourWidgetTests::directionData() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("firstSelected");
    QTest::addColumn<IntList>("selectedDirections");
    QTest::addColumn<PointList>("selectedPoints");

    int maxRows;
    int maxColumns;

    // single

    maxRows = 3;
    maxColumns = 3;

    IntList directions;
    PointList selectedPoints;

    directions.append(Qt::Key_Down);

    selectedPoints.append(QPoint(1, 1));
    selectedPoints.append(QPoint(1, 2));

    QTest::newRow("select down") << maxRows
                                 << maxColumns
                                 << QPoint(1,1)
                                 << directions
                                 << selectedPoints;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Up);

    selectedPoints.append(QPoint(1, 1));
    selectedPoints.append(QPoint(1, 0));

    QTest::newRow("select up") << maxRows
                                 << maxColumns
                                 << QPoint(1,1)
                                 << directions
                                 << selectedPoints;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Left);

    selectedPoints.append(QPoint(1, 1));
    selectedPoints.append(QPoint(0, 1));

    QTest::newRow("select left") << maxRows
                                 << maxColumns
                                 << QPoint(1,1)
                                 << directions
                                 << selectedPoints;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Right);

    selectedPoints.append(QPoint(1, 1));
    selectedPoints.append(QPoint(2, 1));

    QTest::newRow("select right")  << maxRows
                                 << maxColumns
                                 << QPoint(1,1)
                                 << directions
                                 << selectedPoints;

    // multiple

    maxRows = 5;
    maxColumns = 5;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Down);
    directions.append(Qt::Key_Down);

    selectedPoints.append(QPoint(2, 2));
    selectedPoints.append(QPoint(2, 3));
    selectedPoints.append(QPoint(2, 4));

    QTest::newRow("select down multiple") << maxRows
                                 << maxColumns
                                 << QPoint(2,2)
                                 << directions
                                 << selectedPoints;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Up);
    directions.append(Qt::Key_Up);

    selectedPoints.append(QPoint(2, 2));
    selectedPoints.append(QPoint(2, 1));
    selectedPoints.append(QPoint(2, 0));

    QTest::newRow("select up multiple") << maxRows
                                 << maxColumns
                                 << QPoint(2,2)
                                 << directions
                                 << selectedPoints;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Left);
    directions.append(Qt::Key_Left);

    selectedPoints.append(QPoint(2, 2));
    selectedPoints.append(QPoint(1, 2));
    selectedPoints.append(QPoint(0, 2));

    QTest::newRow("select left multiple") << maxRows
                                 << maxColumns
                                 << QPoint(2,2)
                                 << directions
                                 << selectedPoints;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Right);
    directions.append(Qt::Key_Right);

    selectedPoints.append(QPoint(2, 2));
    selectedPoints.append(QPoint(3, 2));
    selectedPoints.append(QPoint(4, 2));

    QTest::newRow("select right multiple") << maxRows
                                 << maxColumns
                                 << QPoint(2,2)
                                 << directions
                                 << selectedPoints;

    // multiple select and unselect

    maxRows = 5;
    maxColumns = 5;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Down);
    directions.append(Qt::Key_Down);
    directions.append(Qt::Key_Up);
    directions.append(Qt::Key_Up);

    selectedPoints.append(QPoint(2, 2));

    QTest::newRow("select down multiple unselect") << maxRows
                                 << maxColumns
                                 << QPoint(2,2)
                                 << directions
                                 << selectedPoints;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Up);
    directions.append(Qt::Key_Up);
    directions.append(Qt::Key_Down);
    directions.append(Qt::Key_Down);

    selectedPoints.append(QPoint(2, 2));

    QTest::newRow("select up multiple unselect") << maxRows
                                 << maxColumns
                                 << QPoint(2,2)
                                 << directions
                                 << selectedPoints;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Left);
    directions.append(Qt::Key_Left);
    directions.append(Qt::Key_Right);
    directions.append(Qt::Key_Right);

    selectedPoints.append(QPoint(2, 2));

    QTest::newRow("select left multiple unselect") << maxRows
                                 << maxColumns
                                 << QPoint(2,2)
                                 << directions
                                 << selectedPoints;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Right);
    directions.append(Qt::Key_Right);
    directions.append(Qt::Key_Left);
    directions.append(Qt::Key_Left);

    selectedPoints.append(QPoint(2, 2));

    QTest::newRow("select right multiple unselect") << maxRows
                                 << maxColumns
                                 << QPoint(2,2)
                                 << directions
                                 << selectedPoints;

    // multiple select and unselect and select the other way

    maxRows = 5;
    maxColumns = 5;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Down);
    directions.append(Qt::Key_Down);
    directions.append(Qt::Key_Up);
    directions.append(Qt::Key_Up);
    directions.append(Qt::Key_Up);

    selectedPoints.append(QPoint(2, 2));
    selectedPoints.append(QPoint(2, 1));

    QTest::newRow("select down unselect select up") << maxRows
                                 << maxColumns
                                 << QPoint(2,2)
                                 << directions
                                 << selectedPoints;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Up);
    directions.append(Qt::Key_Up);
    directions.append(Qt::Key_Down);
    directions.append(Qt::Key_Down);
    directions.append(Qt::Key_Down);

    selectedPoints.append(QPoint(2, 2));
    selectedPoints.append(QPoint(2, 3));

    QTest::newRow("select up unselect select down") << maxRows
                                 << maxColumns
                                 << QPoint(2,2)
                                 << directions
                                 << selectedPoints;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Left);
    directions.append(Qt::Key_Left);
    directions.append(Qt::Key_Right);
    directions.append(Qt::Key_Right);
    directions.append(Qt::Key_Right);

    selectedPoints.append(QPoint(2, 2));
    selectedPoints.append(QPoint(3, 2));

    QTest::newRow("select left unselect select right") << maxRows
                                 << maxColumns
                                 << QPoint(2,2)
                                 << directions
                                 << selectedPoints;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Right);
    directions.append(Qt::Key_Right);
    directions.append(Qt::Key_Left);
    directions.append(Qt::Key_Left);
    directions.append(Qt::Key_Left);

    selectedPoints.append(QPoint(2, 2));
    selectedPoints.append(QPoint(1, 2));

    QTest::newRow("select right unselect select left") << maxRows
                                 << maxColumns
                                 << QPoint(2,2)
                                 << directions
                                 << selectedPoints;

    // select area

    maxRows = 5;
    maxColumns = 5;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Down);
    directions.append(Qt::Key_Left);

    selectedPoints.append(QPoint(2, 2));
    selectedPoints.append(QPoint(1, 2));
    selectedPoints.append(QPoint(2, 3));
    selectedPoints.append(QPoint(1, 3));

    QTest::newRow("select area down left") << maxRows
                                 << maxColumns
                                 << QPoint(2,2)
                                 << directions
                                 << selectedPoints;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Up);
    directions.append(Qt::Key_Left);

    selectedPoints.append(QPoint(2, 2));
    selectedPoints.append(QPoint(1, 2));
    selectedPoints.append(QPoint(2, 1));
    selectedPoints.append(QPoint(1, 1));

    QTest::newRow("select area up left") << maxRows
                                 << maxColumns
                                 << QPoint(2,2)
                                 << directions
                                 << selectedPoints;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Left);
    directions.append(Qt::Key_Down);

    selectedPoints.append(QPoint(2, 2));
    selectedPoints.append(QPoint(1, 2));
    selectedPoints.append(QPoint(2, 3));
    selectedPoints.append(QPoint(1, 3));

    QTest::newRow("select area left down") << maxRows
                                 << maxColumns
                                 << QPoint(2,2)
                                 << directions
                                 << selectedPoints;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Left);
    directions.append(Qt::Key_Up);

    selectedPoints.append(QPoint(2, 2));
    selectedPoints.append(QPoint(1, 2));
    selectedPoints.append(QPoint(2, 1));
    selectedPoints.append(QPoint(1, 1));

    QTest::newRow("select area left up") << maxRows
                                 << maxColumns
                                 << QPoint(2,2)
                                 << directions
                                 << selectedPoints;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Down);
    directions.append(Qt::Key_Right);

    selectedPoints.append(QPoint(2, 2));
    selectedPoints.append(QPoint(3, 2));
    selectedPoints.append(QPoint(2, 3));
    selectedPoints.append(QPoint(3, 3));

    QTest::newRow("select area down right") << maxRows
                                 << maxColumns
                                 << QPoint(2,2)
                                 << directions
                                 << selectedPoints;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Up);
    directions.append(Qt::Key_Right);

    selectedPoints.append(QPoint(2, 2));
    selectedPoints.append(QPoint(3, 2));
    selectedPoints.append(QPoint(2, 1));
    selectedPoints.append(QPoint(3, 1));

    QTest::newRow("select area up right") << maxRows
                                 << maxColumns
                                 << QPoint(2,2)
                                 << directions
                                 << selectedPoints;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Right);
    directions.append(Qt::Key_Down);

    selectedPoints.append(QPoint(2, 2));
    selectedPoints.append(QPoint(3, 2));
    selectedPoints.append(QPoint(2, 3));
    selectedPoints.append(QPoint(3, 3));

    QTest::newRow("select area right down") << maxRows
                                 << maxColumns
                                 << QPoint(2,2)
                                 << directions
                                 << selectedPoints;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Right);
    directions.append(Qt::Key_Up);

    selectedPoints.append(QPoint(2, 2));
    selectedPoints.append(QPoint(3, 2));
    selectedPoints.append(QPoint(2, 1));
    selectedPoints.append(QPoint(3, 1));

    QTest::newRow("select area right up") << maxRows
                                 << maxColumns
                                 << QPoint(2,2)
                                 << directions
                                 << selectedPoints;
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
