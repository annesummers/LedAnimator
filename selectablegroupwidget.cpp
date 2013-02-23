#include "selectablegroupwidget.h"

#include <QtGui>

#include "selectablewidget.h"
#include "colourgroupgroupwidget.h"

#include "exceptions.h"
#include "constants.h"

using namespace AnimatorUi;
using namespace Exception;

SelectableGroupWidget::SelectableGroupWidget(QWidget *parent, int numRows, int numColumns, ColourGroupGroupWidget *groupGroupWidget) :
    QWidget(parent),
    iNumRows(numRows),
    iNumColumns(numColumns),
    iFirstSelectedRow(0),
    iLastSelectedRow(0),
    iFirstSelectedColumn(0),
    iLastSelectedColumn(0),
    iGroupGroup(groupGroupWidget) {

  if(iGroupGroup != NULL) {
      iGroupGroup->addGroup(*this);
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
   // qDebug("select %d", !widget.isSelected());
    doSelect(widget, !widget.isSelected());

    if(selectedCount() == 1) {
        setSingleSelected(widget);
    } else {
        clearGroupSelection();
    }
}

void SelectableGroupWidget::select(SelectableWidget &widget) {
    doSelect(widget, true);

    if(selectedCount() == 1) {
        setSingleSelected(widget);
    } else {
        clearGroupSelection();
    }
}

void SelectableGroupWidget::toggleOne(SelectableWidget &widget) {
//    qDebug("toggleOne");
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
   // qDebug("selectOne");
    clearSelection();

    doSelect(widget, true);

    setSingleSelected(widget);
}

void SelectableGroupWidget::selectArea(SelectableWidget& widget) {
    //qDebug("selectArea");
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

    iFirstSelectedRow = iLastSelectedRow = row;
    iFirstSelectedColumn = iLastSelectedColumn = column;
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
}

void SelectableGroupWidget::doSelect(SelectableWidget &widget, bool selected) {
    if(selected) {
        QString widgetType = "";
        if(iSelected.count() != 0) {
            widgetType = iSelected.at(0)->objectName();
            if(widget.objectName() == widgetType){
                iSelected.append(&widget);

                if(iGroupGroup != NULL) {
                    iGroupGroup->selected(*this);
                }
            } else {
                selected = false;
            }
        } else {
            iSelected.append(&widget);

            if(iGroupGroup != NULL) {
                iGroupGroup->selected(*this);
            }
        }
    } else {
        iSelected.removeOne(&widget);
    }

    widget.select(selected);
}

void SelectableGroupWidget::doGroupSelection() {
    clearSelection();

    QString widgetType = widgetAt(iFirstSelectedRow, iFirstSelectedColumn).objectName();

    if(iFirstSelectedRow < iLastSelectedRow) {
        if(iFirstSelectedColumn < iLastSelectedColumn) {
            for(int i = iFirstSelectedRow; i < iLastSelectedRow + 1; i++) {
                for(int j = iFirstSelectedColumn; j < iLastSelectedColumn + 1; j++) {
                    SelectableWidget& widget = widgetAt(i, j);
                    if(widget.objectName() == widgetType) {
                        doSelect(widget, true);
                    }
                }
            }
        } else {
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
        if(iFirstSelectedColumn < iLastSelectedColumn) {
            for(int i = iFirstSelectedRow; i > iLastSelectedRow - 1; i--) {
                for(int j = iFirstSelectedColumn; j < iLastSelectedColumn + 1; j++) {
                    SelectableWidget& widget = widgetAt(i, j);
                    if(widget.objectName() == widgetType) {
                        doSelect(widget, true);
                    }
                }
            }
        } else {
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
    *topRow = iFirstSelectedRow;
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
    }
}

// events ---------------------------------

void SelectableGroupWidget::keyPressEvent(QKeyEvent *event) {
    Qt::Key key = (Qt::Key)event->key();

  //  Qt::KeyboardModifiers mods = QApplication::keyboardModifiers();
  //  if(mods != 0) {
  //      qDebug("mod");
  //  }

    if((QApplication::keyboardModifiers() & Qt::ShiftModifier) == Qt::ShiftModifier) {
        if(selectedCount() > 0 && key != Qt::Key_Shift) {
            if(validKeyPress(key)) {
                selectDirection(key);
                return;
            }
        }
    }

    if(isGroupSelected() || isSingleSelected()) {
        if(((key == Qt::Key_V) || (key == Qt::Key_C) || (key == Qt::Key_X)) &&
            (QApplication::keyboardModifiers() & Qt::ControlModifier) == Qt::ControlModifier) {
            QCoreApplication::sendEvent(&widgetAt(iFirstSelectedRow, iFirstSelectedColumn), event);
        }
    }
}

// mime data handlers ------------------------

const QByteArray SelectableGroupWidget::writeMimeData() {
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    int topRow;
    int bottomRow;
    int leftColumn;
    int rightColumn;

     if(isMultipleSelected()) {
      /*   dataStream << iSelected.count() << 0 << 0;

         SelectableWidget* widget;
         foreach(widget, iSelected) {
             dataStream << static_cast<ColourWidget&widget->colour();
             widget->addExtraData(dataStream);
         }*/
     } else {
         getLeftRightTopBottomSelection(&topRow, &bottomRow, &leftColumn, &rightColumn);

         int rowSpan = bottomRow + 1 - topRow;
         int columnSpan = rightColumn + 1 - leftColumn;

         dataStream << rowSpan * columnSpan;
         dataStream << rowSpan
                    << columnSpan;

         for(int i = topRow; i < bottomRow + 1; i++) {
             for(int j = leftColumn; j < rightColumn + 1; j++) {
                 //widgetAt(i, j).writeMimeData(dataStream);
                 dataStream << i << j;
             }
         }
    }

    return itemData;
}

void SelectableGroupWidget::handleMimeData(QByteArray itemData, SelectableWidget& dropWidget, bool move, bool wrap) {
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);

    selectOne(dropWidget);

    int row;
    int column;

    getWidgetPosition(dropWidget, &row, &column);

    int numWidgets;

    dataStream >> numWidgets;

    int rowSpan;
    int columnSpan;

    dataStream >> rowSpan >> columnSpan;
/*
    QList<int> oldRows;
    QList<int> oldColumns;

    int oldRow;
    int oldColumn;

    int newRow;
    int newColumn;

    for(int i = 0; i < numWidgets; i++) {
        dataStream >> oldRow;
        dataStream >> oldColumn;

        oldRows.append(oldRow);
        oldColumns.append(oldColumn);
    }



    foreach

        newRow = row + oldRow;
        if(numRows() != 0 && newRow >= numRows()) {
            if(wrap) {
                newRow = newRow - numRows();
            } else {
                newRow = INVALID;
            }
        }

        newColumn = column + oldColumn;
        if(numRows() != 0 && newColumn >= numColumns()) {
            if(wrap) {
                newColumn = newColumn - numColumns();
            } else {
                newColumn = INVALID;
            }
        }

        if(newRow != INVALID && newColumn != INVALID) {
            if(move) {
                moveItem(oldRow, oldColumn, newRow, newColumn);
            } else {
                copyItem(oldRow, oldColumn, newRow, newColumn);
            }
        }
    }
*/


    int oldRow;
    int oldColumn;

    if(rowSpan != 0 && columnSpan != 0) {
        int rowEnd = row + rowSpan;
        int columnEnd = column + columnSpan;

        bool secondLoop = false;
        int newRowEnd = 1;
        int newColumnEnd = 1;

        if(wrap) {
            if(numRows() != 0 && rowEnd >= numRows()) {
                int oldRowEnd = rowEnd;
                rowEnd = numRows();
                secondLoop = true;

                newRowEnd = oldRowEnd - rowEnd;
            }

            if(numColumns() != 0 && columnEnd >= numColumns()) {
                int oldColumnEnd = columnEnd;
                columnEnd = numColumns();
                secondLoop = true;

                newColumnEnd = oldColumnEnd - columnEnd;
            }
        } else {
            if(numRows() != 0 && rowEnd >= numRows()) {
                rowEnd = numRows();
            }

            if(numColumns() != 0 && columnEnd >= numColumns()) {
                columnEnd = numColumns();
            }
        }

        for(int i = row; i < rowEnd; i++) {
            for(int j = column; j < columnEnd; j++) {
              //  SelectableWidget& widget = widgetAt(i, j);

              //  widget.handleMimeData(dataStream, move);
                dataStream >> oldRow >> oldColumn;

                if(move) {
                    moveItem(oldRow, oldColumn, i, j);
                } else {
                    copyItem(oldRow, oldColumn, i, j);
                }

                if(i == rowEnd - 1 && j == columnEnd - 1) {
                    SelectableWidget& newWidget = widgetAt(i, j);
                    selectArea(newWidget);
                }
            }
        }

        if(secondLoop) {
            for(int i = 0; i < newRowEnd; i++) {
                for(int j = 0; j < newColumnEnd; j++) {
                    //  widget.handleMimeData(dataStream, move);
                      dataStream >> oldRow >> oldColumn;

                      if(move) {
                          moveItem(oldRow, oldColumn, i, j);
                      } else {
                          copyItem(oldRow, oldColumn, i, j);
                      }

                    if(i == newRowEnd - 1 && j == newColumnEnd - 1) {
                        SelectableWidget& newWidget = widgetAt(i, j);
                        selectArea(newWidget);
                    }
                }
            }
        }
    } else {
       /* QColor colour;
        int row;
        int column;

        for(int i = 0; i < numCopied; i++) {
            dataStream >> colour;
            dataStream >> row >> column;

           // widgetAt(row, column).hand
        }*/
    }
}

void SelectableGroupWidget::handleOldMimeData(QByteArray itemData) {
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);

    int numCopied;

    dataStream >> numCopied;

    int rowSpan;
    int columnSpan;

    dataStream >> rowSpan >> columnSpan;

    int row;
    int column;

    for(int i = 0; i < numCopied; i++) {
        dataStream >> row >> column;

        deleteIfNeeded(row, column);
    }
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
