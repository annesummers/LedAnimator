#include "selectablegroupwidget.h"

#include <QtGui>

#include "selectablewidget.h"
#include "selectablegroupgroupwidget.h"

#include "exceptions.h"
#include "constants.h"

using namespace AnimatorUi;
using namespace Exception;

SelectableGroupWidget::SelectableGroupWidget(QWidget *parent, int numRows, int numColumns, SelectableGroupGroupWidget *groupGroupWidget) :
    QWidget(parent),
    iGroupGroup(groupGroupWidget),
    iGroupNumber(0),
    iNumRows(numRows),
    iNumColumns(numColumns),
    iFirstSelectedRow(0),
    iLastSelectedRow(0),
    iFirstSelectedColumn(0),
    iLastSelectedColumn(0),
    iTopLeftSelectedRow(INVALID),
    iTopLeftSelectedColumn(INVALID),
    iBottomRightSelectedRow(INVALID),
    iBottomRightSelectedColumn(INVALID){

  if(iGroupGroup != NULL) {
      iGroupNumber = iGroupGroup->addGroup(*this);
  }
}

bool SelectableGroupWidget::isMultipleSelected() {
    return  iSelected.count() != 0 &&
            iFirstSelectedRow == INVALID &&
            iFirstSelectedColumn == INVALID &&
            iLastSelectedRow == INVALID &&
            iLastSelectedColumn == INVALID;
}

bool SelectableGroupWidget::isGroupSelected() {
    return  !isSingleSelected() &&
            !isMultipleSelected();
}

bool SelectableGroupWidget::isSingleSelected() {
    return !isMultipleSelected() &&
            iFirstSelectedRow == iLastSelectedRow &&
            iFirstSelectedColumn == iLastSelectedColumn;
}

// selecting ----------------------------

void SelectableGroupWidget::toggle(SelectableWidget &widget) {
    qDebug("select %d", !widget.isSelected());
    doSelect(widget, !widget.isSelected());

    if(selectedCount() == 1) {
        setSingleSelected(widget);
    } else {
        clearGroupSelection();
    }
}

void SelectableGroupWidget::toggleOne(SelectableWidget &widget) {
    qDebug("toggleOne");
    bool selected = false;

    if(iSelected.count() > 1 ||
       (iSelected.count() == 1 &&
        iSelected.at(0) != &widget) ||
       iSelected.isEmpty()) {
        selected = true;
    }

    if(selected){
        selectOne(widget);
    } else {
        clearSelection();

        clearGroupSelection();
    }
}

void SelectableGroupWidget::selectOne(SelectableWidget &widget) {
    qDebug("selectOne");
    clearSelection();

    doSelect(widget, true);

    setSingleSelected(widget);
}

void SelectableGroupWidget::selectArea(SelectableWidget& widget) {
    qDebug("selectArea");
    if(!isSingleSelected()) {
        return;
    }

    getWidgetPosition(widget, &iLastSelectedRow, &iLastSelectedColumn);

    doGroupSelection();
}

void SelectableGroupWidget::selectDirection(Qt::Key direction) {
    if(!isSingleSelected() &&
       !isGroupSelected()) {
        return;
    }

    switch(direction) {
        case Qt::Key_Up:
            if(iLastSelectedRow - 1 >=0 ) {
                iLastSelectedRow--;
            }
            break;
        case Qt::Key_Down:
            if(iLastSelectedRow + 1 < iNumRows ) {
                iLastSelectedRow++;
            }
            break;
        case Qt::Key_Left:
            if(iLastSelectedColumn - 1 >=0) {
                iLastSelectedColumn--;
            }
            break;
        case Qt::Key_Right:
            if(iLastSelectedColumn + 1 < iNumColumns) {
                iLastSelectedColumn++;
            }
            break;
    default:
        break;
    }

    doGroupSelection();
}

