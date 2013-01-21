#include "colourwidgettests.h"

#include <QMap>

using namespace Test;

ColourGroupTestWidget::ColourGroupTestWidget(QWidget *parent, int maxRow, int maxColumn) :
    ColourGroupWidget(parent, maxRow, maxColumn) {

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

ColourWidgetTests::ColourWidgetTests(QObject *parent) :
    QObject(parent) {
}

void ColourWidgetTests::select_data() {
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

void ColourWidgetTests::select() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(int, numSelected);
    QFETCH(PointList, selectedPoints);

    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    for(int i = 0; i < selectedPoints.count(); i++) {
        groupWidget->select(groupWidget->widgetAt(selectedPoints.at(i).y(), selectedPoints.at(i).x()), true);
    }

    QList<ColourWidget*> selectedList = groupWidget->selectedItems();

    QCOMPARE(selectedList.count(), numSelected);

    compareSelected(groupWidget, selectedList, selectedPoints);
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

    // TODO
    // select invalid points
}

void ColourWidgetTests::selectOne() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, selectedPoint);

    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    groupWidget->selectOne(groupWidget->widgetAt(selectedPoint.y(), selectedPoint.x()));

    QList<ColourWidget*> selectedList = groupWidget->selectedItems();

    QCOMPARE(selectedList.count(), 1);

    int row;
    int column;

    ColourWidget* selected = selectedList.at(0);
    groupWidget->getWidgetPosition(*selected, &row, &column);

    QPoint selPoint(column, row);

    QCOMPARE(selPoint, selectedPoint);
}

void ColourWidgetTests::selectArea_data() {
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

    // TODO
    // select invalid points
    // select row
    // select coloumn
}

void ColourWidgetTests::selectArea() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstSelected);
    QFETCH(QPoint, secondSelected);

    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    groupWidget->selectOne(groupWidget->widgetAt(firstSelected.y(), firstSelected.x()));
    groupWidget->selectArea(groupWidget->widgetAt(secondSelected.y(), secondSelected.x()));

    QList<ColourWidget*> selectedList = groupWidget->selectedItems();

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

    QList<ColourWidget*> selectedTest;

    for(int i = topRow; i < bottomRow + 1; i++) {
        for(int j = leftColumn; j < rightColumn + 1; j++) {
            selectedTest.append(&(groupWidget->widgetAt(i, j)));
        }
    }

    ColourWidget* widget;
    foreach(widget, selectedList) {
        QCOMPARE(selectedTest.contains(widget), true);
    }
}

void ColourWidgetTests::selectDirection_data() {
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
    selectedPoints.append(QPoint(2, 3));
    selectedPoints.append(QPoint(1, 2));
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
    selectedPoints.append(QPoint(2, 1));
    selectedPoints.append(QPoint(1, 2));
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
    selectedPoints.append(QPoint(1, 3));
    selectedPoints.append(QPoint(2, 3));

    QTest::newRow("select area left down") << maxRows
                                 << maxColumns
                                 << QPoint(2,2)
                                 << directions
                                 << selectedPoints;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Down);
    directions.append(Qt::Key_Left);

    selectedPoints.append(QPoint(2, 2));
    selectedPoints.append(QPoint(2, 3));
    selectedPoints.append(QPoint(1, 2));
    selectedPoints.append(QPoint(1, 3));

    QTest::newRow("select area down left") << maxRows
                                 << maxColumns
                                 << QPoint(2,2)
                                 << directions
                                 << selectedPoints;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Down);
    directions.append(Qt::Key_Right);

    selectedPoints.append(QPoint(2, 2));
    selectedPoints.append(QPoint(2, 3));
    selectedPoints.append(QPoint(3, 2));
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
    selectedPoints.append(QPoint(2, 1));
    selectedPoints.append(QPoint(1, 1));
    selectedPoints.append(QPoint(1, 2));

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
    selectedPoints.append(QPoint(2, 3));
    selectedPoints.append(QPoint(3, 2));
    selectedPoints.append(QPoint(3, 3));

    QTest::newRow("select area right down") << maxRows
                                 << maxColumns
                                 << QPoint(2,2)
                                 << directions
                                 << selectedPoints;

    directions.clear();
    selectedPoints.clear();

    directions.append(Qt::Key_Down);
    directions.append(Qt::Key_Right);

    selectedPoints.append(QPoint(2, 2));
    selectedPoints.append(QPoint(2, 3));
    selectedPoints.append(QPoint(3, 2));
    selectedPoints.append(QPoint(3, 3));

    QTest::newRow("select area down right") << maxRows
                                 << maxColumns
                                 << QPoint(2,2)
                                 << directions
                                 << selectedPoints;

    // TODO
    // select then select perpendicular then unslect in other direction *4
    // select then select perpendicular then unslect in other direction then select*4
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
        groupWidget->selectDirection(direction);
    }

    QList<ColourWidget*> selectedList = groupWidget->selectedItems();

    QCOMPARE(selectedList.count(), selectedPoints.count());

    compareSelected(groupWidget, selectedList, selectedPoints);
}

