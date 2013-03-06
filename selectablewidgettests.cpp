/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "selectablewidgettests.h"

#include "selectablegroupgroupwidget.h"

#include "constants.h"
#include "testconstants.h"

using namespace AnimatorTest;

SelectableGroupTestWidget::SelectableGroupTestWidget(QWidget *parent, int maxRow, int maxColumn, SelectableGroupGroupWidget &groupGroupWidget, int groupNumber) :
    SelectableGroupWidget(parent, maxRow, maxColumn, groupGroupWidget, groupNumber) {

    // TODO yuck this is horrilbe

    iWidgetArray = new QVector<WidgetVector*>(maxRow);
    for(int i = 0; i < maxRow; i++) {
        (*iWidgetArray)[i] = new WidgetVector(maxColumn);
        for(int j = 0; j < maxColumn; j++) {
            WidgetVector& row = *((*iWidgetArray)[i]);
            Selectable* item = new Selectable(this, i*j + j);
            row[j] =  new SelectableTestWidget(this, *this, *item);
        }
    }
}

SelectableWidget& SelectableGroupTestWidget::widgetAt(int row, int column) {
    return *((*(*iWidgetArray)[row])[column]);
}

void SelectableGroupTestWidget::getWidgetPosition(SelectableWidget& widget, int* row, int* column) {
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

bool SelectableGroupTestWidget::validKeyPress(Qt::Key key) {
    return (key & Qt::Key_Right) == Qt::Key_Right ||
            (key & Qt::Key_Left) == Qt::Key_Left ||
            (key & Qt::Key_Up) == Qt::Key_Up ||
            (key & Qt::Key_Down) == Qt::Key_Down;
}

SelectableWidgetTests::SelectableWidgetTests(QObject *parent) :
    SelectableWidgetTestBase(parent) {
}

void SelectableWidgetTests::select_data() {
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

void SelectableWidgetTests::select() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(PointList, selectedPoints);

    SelectableGroupGroupWidget* groupGroupWidget = new SelectableGroupGroupWidget(NULL);
    SelectableGroupTestWidget* groupWidget = new SelectableGroupTestWidget(NULL, maxRow, maxColumn, *groupGroupWidget, 0);

    for(int i = 0; i < selectedPoints.count(); i++) {
        groupWidget->toggle(groupWidget->widgetAt(selectedPoints.at(i).y(), selectedPoints.at(i).x()));
    }

    compareAreaPoints(groupWidget, selectedPoints);
}

void SelectableWidgetTests::selectOne_data() {
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

void SelectableWidgetTests::selectOne() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, selectedPoint);

    SelectableGroupGroupWidget* groupGroupWidget = new SelectableGroupGroupWidget(NULL);
    SelectableGroupTestWidget* groupWidget = new SelectableGroupTestWidget(groupGroupWidget, maxRow, maxColumn, *groupGroupWidget, 0);

    groupWidget->selectOne(groupWidget->widgetAt(selectedPoint.y(), selectedPoint.x()));

    QList<QPoint> selPoint;
    selPoint.append(selectedPoint);

    compareAreaPoints(groupWidget, selPoint);
}

void SelectableWidgetTests::selectArea_data() {
    areaData();
}

void SelectableWidgetTests::selectArea() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstSelected);
    QFETCH(QPoint, secondSelected);

    SelectableGroupGroupWidget* groupGroupWidget = new SelectableGroupGroupWidget(NULL);
    SelectableGroupTestWidget* groupWidget = new SelectableGroupTestWidget(groupGroupWidget, maxRow, maxColumn, *groupGroupWidget, 0);

    groupWidget->selectOne(groupWidget->widgetAt(firstSelected.y(), firstSelected.x()));
    groupWidget->selectArea(groupWidget->widgetAt(secondSelected.y(), secondSelected.x()));

    QList<QPoint> selectedPoints;

    calculateAreaPoints(selectedPoints, firstSelected, secondSelected);
    compareAreaPoints(groupWidget, selectedPoints);

    delete groupWidget;
}

