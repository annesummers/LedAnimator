/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "selectablegroupwidget.h"

#include "animation.h"
#include "selectablewidget.h"
#include "selectable.h"

#include "defaults.h"

using namespace Ui;

SelectableGroupWidget::SelectableGroupWidget(QWidget *parent, int maxRow, int maxColumn) :
    QWidget(parent),
    iMaxRow(maxRow),
    iMaxColumn(maxColumn) {
}

void SelectableGroupWidget::clearSelection() {
    SelectableWidget* selectable;
    foreach(selectable, iSelected) {
        selectable->item().select(false);
    }

    iSelected.clear();
}

void SelectableGroupWidget::select(SelectableWidget &selectable, bool selected) {
    if(selected) {
        iSelected.append(&selectable);
    } else {
        iSelected.removeOne(&selectable);
    }

    selectable.item().select(selected);
}

void SelectableGroupWidget::selectOne(SelectableWidget &selectable) {
    bool selected = false;

    if(iSelected.count() > 1) {
        selected = true;
    }

    if(iSelected.count() == 1 && iSelected.at(0) != &selectable) {
        selected = true;
    }

    if(iSelected.isEmpty()) {
        selected = true;
    }

    clearSelection();

    if(selected){
        select(selectable, selected);

        int row;
        int column;

        getWidgetPosition(selectable, &row, &column);
        setFirstSelected(row, column);
    } else {
        setFirstSelected(INVALID, INVALID);
    }
}

void SelectableGroupWidget::setFirstSelected(int row, int column) {
    iFirstSelectedRow = row;
    iFirstSelectedColumn = column;

    iSelectedTopRow = iFirstSelectedRow;
    iSelectedBottomRow = iFirstSelectedRow;
    iSelectedLeftColumn = iFirstSelectedColumn;
    iSelectedRightColumn = iFirstSelectedColumn;
}

void SelectableGroupWidget::selectArea(SelectableWidget& widget) {
    if(iFirstSelectedRow != INVALID &&
       iFirstSelectedColumn != INVALID) {
        int row;
        int column;

        getWidgetPosition(widget, &row, &column);
        selectArea(iFirstSelectedRow, row, iFirstSelectedColumn, column);
    }
}

void SelectableGroupWidget::selectRight(int column) {
    if(iFirstSelectedRow == INVALID ||
       iFirstSelectedColumn == INVALID) {
        return;
    }

    if(iFirstSelectedColumn <= column) {
        // selecting right
        iSelectedLeftColumn = iFirstSelectedColumn;

        if(column + 1 < iMaxColumn) {
            iSelectedRightColumn = column + 1;
        } else {
            iSelectedRightColumn = column;
        }

        selectArea(iSelectedTopRow,
                   iSelectedBottomRow,
                   iSelectedLeftColumn,
                   iSelectedRightColumn);
    } else {
        // unselecting right
        iSelectedLeftColumn = column + 1;
        iSelectedRightColumn = iFirstSelectedColumn;

        selectArea(iSelectedTopRow,
                   iSelectedBottomRow,
                   iSelectedRightColumn,
                   iSelectedLeftColumn);
    }
}

void SelectableGroupWidget::selectLeft(int column) {
    if(iFirstSelectedRow == INVALID ||
       iFirstSelectedColumn == INVALID) {
        return;
    }

    if(iFirstSelectedColumn >= column) {
        // selecting left
        if(column - 1 >= 0) {
            iSelectedLeftColumn = column - 1;
        } else {
            iSelectedLeftColumn = column;
        }

        iSelectedRightColumn = iFirstSelectedColumn;

        selectArea(iSelectedTopRow,
                   iSelectedBottomRow,
                   iSelectedRightColumn,
                   iSelectedLeftColumn);
    } else {
        // unselecting left
        iSelectedLeftColumn = column - 1;
        iSelectedRightColumn = iFirstSelectedColumn;

        selectArea(iSelectedTopRow,
                   iSelectedBottomRow,
                   iSelectedRightColumn,
                   iSelectedLeftColumn);
    }
}

void SelectableGroupWidget::selectUp(int row) {
    if(iFirstSelectedRow == INVALID ||
       iFirstSelectedColumn == INVALID) {
        return;
    }

    if(iFirstSelectedRow >= row) {
        // selecting up
        if(row - 1 >= 0) {
            iSelectedTopRow = row - 1;
        } else {
            iSelectedTopRow = 0;
        }

        iSelectedBottomRow = iFirstSelectedRow;

        selectArea(iSelectedBottomRow,
                   iSelectedTopRow,
                   iSelectedLeftColumn,
                   iSelectedRightColumn);
    } else {
        // unselecting up
        iSelectedTopRow = row - 1;
        iSelectedBottomRow = iFirstSelectedRow;

        selectArea(iSelectedBottomRow,
                   iSelectedTopRow,
                   iSelectedLeftColumn,
                   iSelectedRightColumn);
    }
}

void SelectableGroupWidget::selectDown(int row) {
    if(iFirstSelectedRow == INVALID ||
       iFirstSelectedColumn == INVALID) {
        return;
    }

    if(iFirstSelectedRow <= row) {
        // selecting down
        iSelectedTopRow = iFirstSelectedRow;

        if(row + 1 < iMaxRow) {
            iSelectedBottomRow = row + 1;
        } else {
            iSelectedBottomRow = row;
        }

        selectArea(iSelectedTopRow,
                   iSelectedBottomRow,
                   iSelectedLeftColumn,
                   iSelectedRightColumn);

    } else {     
        // unselecting down
        iSelectedTopRow = row + 1;
        iSelectedBottomRow = iFirstSelectedRow;

        selectArea(iSelectedBottomRow,
                   iSelectedTopRow,
                   iSelectedLeftColumn,
                   iSelectedRightColumn);
    }
}

void SelectableGroupWidget::selectArea(int rowStart, int rowEnd, int columnStart, int columnEnd) {
    clearSelection();

    if(rowStart < rowEnd) {
        if(columnStart < columnEnd) {
            for(int i = rowStart; i < rowEnd + 1; i++) {
                for(int j = columnStart; j < columnEnd + 1; j++) {
                    select(widgetAt(i, j), true);
                }
            }
        } else {
            for(int i = rowStart; i < rowEnd + 1; i++) {
                for(int j = columnStart; j > columnEnd - 1; j--) {
                    select(widgetAt(i, j), true);
                }
            }
        }
    } else {
        if(columnStart < columnEnd) {
            for(int i = rowStart; i > rowEnd - 1; i--) {
                for(int j = columnStart; j < columnEnd + 1; j++) {
                    select(widgetAt(i, j), true);
                }
            }
        } else {
            for(int i = rowStart; i > rowEnd - 1; i--) {
                for(int j = columnStart; j > columnEnd - 1; j--) {
                    select(widgetAt(i, j), true);
                }
            }
        }
    }
}