void ColourWidgetTests::fade_data() {

}

void ColourWidgetTests::fade() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
}

void ColourWidgetTests::copyPasteOne_data() {


}

void ColourWidgetTests::copyPasteOne() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, copyPoint);
    QFETCH(QPoint, pastePoint);

    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    ColourWidget& copyWidget = groupWidget->widgetAt(copyPoint.y(), copyPoint.x());
    copyWidget.setColour(Qt::black);
    copyWidget.copy();

    ColourWidget& pasteWidget = groupWidget->widgetAt(pastePoint.y(), pastePoint.x());
    copyWidget.setColour(Qt::white);
    pasteWidget.paste();

    QCOMPARE(copyWidget.colour(), pasteWidget.colour());
}

void ColourWidgetTests::copyPasteMany_data() {

}

void ColourWidgetTests::copyPasteMany() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, copyTopLeft);
    QFETCH(QPoint, copyBottomRight);
    QFETCH(QPoint, pastePoint);
    QFETCH(QList<QPoint>, pastedPoints);

    ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    groupWidget->selectOne(groupWidget->widgetAt(0,0));
    groupWidget->selectArea(widgetAt(maxRow - 1, maxColumn -1);
    groupWidget->setColour(Qt::white);

    ColourWidget& topLeftWidget = groupWidget->widgetAt(copyTopLeft.y(), copyTopLeft.x());

    groupWidget->selectOne(topLeftWidget);
    groupWidget->selectArea(widgetAt(copyBottomRight.y(), copyBottomRight.x()));
    groupWidget->setColour(Qt::black);

    topLeftWidget.copy();

    ColourWidget& pastePointWidget = groupWidget->widgetAt(pastePoint.y(), pastePoint().x());

    pastePointWidget.paste();

    QPoint pastedPoint;
    foreach(pastedPoint, pastedPoints) {
        ColourWidget& pastedWidget = groupWidget->widgetAt(pastedPoint.y(), pastedPoint.x());

        QCOMPARE(pastedWidget.colour(), Qt::black);
        QCOMPARE(pastedWidget.selected(), true);
    }
}

void ColourWidgetTests::compareSelected(ColourGroupTestWidget* groupWidget, QList<ColourWidget*> selectedList, QList<QPoint> selectedPoints) {
    int row;
    int column;

    ColourWidget* selected = NULL;
    for(int i = 0; i < selectedList.count(); i++) {
        selected = selectedList.at(i);
        groupWidget->getWidgetPosition(*selected, &row, &column);

        QPoint selPoint(column, row);

        QCOMPARE(selPoint.x(), selectedPoints.at(i).x());
        QCOMPARE(selPoint.y(), selectedPoints.at(i).y());
    }
}