void SelectableWidgetTests::selectDirection_data() {
    directionData();
}

void SelectableWidgetTests::selectDirection() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstSelected);
    QFETCH(IntList, selectedDirections);
    QFETCH(PointList, selectedPoints);

    SelectableGroupGroupWidget* groupGroupWidget = new SelectableGroupGroupWidget(NULL);
    SelectableGroupTestWidget* groupWidget = new SelectableGroupTestWidget(groupGroupWidget, maxRow, maxColumn, *groupGroupWidget, 0);

    groupWidget->selectOne(groupWidget->widgetAt(firstSelected.y(), firstSelected.x()));

    int direction;
    foreach(direction, selectedDirections) {
        groupWidget->selectDirection((Qt::Key)direction);
    }

    compareAreaPoints(groupWidget, selectedPoints);

    delete groupWidget;
}

void SelectableWidgetTests::selectOneSelectExternal_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("selectedPoint");

    int maxRows = 2;
    int maxColumns = 3;

    QTest::newRow("select one select external") << maxRows
                                                << maxColumns
                                                << QPoint(0,0);

}

void SelectableWidgetTests::selectOneSelectExternal() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, selectedPoint);

    SelectableGroupGroupWidget* groupGroupWidget = new SelectableGroupGroupWidget(NULL);

    SelectableGroupTestWidget* groupWidget1 = new SelectableGroupTestWidget(groupGroupWidget, maxRow, maxColumn, *groupGroupWidget, 0);
    SelectableGroupTestWidget* groupWidget2 = new SelectableGroupTestWidget(groupGroupWidget, maxRow, maxColumn, *groupGroupWidget, 1);

    groupWidget1->selectOne(groupWidget1->widgetAt(selectedPoint.y(), selectedPoint.x()));
    groupWidget2->selectOne(groupWidget2->widgetAt(selectedPoint.y(), selectedPoint.x()));

    QCOMPARE(groupWidget1->selectedItems().count(), 0);
    QCOMPARE(groupWidget2->selectedItems().count(), 1);

    bool contains = groupWidget2->selectedItems().contains(&(groupWidget2->widgetAt(selectedPoint.y(), selectedPoint.x())));
    QCOMPARE(contains, true);

    delete groupWidget1;
    delete groupWidget2;
}

void SelectableWidgetTests::clickOne_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("selectedPoint");

    int maxRows = 2;
    int maxColumns = 3;

    QTest::newRow("click one") << maxRows
                                << maxColumns
                                << QPoint(0,0);
}

void SelectableWidgetTests::clickOne() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, selectedPoint);

    SelectableGroupGroupWidget* groupGroupWidget = new SelectableGroupGroupWidget(NULL);

    SelectableGroupTestWidget* groupWidget = new SelectableGroupTestWidget(groupGroupWidget, maxRow, maxColumn, *groupGroupWidget, 0);

    QTest::mouseClick(&(groupWidget->widgetAt(selectedPoint.y(), selectedPoint.x())), Qt::LeftButton);

    QList<QPoint> selPoint;
    selPoint.append(selectedPoint);

    compareAreaPoints(groupWidget, selPoint);

    delete groupWidget;
}


void SelectableWidgetTests::clickOneClickOne_data() {
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

void SelectableWidgetTests::clickOneClickOne() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstPoint);
    QFETCH(QPoint, secondPoint);
    QFETCH(QPoint, selectedPoint);

    SelectableGroupGroupWidget* groupGroupWidget = new SelectableGroupGroupWidget(NULL);

    SelectableGroupTestWidget* groupWidget = new SelectableGroupTestWidget(groupGroupWidget, maxRow, maxColumn, *groupGroupWidget, 0);

    QTest::mouseClick(&(groupWidget->widgetAt(firstPoint.y(), firstPoint.x())), Qt::LeftButton);
    QTest::mouseClick(&(groupWidget->widgetAt(secondPoint.y(), secondPoint.x())), Qt::LeftButton);

    QList<QPoint> selectedPoints;
    if(selectedPoint.x() != INVALID && selectedPoint.y() != INVALID) {
        selectedPoints.append(selectedPoint);
    }

    compareAreaPoints(groupWidget, selectedPoints);

    delete groupWidget;
}

