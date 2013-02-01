#include "colourwidgettests.h"

#include <QMap>
#include <QString>

#include "exceptions.h"
#include "defaults.h"

using namespace Test;
using namespace Exception;

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

bool ColourGroupTestWidget::validKeyPress(Qt::Key key) {
    return (key & Qt::Key_Right) != 0 ||
           (key & Qt::Key_Left) != 0 ||
           (key & Qt::Key_Up) != 0 ||
           (key & Qt::Key_Down) != 0;
}

ColourWidgetTests::ColourWidgetTests(QObject *parent) :
    QObject(parent),
    iColourDialog(NULL) {
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
        groupWidget->select(groupWidget->widgetAt(selectedPoints.at(i).y(), selectedPoints.at(i).x()), true);
    }

    compareSelected(groupWidget, selectedPoints);
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

    compareSelected(groupWidget, selPoint);
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

    calculateSelectedPoints(selectedPoints, firstSelected, secondSelected);
    compareSelected(groupWidget, selectedPoints);

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

    compareSelected(groupWidget, selectedPoints);

    delete groupWidget;
}

void ColourWidgetTests::fade_data() {

}

void ColourWidgetTests::fade() {
    //QFETCH(int, maxRow);
    //QFETCH(int, maxColumn);
}

void ColourWidgetTests::clickOne_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("selectedPoint");

    int maxRows;
    int maxColumns;

    maxRows = 2;
    maxColumns = 3;

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

    compareSelected(groupWidget, selPoint);

    delete groupWidget;
}


void ColourWidgetTests::clickOneClickOne_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("firstPoint");
    QTest::addColumn<QPoint>("secondPoint");
    QTest::addColumn<QPoint>("selectedPoint");

    int maxRows;
    int maxColumns;

    maxRows = 2;
    maxColumns = 3;

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

    compareSelected(groupWidget, selectedPoints);

    delete groupWidget;
}

void ColourWidgetTests::doubleClickOne_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("selectedPoint");

    int maxRows;
    int maxColumns;

    maxRows = 2;
    maxColumns = 3;

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

    compareSelected(groupWidget, selPoint);

    delete groupWidget;
}

void ColourWidgetTests::doubleClickOneClickOne_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("firstPoint");
    QTest::addColumn<QPoint>("secondPoint");
    QTest::addColumn<QPoint>("selectedPoint");

    int maxRows;
    int maxColumns;

    maxRows = 2;
    maxColumns = 3;

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

    compareSelected(groupWidget, selPoint);

    delete groupWidget;
}

void ColourWidgetTests::clickOneDoubleClickOne_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("firstPoint");
    QTest::addColumn<QPoint>("secondPoint");
    QTest::addColumn<QPoint>("selectedPoint");

    int maxRows;
    int maxColumns;

    maxRows = 2;
    maxColumns = 3;

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

    compareSelected(groupWidget, selPoint);

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

    compareSelected(groupWidget, selPoint);

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

    compareSelected(groupWidget, selectedPoints);

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

    compareSelected(groupWidget, selectedPoints);
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

    compareSelected(groupWidget, selectedPoints);
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

    calculateSelectedPoints(selectedPoints, firstSelected, secondSelected);
    compareSelected(groupWidget, selectedPoints);

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

    compareSelected(groupWidget, selectedPoints);

    delete groupWidget;
}

void ColourWidgetTests::clickShiftDirectionClick_data() {
    QTest::addColumn<int>("maxRow");
    QTest::addColumn<int>("maxColumn");
    QTest::addColumn<QPoint>("firstSelected");
    QTest::addColumn<IntList>("selectedDirections");
    QTest::addColumn<QPoint>("secondSelected");
    QTest::addColumn<QPoint>("selectedPoint");

    int maxRows;
    int maxColumns;

    // single

    maxRows = 3;
    maxColumns = 3;

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

    compareSelected(groupWidget, selectedPoints);

    delete groupWidget;
}

void ColourWidgetTests::dragDropOneInternal_data() {
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

    ColourTestWidget& target = (ColourTestWidget&)(groupWidget->widgetAt(dropTarget.y(), dropTarget.x()));

    QTest::mouseMove(&target);
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

void ColourWidgetTests::copyPasteOneExternal_data() {

}

void ColourWidgetTests::copyPasteOneExternal() {

}

void ColourWidgetTests::copyPasteManyExternal_data() {

}

void ColourWidgetTests::copyPasteManyExternal() {

}

void ColourWidgetTests::copyPasteOneInternal_data() {

}

void ColourWidgetTests::copyPasteOneInternal() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, copyPoint);
    QFETCH(QPoint, pastePoint);

    //TODO testing copy and paste using menus

   /* ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

    ColourWidget& copyWidget = groupWidget->widgetAt(copyPoint.y(), copyPoint.x());
    copyWidget.setColour(Qt::black);
    copyWidget.copy();

    ColourWidget& pasteWidget = groupWidget->widgetAt(pastePoint.y(), pastePoint.x());
    copyWidget.setColour(Qt::white);
    pasteWidget.paste();

    QCOMPARE(copyWidget.colour(), pasteWidget.colour());*/
}

void ColourWidgetTests::copyPasteManyInternal_data() {

}

void ColourWidgetTests::copyPasteManyInternal() {
    QFETCH(int, maxRow);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, copyTopLeft);
    QFETCH(QPoint, copyBottomRight);
    QFETCH(QPoint, pastePoint);
    QFETCH(QList<QPoint>, pastedPoints);

   /* ColourGroupTestWidget* groupWidget = new ColourGroupTestWidget(NULL, maxRow, maxColumn);

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
    }*/
}

void ColourWidgetTests::doubleClickWidgetAndDismissDialog(ColourGroupTestWidget& groupWidget, QPoint widgetPoint) {
    ColourTestWidget& widget = (ColourTestWidget&)(groupWidget.widgetAt(widgetPoint.y(), widgetPoint.x()));

    iColourDialog = &(widget.colourDialog());
    QTimer::singleShot(1000, this, SLOT(dismissDialog()));

    QTest::mouseDClick(&widget, Qt::LeftButton);
}

void ColourWidgetTests::dismissDialog() {
    QTest::keyClick(iColourDialog, Qt::Key_Escape);
}

void ColourWidgetTests::calculateSelectedPoints(QList<QPoint>& selectedPoints, QPoint firstSelected, QPoint secondSelected) {
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

            qDebug("%d, %d", i, j);
        }
    }
}

void ColourWidgetTests::compareSelected(ColourGroupTestWidget* groupWidget, QList<QPoint> selectedPoints) {
    QList<ColourWidget*> selectedList = groupWidget->selectedItems();

    QCOMPARE(selectedList.count(), selectedPoints.count());

    int row;
    int column;

    ColourWidget* selected = NULL;
    foreach(selected, selectedList) {
        groupWidget->getWidgetPosition(*selected, &row, &column);

        QPoint selPoint(column, row);
        bool contains = selectedPoints.contains(selPoint);
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
