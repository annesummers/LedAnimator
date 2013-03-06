#include "selectablegroupwidget.h"

#include <QtGui>

#include "selectablewidget.h"
#include "selectablegroupgroupwidget.h"

#include "exceptions.h"
#include "constants.h"

using namespace AnimatorUi;
using namespace Exception;

SelectableGroupWidget::SelectableGroupWidget(QWidget *parent, int numRows, int numColumns, SelectableGroupGroupWidget &groupGroupWidget, int groupNumber) :
    QWidget(parent),
    iGroupGroup(groupGroupWidget),
    iGroupNumber(iGroupGroup.addGroup(*this)),
    iColourDialog(NULL),
    iNumRows(numRows),
    iNumColumns(numColumns),
    iFirstSelectedRow(INVALID),
    iLastSelectedRow(INVALID),
    iFirstSelectedColumn(INVALID),
    iLastSelectedColumn(INVALID),
    iTopLeftSelectedRow(INVALID),
    iTopLeftSelectedColumn(INVALID),
    iBottomRightSelectedRow(INVALID),
    iBottomRightSelectedColumn(INVALID) {

    iColourDialog = new QColorDialog(Qt::white, this);
    iColourDialog->setOptions(QColorDialog::DontUseNativeDialog);
}

bool SelectableGroupWidget::isMultipleSelected() {
    return  iSelected.count() > 1 &&
            iFirstSelectedRow == iLastSelectedRow &&
            iFirstSelectedColumn == iLastSelectedColumn;
}

bool SelectableGroupWidget::isGroupSelected() {
    return  iSelected.count() > 1 &&
            !isMultipleSelected();
}

bool SelectableGroupWidget::isSingleSelected() {
    return iSelected.count() == 1 &&
            iFirstSelectedRow == iLastSelectedRow &&
            iFirstSelectedColumn == iLastSelectedColumn;
}

// selecting ----------------------------

void SelectableGroupWidget::toggle(SelectableWidget &widget) {
   // qDebug("select %d", !widget.isSelected());
    bool select = !widget.isSelected();

    doSelect(widget, select);

    if(select) {
        setLastSelected(widget);
    } else if(iSelected.count() == 0) {
        setNoneSelected();
    }
}

void SelectableGroupWidget::toggleOne(SelectableWidget &widget, bool singleSelect) {
  //  qDebug("toggleOne");
    bool selected = false;

    if(iSelected.count() > 1 ||
       (iSelected.count() == 1 &&
        iSelected.at(0) != &widget) ||
       iSelected.isEmpty()) {
        selected = true;
    }

    if(selected){
        selectOne(widget, singleSelect);
    } else {
        clearSelection();
        setNoneSelected();
    }
}

void SelectableGroupWidget::selectOne(SelectableWidget &widget, bool singleSelect) {
  //  qDebug("selectOne");
    clearSelection();

    doSelect(widget, true, singleSelect);

    setLastSelected(widget);
}

void SelectableGroupWidget::selectArea(SelectableWidget& widget, bool singleSelect) {
 //   qDebug("selectArea");

    int row;
    int column;
    getWidgetPosition(widget, &row, &column);

    if(!isAnySelected()) {
        iGroupGroup.selectArea(iGroupNumber, row, column);
        return;
    }

    //if(singleSelect) {
    //    iGroupGroup.selectSingle(*this);
   // }

    doSelectArea(iFirstSelectedRow, iFirstSelectedColumn, row, column);
}

void SelectableGroupWidget::doSelectArea(int startRow, int startColumn, int endRow, int endColumn) {
    iFirstSelectedRow = startRow;
    iFirstSelectedColumn = startColumn;
    iLastSelectedRow = endRow;
    iLastSelectedColumn = endColumn;

    doGroupSelection();
}

void SelectableGroupWidget::selectDirection(Qt::Key direction, bool singleSelect) {
    if(!isAnySelected()) {
        return;
    }

    if(singleSelect) {
        iGroupGroup.selectSingleGroup(*this);
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

void SelectableGroupWidget::clearSelection() {
    SelectableWidget* widget;
    foreach(widget, iSelected) {
        widget->select(false);
    }

    iSelected.clear();
    iTopLeftSelectedRow = iBottomRightSelectedRow = INVALID;
    iTopLeftSelectedColumn = iBottomRightSelectedColumn = INVALID;
}

void SelectableGroupWidget::setNoneSelected() {
    iFirstSelectedRow = iLastSelectedRow = INVALID;
    iFirstSelectedColumn = iLastSelectedColumn = INVALID;
}

void SelectableGroupWidget::getLastSelected(int* lastRow, int* lastColumn) {
    *lastRow = iFirstSelectedRow;
    *lastColumn = iFirstSelectedColumn;
}

void SelectableGroupWidget::setLastSelected(SelectableWidget &widget) {
    int row;
    int column;

    getWidgetPosition(widget, &row, &column);

    iFirstSelectedRow = iLastSelectedRow = row;
    iFirstSelectedColumn = iLastSelectedColumn = column;
}

void SelectableGroupWidget::doSelect(SelectableWidget &widget, bool selected, bool singleSelect) {
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
          //      qDebug("ADD TO HEAD: topLeftRow is %d, topLeftColumn is %d", iTopLeftSelectedRow, iTopLeftSelectedColumn);

                iSelected.insert(0, &widget);
            } else {
            //    qDebug("ADD TO TAIL: topLeftRow is %d, topLeftColumn is %d", iTopLeftSelectedRow, iTopLeftSelectedColumn);
                iSelected.append(&widget);
            }

            iGroupGroup.selectGroup(iGroupNumber, true, singleSelect);

        } else {
            selected = false;
        }
    } else {
        iSelected.removeOne(&widget);
        //qDebug("REMOVE : topLeftRow is %d, topLeftColumn is %d", iTopLeftSelectedRow, iTopLeftSelectedColumn);

        if(iSelected.count() == 0) {
            iTopLeftSelectedRow = iTopLeftSelectedColumn = INVALID;
          //  qDebug("REMOVE LAST");

            iGroupGroup.selectGroup(iGroupNumber, false);
        } else if(row == iTopLeftSelectedRow &&
                  column == iTopLeftSelectedColumn) {
            sortSelected();
        }
    }

    widget.select(selected);
}