void SelectableWidgetTests::ctrlClickMany_data() {
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

void SelectableWidgetTests::ctrlClickMany() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstPoint);
    QFETCH(QPoint, secondPoint);
    QFETCH(QPoint, thirdPoint);
    QFETCH(PointList, selectedPoints);

    SelectableGroupGroupWidget* groupGroupWidget = new SelectableGroupGroupWidget(NULL);

    SelectableGroupTestWidget* groupWidget = new SelectableGroupTestWidget(groupGroupWidget, maxRow, maxColumn, *groupGroupWidget, 0);

    QTest::mouseClick(&(groupWidget->widgetAt(firstPoint.y(), firstPoint.x())), Qt::LeftButton, Qt::ControlModifier);
    QTest::mouseClick(&(groupWidget->widgetAt(secondPoint.y(), secondPoint.x())), Qt::LeftButton, Qt::ControlModifier);
    QTest::mouseClick(&(groupWidget->widgetAt(thirdPoint.y(), thirdPoint.x())), Qt::LeftButton, Qt::ControlModifier);

    compareAreaPoints(groupWidget, selectedPoints);

    delete groupWidget;
}

void SelectableWidgetTests::ctrlClickManyDifferentType_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<PointList>("otherPositions");
    QTest::addColumn<QPoint>("firstPoint");
    QTest::addColumn<QPoint>("secondPoint");
    QTest::addColumn<QPoint>("thirdPoint");
    QTest::addColumn<PointList>("selectedPoints");

    int maxRows;
    int maxColumns;

    maxRows = 3;
    maxColumns = 3;

    PointList otherPositions;

    otherPositions.append(QPoint(0,0));
    otherPositions.append(QPoint(1,0));

    PointList selectedPoints;

    selectedPoints.append(QPoint(0,0));
    selectedPoints.append(QPoint(1,0));

    QTest::newRow("select two other one not") << maxRows
                                << maxColumns
                                << otherPositions
                                << QPoint(0,0)
                                << QPoint(1,0)
                                << QPoint(2,1)
                                << selectedPoints;
}

void SelectableWidgetTests::ctrlClickManyDifferentType() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(PointList, otherPositions);
    QFETCH(QPoint, firstPoint);
    QFETCH(QPoint, secondPoint);
    QFETCH(QPoint, thirdPoint);
    QFETCH(PointList, selectedPoints);

    SelectableGroupGroupWidget* groupGroupWidget = new SelectableGroupGroupWidget(NULL);

    SelectableGroupTestWidget* groupWidget = new SelectableGroupTestWidget(groupGroupWidget, maxRow, maxColumn, *groupGroupWidget, 0);

    for(int i = 0; i < otherPositions.count(); i++) {
        QPoint position = otherPositions.at(i);
        Selectable* item = new Selectable(this, i + maxRow*maxColumn);
        groupWidget->iWidgetArray->at(position.y())->replace(position.x(), new SelectableTestWidget2(groupWidget, *groupWidget, *item));
    }

    QTest::mouseClick(&(groupWidget->widgetAt(firstPoint.y(), firstPoint.x())), Qt::LeftButton, Qt::ControlModifier);
    QTest::mouseClick(&(groupWidget->widgetAt(secondPoint.y(), secondPoint.x())), Qt::LeftButton, Qt::ControlModifier);
    QTest::mouseClick(&(groupWidget->widgetAt(thirdPoint.y(), thirdPoint.x())), Qt::LeftButton, Qt::ControlModifier);

    compareAreaPoints(groupWidget, selectedPoints);

    delete groupWidget;
}

