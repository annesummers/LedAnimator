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
#include "exceptions.h"

using namespace Exception;
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
#ifndef NDEBUG
    if(row < 0) {
        throw IllegalArgumentException("Row is negative");
    }

    if(row > iMaxRow) {
        throw IllegalArgumentException("Row is greater than number of rows");
    }

    if(column < 0) {
        throw IllegalArgumentException("Column is negative");
    }

    if(column >= iMaxColumn) {
        throw IllegalArgumentException("Column is greater than number of columns");
    }
#endif

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

void SelectableGroupWidget::selectDirection(int direction) {
    switch(direction) {
    case Qt::Key_Up:
        selectUp();
        break;
    case Qt::Key_Down:
        selectDown();
        break;
    case Qt::Key_Left:
        selectLeft();
        break;
    case Qt::Key_Right:
        selectRight();
        break;
    }
}

void SelectableGroupWidget::selectRight() {
    if(iFirstSelectedRow == INVALID ||
       iFirstSelectedColumn == INVALID) {
        return;
    }

    bool selecting = false;

    if(iFirstSelectedColumn == iSelectedRightColumn &&
       iFirstSelectedColumn == iSelectedLeftColumn) {
            // selecting right
            selecting = true;
    }

    if(iFirstSelectedColumn < iSelectedRightColumn) {
        // selecting right
        selecting = true;
    }

    if(selecting) {
        iSelectedLeftColumn = iFirstSelectedColumn;

        if(iSelectedRightColumn + 1 < iMaxColumn) {
            iSelectedRightColumn++;
        }

        selectArea(iSelectedTopRow,
                   iSelectedBottomRow,
                   iSelectedLeftColumn,
                   iSelectedRightColumn);
    } else {
        // unselecting right
        iSelectedLeftColumn++;
        iSelectedRightColumn = iFirstSelectedColumn;

        selectArea(iSelectedTopRow,
                   iSelectedBottomRow,
                   iSelectedRightColumn,
                   iSelectedLeftColumn);
    }
}

void SelectableGroupWidget::selectLeft() {
    if(iFirstSelectedRow == INVALID ||
       iFirstSelectedColumn == INVALID) {
        return;
    }

    bool selecting = false;

    if(iFirstSelectedColumn == iSelectedLeftColumn &&
       iFirstSelectedColumn == iSelectedRightColumn) {
            selecting = true;
    }

    if(iFirstSelectedColumn > iSelectedLeftColumn) {
        selecting = true;
    }

    if(selecting) {
        // selecting left
        if(iSelectedLeftColumn - 1 >= 0) {
            iSelectedLeftColumn--;
        }

        iSelectedRightColumn = iFirstSelectedColumn;

        selectArea(iSelectedTopRow,
                   iSelectedBottomRow,
                   iSelectedRightColumn,
                   iSelectedLeftColumn);
    } else {
        // unselecting left
        iSelectedRightColumn--;
        iSelectedLeftColumn = iFirstSelectedColumn;

        selectArea(iSelectedTopRow,
                   iSelectedBottomRow,
                   iSelectedRightColumn,
                   iSelectedLeftColumn);
    }
}

void SelectableGroupWidget::selectUp() {
    if(iFirstSelectedRow == INVALID ||
       iFirstSelectedColumn == INVALID) {
        return;
    }

    bool selecting = false;

    if((iFirstSelectedRow == iSelectedTopRow &&
        iFirstSelectedRow == iSelectedBottomRow) ||
        iFirstSelectedRow > iSelectedTopRow) {
        selecting = true;
    }

    if(selecting) {
        // selecting up
        if(iSelectedTopRow - 1 >= 0) {
            iSelectedTopRow --;
        }

        iSelectedBottomRow = iFirstSelectedRow;

        selectArea(iSelectedBottomRow,
                   iSelectedTopRow,
                   iSelectedLeftColumn,
                   iSelectedRightColumn);
    } else {
        // unselecting up
        iSelectedBottomRow--;
        iSelectedTopRow = iFirstSelectedRow;

        selectArea(iSelectedBottomRow,
                   iSelectedTopRow,
                   iSelectedLeftColumn,
                   iSelectedRightColumn);
    }
}

void SelectableGroupWidget::selectDown() {
    if(iFirstSelectedRow == INVALID ||
       iFirstSelectedColumn == INVALID) {
        return;
    }

    bool selecting = false;

    if((iFirstSelectedRow == iSelectedTopRow &&
        iFirstSelectedRow == iSelectedBottomRow) ||
        iFirstSelectedRow < iSelectedBottomRow) {
        selecting = true;
    }

    if(selecting) {
        // selecting down
        iSelectedTopRow = iFirstSelectedRow;

        if(iSelectedBottomRow + 1 < iMaxRow) {
            iSelectedBottomRow++;
        }

        selectArea(iSelectedTopRow,
                   iSelectedBottomRow,
                   iSelectedLeftColumn,
                   iSelectedRightColumn);

    } else {     
        // unselecting down
        iSelectedBottomRow = iFirstSelectedRow;
        iSelectedTopRow++;

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