void SelectableGroupWidget::setSingleSelected(SelectableWidget& widget) {
    int row;
    int column;

    getWidgetPosition(widget, &row, &column);

    iFirstSelectedRow = iLastSelectedRow = iTopLeftSelectedRow = iBottomRightSelectedRow = row;
    iFirstSelectedColumn = iLastSelectedColumn = iTopLeftSelectedColumn = iBottomRightSelectedColumn = column;
}

void SelectableGroupWidget::clearGroupSelection() {
    iFirstSelectedRow = iLastSelectedRow = iFirstSelectedColumn = iLastSelectedColumn = INVALID;
}

void SelectableGroupWidget::clearSelection() {
    SelectableWidget* widget;
    foreach(widget, iSelected) {
        widget->select(false);
    }

    iSelected.clear();
    iTopLeftSelectedRow = iBottomRightSelectedRow = iTopLeftSelectedColumn = iBottomRightSelectedColumn = INVALID;
}

void SelectableGroupWidget::doSelect(SelectableWidget &widget, bool selected) {
    int row;
    int column;
    getWidgetPosition(widget, &row, &column);

    if(selected) {
        QString widgetType = "";
        if(iSelected.count() != 0) {
            widgetType = iSelected.at(0)->objectName();
        }

        if(iSelected.count() == 0 ||
           widget.objectName() == widgetType) {

            if(iTopLeftSelectedRow == INVALID || row < iTopLeftSelectedRow ||
               iTopLeftSelectedColumn == INVALID || column < iTopLeftSelectedColumn) {
                iTopLeftSelectedRow = row;
                iTopLeftSelectedColumn = column;
                qDebug("ADD : topLeftRow is %d, topLeftColumn is %d", iTopLeftSelectedRow, iTopLeftSelectedColumn);

                iSelected.insert(0, &widget);
            } else {
                iSelected.append(&widget);
            }

            if(iGroupGroup != NULL) {
                iGroupGroup->selected(*this);
            }
        } else {
            selected = false;
        }
    } else {
        iSelected.removeOne(&widget);

        if(row == iTopLeftSelectedRow &&
           column == iTopLeftSelectedColumn) {
            iTopLeftSelectedRow = numRows() - 1;
            iTopLeftSelectedColumn = numColumns() - 1;

            if(iSelected.count() == 0) {
                iTopLeftSelectedRow = INVALID;
                iTopLeftSelectedColumn = INVALID;
                qDebug("REMOVE : topLeftRow is %d, topLeftColumn is %d", iTopLeftSelectedRow, iTopLeftSelectedColumn);

            } else {
                sortSelected();
            }
        }
    }

    widget.select(selected);
}

void SelectableGroupWidget::sortSelected() {
    int row;
    int column;
    SelectableWidget* widget;

    for(int i = 0; i < iSelected.count(); i++) {
        widget = iSelected.at(i);
        getWidgetPosition(*widget, &row, &column);

        if((row < iTopLeftSelectedRow &&
           column < iTopLeftSelectedColumn) ||
           (row == iTopLeftSelectedRow &&
            column < iTopLeftSelectedColumn) ||
           (row < iTopLeftSelectedRow &&
            column == iTopLeftSelectedColumn)) {
            iTopLeftSelectedRow = row;
            iTopLeftSelectedColumn = column;
            qDebug("SORT : topLeftRow is %d, topLeftColumn is %d", iTopLeftSelectedRow, iTopLeftSelectedColumn);

            iSelected.removeOne(widget);
            iSelected.insert(0, widget);

            i--;
        }
    }
}

