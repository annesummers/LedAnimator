/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "selectablewidgettestbase.h"

using namespace AnimatorTest;

SelectableWidgetTestBase::SelectableWidgetTestBase(QObject* parent) :
    QObject(parent)
{
}

void SelectableWidgetTestBase::calculateAreaPoints(QList<Position>& selectedPoints, Position firstSelected, Position secondSelected) {
    int leftColumn;
    int rightColumn;
    int topRow;
    int bottomRow;

    if(firstSelected.row() > secondSelected.row()) {
        topRow = secondSelected.row();
        bottomRow = firstSelected.row();
    } else {
        topRow = firstSelected.row();
        bottomRow = secondSelected.row();
    }

    if(firstSelected.column() > secondSelected.column()) {
        leftColumn = secondSelected.column();
        rightColumn = firstSelected.column();
    } else {
        leftColumn = firstSelected.column();
        rightColumn = secondSelected.column();
    }

    for(int i = topRow; i < bottomRow + 1; i++) {
        for(int j = leftColumn; j < rightColumn + 1; j++) {
            selectedPoints.append(Position(i, j));
        }
    }
}

void SelectableWidgetTestBase::compareAreaPoints(SelectableGroupWidget* groupWidget, QList<Position> selectedPoints) {
    QList<SelectableWidget*> selectedList = groupWidget->selectedItems();

    QCOMPARE(selectedList.count(), selectedPoints.count());

    for(int i = 0; i < selectedPoints.count(); i++) {
        SelectableWidget& widget = groupWidget->widgetAt(selectedPoints.at(i));

        bool contains = selectedList.contains(&widget);
        QCOMPARE(contains, true);
    }

}