void SelectableWidgetTests::clickOneCtrlClickMany_data() {
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

void SelectableWidgetTests::clickOneCtrlClickMany() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstPoint);
    QFETCH(QPoint, secondPoint);
    QFETCH(QPoint, thirdPoint);
    QFETCH(PointList, selectedPoints);

    SelectableGroupGroupWidget* groupGroupWidget = new SelectableGroupGroupWidget(NULL);

    SelectableGroupTestWidget* groupWidget = new SelectableGroupTestWidget(groupGroupWidget, maxRow, maxColumn, *groupGroupWidget, 0);

    QTest::mouseClick(&(groupWidget->widgetAt(firstPoint.y(), firstPoint.x())), Qt::LeftButton);
    QTest::mouseClick(&(groupWidget->widgetAt(secondPoint.y(), secondPoint.x())), Qt::LeftButton, Qt::ControlModifier);
    QTest::mouseClick(&(groupWidget->widgetAt(thirdPoint.y(), thirdPoint.x())), Qt::LeftButton, Qt::ControlModifier);

    compareAreaPoints(groupWidget, selectedPoints);
}

void SelectableWidgetTests::clickOneCtrlClickManyDifferentType_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<PointList>("otherPositions");
    QTest::addColumn<QPoint>("firstPoint");
    QTest::addColumn<QPoint>("secondPoint");
    QTest::addColumn<QPoint>("thirdPoint");
    QTest::addColumn<PointList>("selectedPoints");

    int maxRows;
    int maxColumns;

    maxRows = 3;
    maxColumns = 3;

    PointList otherPositions;

    otherPositions.append(QPoint(0,0));
    otherPositions.append(QPoint(1,0));

    PointList selectedPoints;

    selectedPoints.append(QPoint(0,0));

    QTest::newRow("select one other select many both not") << maxRows
                                << maxColumns
                                << otherPositions
                                << QPoint(0,0)
                                << QPoint(1,1)
                                << QPoint(2,1)
                                << selectedPoints;

    selectedPoints.clear();
    selectedPoints.append(QPoint(1,1));

    QTest::newRow("select one not select many both other") << maxRows
                                << maxColumns
                                << otherPositions
                                << QPoint(1,1)
                                << QPoint(0,0)
                                << QPoint(1,0)
                                << selectedPoints;
}

void SelectableWidgetTests::clickOneCtrlClickManyDifferentType() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(PointList, otherPositions);
    QFETCH(QPoint, firstPoint);
    QFETCH(QPoint, secondPoint);
    QFETCH(QPoint, thirdPoint);
    QFETCH(PointList, selectedPoints);

    SelectableGroupGroupWidget* groupGroupWidget = new SelectableGroupGroupWidget(NULL);

    SelectableGroupTestWidget* groupWidget = new SelectableGroupTestWidget(groupGroupWidget, maxRow, maxColumn, *groupGroupWidget, 0);

    for(int i = 0; i < otherPositions.count(); i++) {
        QPoint position = otherPositions.at(i);
        Selectable* item = new Selectable(this, i + maxRow*maxColumn);
        groupWidget->iWidgetArray->at(position.y())->replace(position.x(), new SelectableTestWidget2(groupWidget, *groupWidget, *item));
    }

    QTest::mouseClick(&(groupWidget->widgetAt(firstPoint.y(), firstPoint.x())), Qt::LeftButton);
    QTest::mouseClick(&(groupWidget->widgetAt(secondPoint.y(), secondPoint.x())), Qt::LeftButton, Qt::ControlModifier);
    QTest::mouseClick(&(groupWidget->widgetAt(thirdPoint.y(), thirdPoint.x())), Qt::LeftButton, Qt::ControlModifier);

    compareAreaPoints(groupWidget, selectedPoints);

    delete groupWidget;
}