void SelectableGroupWidget::doGroupSelection() {
    clearSelection();

    QString widgetType = widgetAt(iFirstSelectedRow, iFirstSelectedColumn).objectName();

    if(iFirstSelectedRow < iLastSelectedRow) {
        iTopLeftSelectedRow = iFirstSelectedRow;
        iBottomRightSelectedRow = iLastSelectedRow;

        if(iFirstSelectedColumn < iLastSelectedColumn) {
            iTopLeftSelectedColumn = iFirstSelectedColumn;
            iBottomRightSelectedColumn = iLastSelectedColumn;

            for(int i = iFirstSelectedRow; i < iLastSelectedRow + 1; i++) {
                for(int j = iFirstSelectedColumn; j < iLastSelectedColumn + 1; j++) {
                    SelectableWidget& widget = widgetAt(i, j);
                    if(widget.objectName() == widgetType) {
                        doSelect(widget, true);
                    }
                }
            }
        } else {
            iTopLeftSelectedColumn = iLastSelectedColumn;
            iBottomRightSelectedColumn = iFirstSelectedColumn;

            for(int i = iFirstSelectedRow; i < iLastSelectedRow + 1; i++) {
                for(int j = iFirstSelectedColumn; j > iLastSelectedColumn - 1; j--) {
                    SelectableWidget& widget = widgetAt(i, j);
                    if(widget.objectName() == widgetType) {
                        doSelect(widget, true);
                    }
                }
            }
        }
    } else {
        iTopLeftSelectedRow = iLastSelectedRow;
        iBottomRightSelectedRow = iFirstSelectedRow;

        if(iFirstSelectedColumn < iLastSelectedColumn) {
            iTopLeftSelectedColumn = iFirstSelectedColumn;
            iBottomRightSelectedColumn = iLastSelectedColumn;

            for(int i = iFirstSelectedRow; i > iLastSelectedRow - 1; i--) {
                for(int j = iFirstSelectedColumn; j < iLastSelectedColumn + 1; j++) {
                    SelectableWidget& widget = widgetAt(i, j);
                    if(widget.objectName() == widgetType) {
                        doSelect(widget, true);
                    }
                }
            }
        } else {
            iTopLeftSelectedColumn = iLastSelectedColumn;
            iBottomRightSelectedColumn = iFirstSelectedColumn;

            for(int i = iFirstSelectedRow; i > iLastSelectedRow - 1; i--) {
                for(int j = iFirstSelectedColumn; j > iLastSelectedColumn - 1; j--) {
                    SelectableWidget& widget = widgetAt(i, j);
                    if(widget.objectName() == widgetType) {
                        doSelect(widget, true);
                    }
                }
            }
        }
    }
}

void SelectableGroupWidget::getLeftRightTopBottomSelection(int * const topRow, int * const bottomRow, int * const leftColumn, int * const rightColumn) const {
    *topRow = iTopLeftSelectedRow;
    *bottomRow = iBottomRightSelectedRow;
    *leftColumn = iTopLeftSelectedColumn;
    *rightColumn = iBottomRightSelectedColumn;

    /**topRow = iFirstSelectedRow;
    *bottomRow = iLastSelectedRow;
    *leftColumn = iFirstSelectedColumn;
    *rightColumn = iLastSelectedColumn;

    if(iFirstSelectedRow > iLastSelectedRow) {
        *topRow = iLastSelectedRow;
        *bottomRow = iFirstSelectedRow;
    }

    if(iFirstSelectedColumn > iLastSelectedColumn) {
        *leftColumn = iLastSelectedColumn;
        *rightColumn = iFirstSelectedColumn;
    }*/
}


SelectableWidget& SelectableGroupWidget::widgetAt(int row, int column) {
#ifndef NDEBUG
    if(row < 0) {
        throw IllegalArgumentException("SelectableGroupWidget::widgetAt : Row is negative");
    }

    if(numRows() != 0 && row >= numRows()) {
        throw IllegalArgumentException("SelectableGroupWidget::widgetAt : Row is greater than number of rows");
    }

    if(column < 0) {
        throw IllegalArgumentException("SelectableGroupWidget::widgetAt : Column is negative");
    }

    if(numColumns() != 0 && column >= numColumns()) {
        throw IllegalArgumentException("SelectableGroupWidget::widgetAt : Column is greater than number of columns");
    }
#endif
}