void SelectableGroupWidget::sortSelected() {
    iTopLeftSelectedRow = numRows() - 1;
    iTopLeftSelectedColumn = numColumns() - 1;

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
            //qDebug("SORT : topLeftRow is %d, topLeftColumn is %d", iTopLeftSelectedRow, iTopLeftSelectedColumn);

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
    } else {
        iTopLeftSelectedRow = iLastSelectedRow;
        iBottomRightSelectedRow = iFirstSelectedRow;
    }

    if(iFirstSelectedColumn < iLastSelectedColumn) {
        iTopLeftSelectedColumn = iFirstSelectedColumn;
        iBottomRightSelectedColumn = iLastSelectedColumn;
    } else {
        iTopLeftSelectedColumn = iLastSelectedColumn;
        iBottomRightSelectedColumn = iFirstSelectedColumn;
    }

    for(int i = iTopLeftSelectedRow; i < iBottomRightSelectedRow + 1; i++) {
        for(int j = iTopLeftSelectedColumn; j < iBottomRightSelectedColumn + 1; j++) {
            SelectableWidget& widget = widgetAt(i, j);
            if(widget.objectName() == widgetType) {
                doSelect(widget, true);
            }
        }
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

// mime data handlers ------------------------

const QByteArray SelectableGroupWidget::writeMimeData(bool cut) {
    return iGroupGroup.writeMimeData(cut);
}

bool SelectableGroupWidget::handleMimeData(QByteArray mimeData, SelectableWidget& dropWidget, bool wrap, bool move) {
    int row;
    int column;

    getWidgetPosition(dropWidget, &row, &column);
    if(dropWidget.isSelected()) {
        toggle(dropWidget);  // unselect the drop widget
    }

    qDebug("drop group number is %d", iGroupNumber);

    return iGroupGroup.handleMimeData(mimeData, iGroupNumber, row, column, wrap, move);
}

void SelectableGroupWidget::doWriteMimeData(QDataStream& dataStream, bool cut) {
    dataStream << cut;
    qDebug("WriteMimeData : Cut is %d", cut);
    dataStream << iGroupNumber;
    qDebug("WriteMimeData : Group number is %d", iGroupNumber);
    dataStream << iSelected.count();
    qDebug("WriteMimeData : Selected number is %d", iSelected.count());

    int row;
    int column;

    SelectableWidget* widget;
    foreach(widget, iSelected) {
        getWidgetPosition(*widget, &row, &column);

        qDebug("WriteMimeData : Adding item at %d,%d", row, column);
        dataStream << row << column;

        if(cut) {
            moveToClipboard(iGroupNumber, row, column);
        }
    }
}

bool SelectableGroupWidget::doHandleMimeData(QDataStream& dataStream, int dropRow, int dropColumn, int* originRow, int* originColumn, bool wrap, bool move) {
    bool cut;

    dataStream >> cut;

    int groupNumber;

    dataStream >> groupNumber;

    int numWidgets;

    dataStream >> numWidgets;

    QList<int> newRows;
    QList<int> newColumns;

    int oldRow;
    int oldColumn;

    int newRow;
    int newColumn;

    for(int i = 0; i < numWidgets; i++) {
        dataStream >> oldRow;
        dataStream >> oldColumn;

        if(*originRow == INVALID && *originColumn == INVALID) {
            *originRow = oldRow;
            *originColumn = oldColumn;
        }

        newRow = dropRow + (oldRow - *originRow);
        if(numRows() != 0 && newRow >= numRows()) {
            if(wrap) {
                newRow = newRow - numRows();
            } else {
                newRow = INVALID;
            }

        }

        newColumn = dropColumn + (oldColumn - *originColumn);
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
            } else if(cut) {
                pasteItem(groupNumber, oldRow, oldColumn, newRow, newColumn);
            } else {
                cloneItem(groupNumber, oldRow, oldColumn, newRow, newColumn);
            }
        }
    }

    if(groupNumber != iGroupNumber) {
        SelectableGroupWidget& oldGroup = iGroupGroup.group(groupNumber);
        oldGroup.clearSelection();
        oldGroup.setNoneSelected();
    }

    clearSelection();
    setNoneSelected();
    for(int i = 0; i < newRows.count(); i++) {
        toggle(widgetAt(newRows.at(i), newColumns.at(i)));
    }

    return cut;
}

// events ---------------------------------

void SelectableGroupWidget::keyPressEvent(QKeyEvent *event) {
    Qt::Key key = (Qt::Key)event->key();

    if((QApplication::keyboardModifiers() & Qt::ShiftModifier) == Qt::ShiftModifier) {
        if(selectedCount() > 0 && key != Qt::Key_Shift) {
            if(validKeyPress(key)) {
                selectDirection(key, (QApplication::keyboardModifiers() & Qt::ControlModifier) == Qt::ControlModifier);
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