void SelectableWidgetTests::ctrlClickManyClickOne_data() {
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

void SelectableWidgetTests::ctrlClickManyClickOne() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstPoint);
    QFETCH(QPoint, secondPoint);
    QFETCH(QPoint, thirdPoint);
    QFETCH(PointList, selectedPoints);

    SelectableGroupGroupWidget* groupGroupWidget = new SelectableGroupGroupWidget(NULL);

    SelectableGroupTestWidget* groupWidget = new SelectableGroupTestWidget(groupGroupWidget, maxRow, maxColumn, *groupGroupWidget, 0);

    QTest::mouseClick(&(groupWidget->widgetAt(firstPoint.y(), firstPoint.x())), Qt::LeftButton, Qt::ControlModifier);
    QTest::mouseClick(&(groupWidget->widgetAt(secondPoint.y(), secondPoint.x())), Qt::LeftButton, Qt::ControlModifier);
    QTest::mouseClick(&(groupWidget->widgetAt(thirdPoint.y(), thirdPoint.x())), Qt::LeftButton);

    compareAreaPoints(groupWidget, selectedPoints);
}

void SelectableWidgetTests::ctrlClickManyClickOneDifferentType_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<PointList>("otherPositions");
    QTest::addColumn<QPoint>("firstPoint");
    QTest::addColumn<QPoint>("secondPoint");
    QTest::addColumn<QPoint>("thirdPoint");
    QTest::addColumn<PointList>("selectedPoints");

    int maxRows;
    int maxColumns;

    maxRows = 3;
    maxColumns = 3;

    PointList otherPositions;

    otherPositions.append(QPoint(0,0));
    otherPositions.append(QPoint(1,0));

    PointList selectedPoints;

    selectedPoints.append(QPoint(1,1));

    QTest::newRow("select many both other select one not") << maxRows
                                << maxColumns
                                << otherPositions
                                << QPoint(0,0)
                                << QPoint(1,0)
                                << QPoint(1,1)
                                << selectedPoints;

    selectedPoints.clear();
    selectedPoints.append(QPoint(0,0));

    QTest::newRow("select many both not select one other") << maxRows
                                << maxColumns
                                << otherPositions
                                << QPoint(2,1)
                                << QPoint(1,1)
                                << QPoint(0,0)
                                << selectedPoints;
}

void SelectableWidgetTests::ctrlClickManyClickOneDifferentType() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(PointList, otherPositions);
    QFETCH(QPoint, firstPoint);
    QFETCH(QPoint, secondPoint);
    QFETCH(QPoint, thirdPoint);
    QFETCH(PointList, selectedPoints);

    SelectableGroupGroupWidget* groupGroupWidget = new SelectableGroupGroupWidget(NULL);

    SelectableGroupTestWidget* groupWidget = new SelectableGroupTestWidget(groupGroupWidget, maxRow, maxColumn, *groupGroupWidget, 0);

    for(int i = 0; i < otherPositions.count(); i++) {
        QPoint position = otherPositions.at(i);
        Selectable* item = new Selectable(this, i + maxColumn*maxRow);
        groupWidget->iWidgetArray->at(position.y())->replace(position.x(), new SelectableTestWidget2(groupWidget, *groupWidget, *item));
    }

    QTest::mouseClick(&(groupWidget->widgetAt(firstPoint.y(), firstPoint.x())), Qt::LeftButton, Qt::ControlModifier);
    QTest::mouseClick(&(groupWidget->widgetAt(secondPoint.y(), secondPoint.x())), Qt::LeftButton, Qt::ControlModifier);
    QTest::mouseClick(&(groupWidget->widgetAt(thirdPoint.y(), thirdPoint.x())), Qt::LeftButton);

    compareAreaPoints(groupWidget, selectedPoints);

    delete groupWidget;
}

void SelectableWidgetTests::clickShiftClick_data() {
    areaData();
}

void SelectableWidgetTests::clickShiftClick() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstSelected);
    QFETCH(QPoint, secondSelected);

    SelectableGroupGroupWidget* groupGroupWidget = new SelectableGroupGroupWidget(NULL);

    SelectableGroupTestWidget* groupWidget = new SelectableGroupTestWidget(groupGroupWidget, maxRow, maxColumn, *groupGroupWidget, 0);

    QTest::mouseClick(&(groupWidget->widgetAt(firstSelected.y(), firstSelected.x())), Qt::LeftButton);
    QTest::mouseClick(&(groupWidget->widgetAt(secondSelected.y(), secondSelected.x())), Qt::LeftButton, Qt::ShiftModifier);

    QList<QPoint> selectedPoints;

    calculateAreaPoints(selectedPoints, firstSelected, secondSelected);
    compareAreaPoints(groupWidget, selectedPoints);

    delete groupWidget;
}

