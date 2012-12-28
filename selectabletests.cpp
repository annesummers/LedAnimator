#include "selectabletests.h"

using namespace Test;

SelectableGroupTestWidget::SelectableGroupTestWidget(QWidget *parent, int minRow, int maxRow, int minColumn, int maxColumn) :
    SelectableGroupWidget(parent, minRow, maxRow, minColumn, maxColumn) {

    for(int i = minRow; i < maxRow; i++) {
        iWidgetArray.append(new QList<SelectableWidget*>);
        for(int j = minColumn; j < maxColumn; j++) {
            iWidgetArray.at(i).append(new SelectableWidget(this, *this, *(new Selectable(this))));
        }
    }
}

SelectableTests::SelectableTests(QObject *parent) :
    QObject(parent)
{
}

void SelectableTests::select_data() {

}

void SelectableTests::select() {
    QFETCH(int, minRow);
    QFETCH(int, maxRow);
    QFETCH(int, minColumn);
    QFETCH(int, maxColumn);
    QFETCH(int, numSelected);
    QFETCH(QList<QPoint>, whichSelected);

    SelectableGroupTestWidget* groupWidget = new SelectableGroupTestWidget(NULL, minRow, maxRow, minColumn, maxColumn);

    QPoint point;
    foreach(point, whichSelected){
        groupWidget->select(groupWidget->widgetAt(point.y(), point.x()), true);
    }

    QList<SelectableWidget*> selectedList = groupWidget->selectedItems();

    QCOMPARE(selectedList.count(), numSelected);

    int row;
    int column;

    SelectableWidget* selected = NULL;
    for(int i = 0; i < selectedList.count(); i++) {
        selected = selectedList.at(i);
        groupWidget->getWidgetPosition(*selected, &row, &column);

        QPoint selPoint(column, row);

        QCOMPARE(selPoint, whichSelected.at(i));
    }
}

void SelectableTests::selectOne_data() {

}

void SelectableTests::selectOne() {
    QFETCH(int, minRow);
    QFETCH(int, maxRow);
    QFETCH(int, minColumn);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, whichSelected);

    SelectableGroupTestWidget* groupWidget = new SelectableGroupTestWidget(NULL, minRow, maxRow, minColumn, maxColumn);


    groupWidget->selectOne(groupWidget->widgetAt(whichSelected.y(), whichSelected.x()));

    QList<SelectableWidget*> selectedList = groupWidget->selectedItems();

    QCOMPARE(selectedList.count(), 1);

    int row;
    int column;

    SelectableWidget* selected = selectedList.at(0);
    groupWidget->getWidgetPosition(*selected, &row, &column);

    QPoint selPoint(column, row);

    QCOMPARE(selPoint, whichSelected);
}

void SelectableTests::selectArea_data() {

}

void SelectableTests::selectArea() {
    QFETCH(int, minRow);
    QFETCH(int, maxRow);
    QFETCH(int, minColumn);
    QFETCH(int, maxColumn);
    QFETCH(QPoint, firstSelected);
    QFETCH(QPoint, secondSelected);

    SelectableGroupTestWidget* groupWidget = new SelectableGroupTestWidget(NULL, minRow, maxRow, minColumn, maxColumn);


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
