#include "selectablewidgettestbase.h"

using namespace AnimatorTest;

SelectableWidgetTestBase::SelectableWidgetTestBase(QObject *parent) :
    QObject(parent)
{
}

void SelectableWidgetTestBase::calculateAreaPoints(QList<QPoint>& selectedPoints, QPoint firstSelected, QPoint secondSelected) {
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

void SelectableWidgetTestBase::compareAreaPoints(SelectableGroupWidget* groupWidget, QList<QPoint> selectedPoints) {
    QList<SelectableWidget*> selectedList = groupWidget->selectedItems();

    QCOMPARE(selectedList.count(), selectedPoints.count());

    for(int i = 0; i < selectedPoints.count(); i++) {
        SelectableWidget& widget = groupWidget->widgetAt(selectedPoints.at(i).y(), selectedPoints.at(i).x());

        bool contains = selectedList.contains(&widget);
        QCOMPARE(contains, true);
    }

}