void SelectableWidgetTests::clickShiftClickMixedType_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<PointList>("otherPositions");
    QTest::addColumn<QPoint>("firstPoint");
    QTest::addColumn<QPoint>("secondPoint");
    QTest::addColumn<PointList>("selectedPoints");

    int maxRows;
    int maxColumns;

    maxRows = 3;
    maxColumns = 3;

    PointList otherPositions;

    otherPositions.append(QPoint(0,0));
    otherPositions.append(QPoint(1,1));

    PointList selectedPoints;

    selectedPoints.append(QPoint(0,0));
    selectedPoints.append(QPoint(1,1));

    QTest::newRow("same start and end types") << maxRows
                                                << maxColumns
                                                << otherPositions
                                                << QPoint(0,0)
                                                << QPoint(1,1)
                                                << selectedPoints;

    selectedPoints.clear();

    selectedPoints.append(QPoint(0,0));
    selectedPoints.append(QPoint(1,1));

    QTest::newRow("different start and end types") << maxRows
                                                << maxColumns
                                                << otherPositions
                                                << QPoint(0,0)
                                                << QPoint(2,2)
                                                << selectedPoints;
}

void SelectableWidgetTests::clickShiftClickMixedType() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(PointList, otherPositions);
    QFETCH(QPoint, firstPoint);
    QFETCH(QPoint, secondPoint);
    QFETCH(PointList, selectedPoints);

    SelectableGroupGroupWidget* groupGroupWidget = new SelectableGroupGroupWidget(NULL);

    SelectableGroupTestWidget* groupWidget = new SelectableGroupTestWidget(groupGroupWidget, maxRow, maxColumn, *groupGroupWidget, 0);

    for(int i = 0; i < otherPositions.count(); i++) {
        QPoint position = otherPositions.at(i);
        Selectable* item = new Selectable(this, i + maxColumn*maxRow);
        groupWidget->iWidgetArray->at(position.y())->replace(position.x(), new SelectableTestWidget2(groupWidget, *groupWidget, *item));
    }

    QTest::mouseClick(&(groupWidget->widgetAt(firstPoint.y(), firstPoint.x())), Qt::LeftButton, Qt::ControlModifier);
    QTest::mouseClick(&(groupWidget->widgetAt(secondPoint.y(), secondPoint.x())), Qt::LeftButton, Qt::ShiftModifier);

    compareAreaPoints(groupWidget, selectedPoints);

    delete groupWidget;
}

void SelectableWidgetTests::clickShiftDirection_data() {
    directionData();
}

void SelectableWidgetTests::clickShiftDirection() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstSelected);
    QFETCH(IntList, selectedDirections);
    QFETCH(PointList, selectedPoints);

    SelectableGroupGroupWidget* groupGroupWidget = new SelectableGroupGroupWidget(NULL);

    SelectableGroupTestWidget* groupWidget = new SelectableGroupTestWidget(groupGroupWidget, maxRow, maxColumn, *groupGroupWidget, 0);
    SelectableTestWidget& widget = (SelectableTestWidget&)(groupWidget->widgetAt(firstSelected.y(), firstSelected.x()));

    QTest::mouseClick(&widget, Qt::LeftButton);

    int direction;
    foreach(direction, selectedDirections) {
        Qt::Key key = (Qt::Key)direction;
        QTest::keyClick(groupWidget, key, Qt::ShiftModifier);

    }

    compareAreaPoints(groupWidget, selectedPoints);

    delete groupWidget;
}

