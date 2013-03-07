/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

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
    iNumColumns(numColumns)//,
    /*iFirstSelectedRow(INVALID),
    iLastSelectedRow(INVALID),
    iFirstSelectedColumn(INVALID),
    iLastSelectedColumn(INVALID),
    iTopLeftSelectedRow(INVALID),
    iTopLeftSelectedColumn(INVALID),
    iBottomRightSelectedRow(INVALID),
    iBottomRightSelectedColumn(INVALID)*/ {

    iColourDialog = new QColorDialog(Qt::white, this);
    iColourDialog->setOptions(QColorDialog::DontUseNativeDialog);
}

bool SelectableGroupWidget::isMultipleSelected() {
    return  (iGroupGroup.isOtherSelected(iGroupNumber) &&
             iSelected.count() > 0) ||
            (iSelected.count() > 1 &&
            iAreas.count() == 0);
}

bool SelectableGroupWidget::isAreaSelected() {
    return  iAreas.count() > 0;
}

bool SelectableGroupWidget::isSingleSelected() {
    return iSelected.count() == 1;
}

// selecting ----------------------------

void SelectableGroupWidget::toggle(SelectableWidget &widget) {
   // qDebug("select %d", !widget.isSelected());
    bool select = !widget.isSelected();

    doSelect(widget, select);

    if(select) {
        setLastSelected(widget);
    } else if(isSingleSelected()) {
        clearAreaSelection();
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
        clearAreaSelection();
    }
}

void SelectableGroupWidget::selectOne(SelectableWidget &widget, bool singleSelect) {
  //  qDebug("selectOne");
    clearSelection();

    doSelect(widget, true, singleSelect);

    setLastSelected(widget);
}

void SelectableGroupWidget::selectArea(SelectableWidget& widget, bool multipleAreas) {
 //   qDebug("selectArea");

    Position position = widgetPosition(widget);

    if(!isAnySelected()) {
        iGroupGroup.selectArea(iGroupNumber, position, multipleAreas);
        return;
    }

    doSelectArea(iLastSelected, position, multipleAreas);
}

void SelectableGroupWidget::doSelectArea(Position start, Position end, bool multipleAreas) {
    if(!multipleAreas) {
        clearAreaSelection();
        clearSelection();
    }

    iAreas.append(Area(start, end));

    doGroupSelection();
}

void SelectableGroupWidget::selectDirection(Qt::Key direction, bool singleSelect) {
    if(!isAnySelected()) {
        return;
    }

    if(singleSelect) {
        iGroupGroup.selectSingleGroup(*this);
    }

    int lastRow = iAreas.last().lastSelected().row();
    int lastColumn = iAreas.last().lastSelected().column();

    switch(direction) {
        case Qt::Key_Up:
            if(lastRow - 1 >=0 ) {
                lastRow--;
            }
            break;
        case Qt::Key_Down:
            if(lastRow + 1 < iNumRows ) {
                lastRow++;
            }
            break;
        case Qt::Key_Left:
            if(lastColumn - 1 >=0) {
                lastColumn--;
            }
            break;
        case Qt::Key_Right:
            if(lastColumn + 1 < iNumColumns) {
                lastColumn++;
            }
            break;
        default:
            break;
    }

    iAreas.last().setLastSelected(Position(lastRow, lastColumn));

    doGroupSelection();
}

void SelectableGroupWidget::doSelectDirection(Qt::Key direction){

}

void SelectableGroupWidget::clearAllSelection() {
    clearSelection();
    clearAreaSelection();
}

void SelectableGroupWidget::clearSelection() {
    SelectableWidget* widget;
    foreach(widget, iSelected) {
        widget->select(false);
    }

    iSelected.clear();
    iLastSelected = Position();
}

void SelectableGroupWidget::clearAreaSelection() {
    iAreas.clear();
}

Position SelectableGroupWidget::lastSelected() {
    return iLastSelected;
}

void SelectableGroupWidget::setLastSelected(SelectableWidget &widget) {
    iLastSelected = widgetPosition(widget);
}

void SelectableGroupWidget::doSelect(SelectableWidget &widget, bool selected, bool singleSelect) {
    Position position = widgetPosition(widget);

    if(selected) {
        QString widgetType = "";
        if(iSelected.count() != 0) {
            widgetType = iSelected.at(0)->objectName();
        }

        if(iSelected.count() == 0 ||
           widget.objectName() == widgetType) {

            if(!iLastSelected.isValid() || position < iLastSelected) {
                iLastSelected = position;
                qDebug("ADD TO HEAD: lastRow is %d, lastColumn is %d", iLastSelected.row(), iLastSelected.column());

                iSelected.insert(0, &widget);
            } else {
                qDebug("ADD TO TAIL: lastRow is %d, lastColumn is %d", iLastSelected.row(), iLastSelected.column());
                iSelected.append(&widget);
            }

            iGroupGroup.selectGroup(iGroupNumber, true, singleSelect);

        } else {
            selected = false;
        }
    } else {
        iSelected.removeOne(&widget);
        qDebug("REMOVE : lastRow is %d, lastColumn is %d", iLastSelected.row(), iLastSelected.column());

        if(iSelected.count() == 0) {
            iLastSelected = Position();
            qDebug("REMOVE LAST");

            iGroupGroup.selectGroup(iGroupNumber, false);
        } else if(position == iLastSelected) {
            sortSelected();
        }
    }

    widget.select(selected);
}