void SelectableGroupWidget::getWidgetPosition(SelectableWidget &widget, int *row, int *column) {
     Q_UNUSED(widget);
#ifndef NDEBUG
    if(*row < 0) {
        throw IllegalArgumentException("SelectableGroupWidget::getWidgetPosition : Row is negative");
    }

    if(*row > iNumRows) {
        throw IllegalArgumentException("SelectableGroupWidget::getWidgetPosition : Row is greater than number of rows");
    }

    if(*column < 0) {
        throw IllegalArgumentException("SelectableGroupWidget::getWidgetPosition : Column is negative");
    }

    if(*column >= iNumColumns) {
        throw IllegalArgumentException("SelectableGroupWidget::getWidgetPosition : Column is greater than number of columns");
    }
#endif
}

// mime data handlers ------------------------

const QByteArray SelectableGroupWidget::writeMimeData() {
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

     dataStream << iSelected.count();
     dataStream << iGroupNumber;

     int row;
     int column;

     SelectableWidget* widget;
     foreach(widget, iSelected) {
         getWidgetPosition(*widget, &row, &column);
         dataStream << row << column;
     }

    return itemData;
}

void SelectableGroupWidget::handleMimeData(QByteArray itemData, SelectableWidget& dropWidget, bool move, bool wrap) {
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);

   // selectOne(dropWidget);

    int row;
    int column;

    getWidgetPosition(dropWidget, &row, &column);
    toggle(dropWidget);  // unselect the drop widget

    int numWidgets;

    dataStream >> numWidgets;

  /*  int rowSpan;
    int columnSpan;

    dataStream >> rowSpan >> columnSpan;*/

    int groupNumber;

    dataStream >> groupNumber;

    QList<int> newRows;
    QList<int> newColumns;

    int oldRow;
    int oldColumn;

    int newRow;
    int newColumn;

    int firstRow;
    int firstColumn;

    for(int i = 0; i < numWidgets; i++) {
        dataStream >> oldRow;
        dataStream >> oldColumn;

        if(i == 0) {
            firstRow = oldRow;
            firstColumn = oldColumn;
        }

        newRow = row + (oldRow - firstRow);
        if(numRows() != 0 && newRow >= numRows()) {
            if(wrap) {
                newRow = newRow - numRows();
            } else {
                newRow = INVALID;
            }

        }

        newColumn = column + (oldColumn - firstColumn);
        if(numColumns() != 0 && newColumn >= numColumns()) {
            if(wrap) {
                newColumn = newColumn - numColumns();
            } else {
                newColumn = INVALID;
            }
        }

        if(newRow != INVALID && newColumn != INVALID) {
            newRows.append(newRow);
            newColumns.append(newColumn);

            if(move) {
                moveItem(groupNumber, oldRow, oldColumn, newRow, newColumn);
            } else {
                copyItem(groupNumber, oldRow, oldColumn, newRow, newColumn);
            }
        }
    }

    for(int i = 0; i < newRows.count(); i++) {
        toggle(widgetAt(newRows.at(i), newColumns.at(i)));
    }
}

void SelectableGroupWidget::handleOldMimeData(QByteArray itemData) {
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);

    int numCopied;

    dataStream >> numCopied;

   /* int rowSpan;
    int columnSpan;

    dataStream >> rowSpan >> columnSpan;
*/
    int row;
    int column;

    for(int i = 0; i < numCopied; i++) {
        dataStream >> row >> column;

        deleteIfNeeded(row, column);
    }
}

// events ---------------------------------

void SelectableGroupWidget::keyPressEvent(QKeyEvent *event) {
    Qt::Key key = (Qt::Key)event->key();

    if((QApplication::keyboardModifiers() & Qt::ShiftModifier) == Qt::ShiftModifier) {
        if(selectedCount() > 0 && key != Qt::Key_Shift) {
            if(validKeyPress(key)) {
                selectDirection(key);
                return;
            }
        }
    }

    /*if(isGroupSelected() || isSingleSelected()) {
        if(((key == Qt::Key_V) || (key == Qt::Key_C)) || (key == Qt::Key_X)) &&
            (QApplication::keyboardModifiers() & Qt::ControlModifier) == Qt::ControlModifier) {
            QCoreApplication::sendEvent(&widgetAt(iFirstSelectedRow, iFirstSelectedColumn), event);
        }
    }*/
}