void SelectableWidgetTests::clickShiftDirectionMixedType_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<PointList>("otherPositions");
    QTest::addColumn<QPoint>("firstPoint");
    QTest::addColumn<int>("firstDirection");
    QTest::addColumn<int>("secondDirection");
    QTest::addColumn<PointList>("selectedPoints");

    int maxRows;
    int maxColumns;

    maxRows = 3;
    maxColumns = 3;

    PointList otherPositions;

    otherPositions.append(QPoint(0,0));
    otherPositions.append(QPoint(1,0));

    PointList selectedPoints;

    selectedPoints.append(QPoint(0,0));
    selectedPoints.append(QPoint(1,0));

    QTest::newRow("select down and right") << maxRows
                                        << maxColumns
                                        << otherPositions
                                        << QPoint(0,0)
                                        << (int)Qt::Key_Down
                                        << (int)Qt::Key_Right
                                        << selectedPoints;
}

void SelectableWidgetTests::clickShiftDirectionMixedType() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(PointList, otherPositions);
    QFETCH(QPoint, firstPoint);
    QFETCH(int, firstDirection);
    QFETCH(int, secondDirection);
    QFETCH(PointList, selectedPoints);

    SelectableGroupGroupWidget* groupGroupWidget = new SelectableGroupGroupWidget(NULL);

    SelectableGroupTestWidget* groupWidget = new SelectableGroupTestWidget(groupGroupWidget, maxRow, maxColumn, *groupGroupWidget, 0);

    for(int i = 0; i < otherPositions.count(); i++) {
        QPoint position = otherPositions.at(i);
        Selectable* item = new Selectable(this, i + maxRow*maxColumn);
        groupWidget->iWidgetArray->at(position.y())->replace(position.x(), new SelectableTestWidget2(groupWidget, *groupWidget, *item));
    }

    QTest::mouseClick(&(groupWidget->widgetAt(firstPoint.y(), firstPoint.x())), Qt::LeftButton);

    Qt::Key key = (Qt::Key)firstDirection;
    QTest::keyClick(groupWidget, key, Qt::ShiftModifier);

    key = (Qt::Key)secondDirection;
    QTest::keyClick(groupWidget, key, Qt::ShiftModifier);

    compareAreaPoints(groupWidget, selectedPoints);

    delete groupWidget;
}

void SelectableWidgetTests::clickShiftDirectionClick_data() {
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

void SelectableWidgetTests::clickShiftDirectionClick() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstSelected);
    QFETCH(IntList, selectedDirections);
    QFETCH(QPoint, secondSelected);
    QFETCH(QPoint, selectedPoint);

    SelectableGroupGroupWidget* groupGroupWidget = new SelectableGroupGroupWidget(NULL);

    SelectableGroupTestWidget* groupWidget = new SelectableGroupTestWidget(groupGroupWidget, maxRow, maxColumn, *groupGroupWidget, 0);
    SelectableTestWidget& widget = (SelectableTestWidget&)(groupWidget->widgetAt(firstSelected.y(), firstSelected.x()));

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

void SelectableWidgetTests::clickShiftClickRightClick_data() {
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

void SelectableWidgetTests::clickShiftClickRightClick() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstSelected);
    QFETCH(QPoint, secondSelected);

    SelectableGroupGroupWidget* groupGroupWidget = new SelectableGroupGroupWidget(NULL);

    SelectableGroupTestWidget* groupWidget = new SelectableGroupTestWidget(groupGroupWidget, maxRow, maxColumn, *groupGroupWidget, 0);

    QTest::mouseClick(&(groupWidget->widgetAt(firstSelected.y(), firstSelected.x())), Qt::LeftButton);
    QTest::mouseClick(&(groupWidget->widgetAt(secondSelected.y(), secondSelected.x())), Qt::LeftButton, Qt::ShiftModifier);

    QList<QPoint> selectedPoints;

    QTest::mouseClick(&(groupWidget->widgetAt(firstSelected.y(), firstSelected.x())), Qt::RightButton);

    calculateAreaPoints(selectedPoints, firstSelected, secondSelected);
    compareAreaPoints(groupWidget, selectedPoints);

    delete groupWidget;
}

void SelectableWidgetTests::areaData() {
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

void SelectableWidgetTests::directionData() {
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