void SelectableGroupWidget::sortSelected() {
    iLastSelected = Position(numRows() - 1, numColumns() - 1);

    SelectableWidget* widget;

    for(int i = 0; i < iSelected.count(); i++) {
        widget = iSelected.at(i);
        Position position = widgetPosition(*widget);

        if(position < iLastSelected) {
            iLastSelected = position;
            qDebug("SORT : lastRow is %d, lastColumn is %d", iLastSelected.row(), iLastSelected.column());

            iSelected.removeOne(widget);
            iSelected.insert(0, widget);

            i--;
        }
    }
}

void SelectableGroupWidget::doGroupSelection() {
    Position firstPosition = iAreas.last().firstSelected();
    Position lastPosition = iAreas.last().lastSelected();

    QString widgetType = widgetAt(firstPosition.row(), firstPosition.column()).objectName();

    int topLeftRow;
    int topLeftColumn;
    int bottomRightRow;
    int bottomRightColumn;

    if(firstPosition.row() < lastPosition.row()) {
        topLeftRow = firstPosition.row();
        bottomRightRow = lastPosition.row();
    } else {
        topLeftRow = lastPosition.row();
        bottomRightRow = firstPosition.row();
    }

    if(firstPosition.column() < lastPosition.column()) {
        topLeftColumn = firstPosition.column();
        bottomRightColumn = lastPosition.column();
    } else {
        topLeftColumn = lastPosition.column();
        bottomRightColumn = firstPosition.column();
    }

    for(int i = topLeftRow; i < bottomRightRow + 1; i++) {
        for(int j = topLeftColumn; j < bottomRightColumn + 1; j++) {
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

Position SelectableGroupWidget::widgetPosition(SelectableWidget &widget) {
     Q_UNUSED(widget);
#ifndef NDEBUG
 /*   if(*row < 0) {
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
    }*/
#endif

    return Position();
}

// mime data handlers ------------------------

const QByteArray SelectableGroupWidget::writeMimeData(bool cut) {
    return iGroupGroup.writeMimeData(cut);
}

bool SelectableGroupWidget::handleMimeData(QByteArray mimeData, SelectableWidget& dropWidget, bool wrap, bool move) {
    Position position = widgetPosition(dropWidget);

    if(dropWidget.isSelected()) {
        toggle(dropWidget);  // unselect the drop widget
    }

    qDebug("drop group number is %d", iGroupNumber);

    return iGroupGroup.handleMimeData(mimeData, iGroupNumber, position, wrap, move);
}

void SelectableGroupWidget::doWriteMimeData(QDataStream& dataStream, bool cut) {
    dataStream << iGroupNumber;
    dataStream << cut;
    dataStream << iSelected.count();

    SelectableWidget* widget;
    foreach(widget, iSelected) {
        Position position = widgetPosition(*widget);

        qDebug("WriteMimeData : Adding item at %d,%d", position.row(), position.column());
        dataStream << position.row() << position.column();

        if(cut) {
            moveToClipboard(iGroupNumber, position);
        }
    }
}

bool SelectableGroupWidget::doHandleMimeData(QDataStream& dataStream,
                                             int fromGroupNumber,
                                             Position dropPosition,
                                             int* originRow,
                                             int* originColumn,
                                             bool wrap,
                                             bool move) {
    bool cut;

    dataStream >> cut;

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

        newRow = dropPosition.row() + (oldRow - *originRow);
        if(numRows() != 0 && newRow >= numRows()) {
            if(wrap) {
                newRow = newRow - numRows();
            } else {
                newRow = INVALID;
            }

        }

        newColumn = dropPosition.column() + (oldColumn - *originColumn);
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
                moveItem(fromGroupNumber, oldRow, oldColumn, newRow, newColumn);
            } else if(cut) {
                pasteItem(fromGroupNumber, oldRow, oldColumn, newRow, newColumn);
            } else {
                cloneItem(fromGroupNumber, oldRow, oldColumn, newRow, newColumn);
            }
        }
    }

   /* if(groupNumber != iGroupNumber) {
        SelectableGroupWidget& oldGroup = iGroupGroup.group(groupNumber);
        oldGroup.clearSelection();
        oldGroup.clearAreaSelection();
    }*/

    clearSelection();
    clearAreaSelection();
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
