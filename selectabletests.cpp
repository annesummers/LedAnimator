#include "selectabletests.h"

#include <QMap>

using namespace Test;

SelectableGroupTestWidget::SelectableGroupTestWidget(QWidget *parent, int maxRow, int maxColumn) :
    SelectableGroupWidget(parent, maxRow, maxColumn) {

    iWidgetArray = new QVector<WidgetVector*>(maxRow);
    for(int i = 0; i < maxRow; i++) {
        (*iWidgetArray)[i] = new WidgetVector(maxColumn);
        for(int j = 0; j < maxColumn; j++) {
            WidgetVector& row = *((*iWidgetArray)[i]);
            row[j] =  new SelectableWidget(this, *this, *(new Selectable(this)));
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

SelectableTests::SelectableTests(QObject *parent) :
    QObject(parent)
{

}

void SelectableTests::select_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<int>("numSelected");
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
                                << maxRows * maxColumns
                                << points;
}

void SelectableTests::select() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(int, numSelected);
    QFETCH(PointList, selectedPoints);

    SelectableGroupTestWidget* groupWidget = new SelectableGroupTestWidget(NULL, maxRow, maxColumn);

    for(int i = 0; i < selectedPoints.count(); i++) {
        groupWidget->select(groupWidget->widgetAt(selectedPoints.at(i).y(), selectedPoints.at(i).x()), true);
    }

    QList<SelectableWidget*> selectedList = groupWidget->selectedItems();

    QCOMPARE(selectedList.count(), numSelected);

    compareSelected(groupWidget, selectedList, selectedPoints);
}

void SelectableTests::selectOne_data() {  
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

void SelectableTests::selectOne() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, selectedPoint);

    SelectableGroupTestWidget* groupWidget = new SelectableGroupTestWidget(NULL, maxRow, maxColumn);

    groupWidget->selectOne(groupWidget->widgetAt(selectedPoint.y(), selectedPoint.x()));

    QList<SelectableWidget*> selectedList = groupWidget->selectedItems();

    QCOMPARE(selectedList.count(), 1);

    int row;
    int column;

    SelectableWidget* selected = selectedList.at(0);
    groupWidget->getWidgetPosition(*selected, &row, &column);

    QPoint selPoint(column, row);

    QCOMPARE(selPoint, selectedPoint);
}

void SelectableTests::selectArea_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("firstSelected");
    QTest::addColumn<QPoint>("secondSelected");

    int maxRows;
    int maxColumns;

    maxRows = 2;
    maxColumns = 3;

    QTest::newRow("select all") << maxRows
                                << maxColumns
                                << QPoint(0,0)
                                << QPoint(maxColumns - 1, maxRows - 1);
}

void SelectableTests::selectArea() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstSelected);
    QFETCH(QPoint, secondSelected);

    SelectableGroupTestWidget* groupWidget = new SelectableGroupTestWidget(NULL, maxRow, maxColumn);

    groupWidget->selectOne(groupWidget->widgetAt(firstSelected.y(), firstSelected.x()));
    groupWidget->selectArea(groupWidget->widgetAt(secondSelected.y(), secondSelected.x()));

    QList<SelectableWidget*> selectedList = groupWidget->selectedItems();

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

    int numSelected = (bottomRow - topRow) * (rightColumn - leftColumn);

    QCOMPARE(selectedList.count(), numSelected);

    // TODO how to test

}

void SelectableTests::selectDirection_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("firstSelected");
    QTest::addColumn<IntList>("selectedDirections");
    QTest::addColumn<PointList>("selectedPoints");

    int maxRows;
    int maxColumns;

    maxRows = 3;
    maxColumns = 3;

    IntList directions;
    PointList selectedPoints;

    directions.append(Qt::Key_Down);

    selectedPoints.append(QPoint(1, 1));
    selectedPoints.append(QPoint(2, 1));

    QTest::newRow("select down") << maxRows
                                 << maxColumns
                                 << QPoint(1,1)
                                 << directions
                                 << selectedPoints;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Up);

    selectedPoints.append(QPoint(1, 1));
    selectedPoints.append(QPoint(0, 1));

    QTest::newRow("select up") << maxRows
                                 << maxColumns
                                 << QPoint(1,1)
                                 << directions
                                 << selectedPoints;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Left);

    selectedPoints.append(QPoint(1, 1));
    selectedPoints.append(QPoint(1, 0));

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

    QTest::newRow("select right") << maxRows
                                 << maxColumns
                                 << QPoint(1,1)
                                 << directions
                                 << selectedPoints;
}

void SelectableTests::selectDirection() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstSelected);
    QFETCH(IntList, selectedDirections);
    QFETCH(PointList, selectedPoints);

    SelectableGroupTestWidget* groupWidget = new SelectableGroupTestWidget(NULL, maxRow, maxColumn);

    groupWidget->selectOne(groupWidget->widgetAt(firstSelected.y(), firstSelected.x()));

    int direction;
    foreach(direction, selectedDirections) {
        groupWidget->selectDirection(direction);
    }

    QList<SelectableWidget*> selectedList = groupWidget->selectedItems();

    QCOMPARE(selectedList.count(), selectedPoints.count());

    compareSelected(groupWidget, selectedList, selectedPoints);
}

void SelectableTests::compareSelected(SelectableGroupTestWidget* groupWidget, QList<SelectableWidget*> selectedList, QList<QPoint> selectedPoints) {
    int row;
    int column;

    SelectableWidget* selected = NULL;
    for(int i = 0; i < selectedList.count(); i++) {
        selected = selectedList.at(i);
        groupWidget->getWidgetPosition(*selected, &row, &column);

        QPoint selPoint(column, row);

        QCOMPARE(selPoint.x(), selectedPoints.at(i).x());
        QCOMPARE(selPoint.y(), selectedPoints.at(i).y());
    }
}
