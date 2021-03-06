/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "selectablegroupwidget.h"

#include <QApplication>
#include <QtGui>

#include "selectablewidget.h"
#include "selectablegroupgroupwidget.h"

#include "exceptions.h"
#include "constants.h"

using namespace AnimatorUi;
using namespace Exception;

SelectableGroupWidget::SelectableGroupWidget(QWidget *parent,
                                             int numRows,
                                             int numColumns,
                                             SelectableGroupGroupWidget &groupGroupWidget//,
                                             /*int groupNumber*/) :
    QWidget(parent),
    iGroupGroup(groupGroupWidget),
    iGroupNumber(iGroupGroup.addGroup(*this)),
    iColourDialog(NULL),
    iNumRows(numRows),
    iNumColumns(numColumns) {

    iColourDialog = new QColorDialog(Qt::white, this);
    iColourDialog->setOptions(QColorDialog::DontUseNativeDialog);
}

bool SelectableGroupWidget::isMultipleSelected() const {
    return  (iGroupGroup.isOtherSelected(iGroupNumber) &&
             iSelected.count() > 0) ||
            (iSelected.count() > 1 &&
            iAreas.count() == 0);
}

bool SelectableGroupWidget::isAreaSelected() const {
    return  iAreas.count() > 0;
}

bool SelectableGroupWidget::isSingleSelected() const {
    return iSelected.count() == 1;
}

// selecting ----------------------------

void SelectableGroupWidget::select(SelectableWidget &widget, bool select) {
    qDebug() << "select " << select;

    doSelect(widget, select, false);

    clearAreaSelection();
}

void SelectableGroupWidget::selectOne(SelectableWidget &widget, bool singleSelect) {
    qDebug("selectOne");
    clearSelection();

    doSelect(widget, true, singleSelect);

    clearAreaSelection();
}

void SelectableGroupWidget::selectArea(SelectableWidget& widget, bool multipleAreas) {
 //   qDebug("selectArea");

    Position position = widgetPosition(widget);

    if(!multipleAreas) {
        clearAllSelection();
    }

    if((multipleAreas && firstSelectedPosition().isValid()) ||
       !iGroupGroup.isOtherSelected(iGroupNumber)) {
        doSelectArea(firstSelectedPosition(), position);
        return;
    }

    iGroupGroup.selectArea(iGroupNumber, position, multipleAreas);
}

void SelectableGroupWidget::doSelectArea(Position start, Position end) {
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

    if(isSingleSelected()) {
        iAreas.append(Area(lastSelectedPosition(), lastSelectedPosition()));
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

    clearSelection();
    doGroupSelection();
}

void SelectableGroupWidget::doSelectDirection(Qt::Key direction){
    Q_UNUSED(direction);
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
    setLastSelected(NULL);
}

void SelectableGroupWidget::clearAreaSelection() {
    iAreas.clear();
}

Position SelectableGroupWidget::lastSelectedPosition() const {
    return iLastSelected;
}

void SelectableGroupWidget::setLastSelected(SelectableWidget *widget) {
    if(widget == NULL) {
        //qDebug("setLastSelected NONE");
        iLastSelected = Position();
    } else {
        iLastSelected = widgetPosition(*widget);
        //qDebug("setLastSelected %d,%d", iLastSelected.row(), iLastSelected.column());
    }
}

Position SelectableGroupWidget::firstSelectedPosition() const {
    return iFirstSelected;
}

void SelectableGroupWidget::setFirstSelected(SelectableWidget *widget) {
    if(widget == NULL) {
        //qDebug("setLastSelected NONE");
        iFirstSelected = Position();
    } else {
        iFirstSelected = widgetPosition(*widget);
        //qDebug("setLastSelected %d,%d", iLastSelected.row(), iLastSelected.column());
    }
}

void SelectableGroupWidget::doSelect(SelectableWidget &widget, bool selected, bool singleSelect) {
    Position position = widgetPosition(widget);

    if(selected) {
        QString widgetType = "";
        if(iSelected.count() != 0) {
            widgetType = iSelected.at(0)->objectName();
        }

        if(iSelected.count() == 0 || widget.objectName() == widgetType) {
            if(iSelected.count() == 0) {
                setFirstSelected(&widget);
            }

            setLastSelected(&widget);

            if(!lastSelectedPosition().isValid() || position < lastSelectedPosition()) {
                //qDebug("ADD TO HEAD: lastRow is %d, lastColumn is %d", lastSelectedPosition().row(), lastSelectedPosition().column());

                iSelected.insert(0, &widget);
            } else {
                //qDebug("ADD TO TAIL: lastRow is %d, lastColumn is %d", lastSelectedPosition().row(), lastSelectedPosition().column());
                iSelected.append(&widget);
            }

            iGroupGroup.selectGroup(iGroupNumber, true, singleSelect);

        } else {
            selected = false;
        }
    } else {
        iSelected.removeOne(&widget);
        //qDebug("REMOVE : lastRow is %d, lastColumn is %d", lastSelectedPosition().row(), lastSelectedPosition().column());

        if(iSelected.count() == 0) {
            setLastSelected(NULL);
            setFirstSelected(NULL);
            //qDebug("REMOVE LAST");

            iGroupGroup.selectGroup(iGroupNumber, false);
        } else if(position == lastSelectedPosition()) {
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

        if(position < lastSelectedPosition()) {
            setLastSelected(widget);
            //qDebug("SORT : lastRow is %d, lastColumn is %d", lastSelectedPosition().row(), lastSelectedPosition().column());

            iSelected.removeOne(widget);
            iSelected.insert(0, widget);

            i--;
        }
    }

    setFirstSelected(iSelected.at(0));
}

void SelectableGroupWidget::doGroupSelection() {
    Position firstPosition = iAreas.last().firstSelected();
    Position lastPosition = iAreas.last().lastSelected();

    QString widgetType = widgetAt(firstPosition).objectName();

    int rowEnd = lastPosition.row();
    int columnEnd = lastPosition.column();

    bool rowIncrement = firstPosition.row() <= lastPosition.row();
    bool columnIncrement = firstPosition.column() <= lastPosition.column();

    for(int row = firstPosition.row();
        rowIncrement ? row <= rowEnd : row >= rowEnd;
        rowIncrement ? row++ : row--) {
        for(int column = firstPosition.column();
            columnIncrement ? column <= columnEnd : column >= columnEnd;
            columnIncrement ? column++ : column--) {

           SelectableWidget& widget = widgetAt(Position(row, column));
            if(widget.objectName() == widgetType) {
                doSelect(widget, true);
            }
        }
    }

    if(iAreas.last().lastSelected() != iLastSelected) {
        iAreas.last().setLastSelected(iLastSelected);
    }
}

SelectableWidget& SelectableGroupWidget::widgetAt(Position position) {
#ifndef NDEBUG
    if(position.row() < 0) {
        throw IllegalArgumentException("SelectableGroupWidget::widgetAt : Row is negative");
    }

    if(numRows() != 0 && position.row() >= numRows()) {
        throw IllegalArgumentException("SelectableGroupWidget::widgetAt : Row is greater than number of rows");
    }

    if(position.column() < 0) {
        throw IllegalArgumentException("SelectableGroupWidget::widgetAt : Column is negative");
    }

    if(numColumns() != 0 && position.column() >= numColumns()) {
        throw IllegalArgumentException("SelectableGroupWidget::widgetAt : Column is greater than number of columns");
    }
#endif
}

Position SelectableGroupWidget::widgetPosition(SelectableWidget &widget) {
     Q_UNUSED(widget);

    return Position();
}

// cut copy and paste

void SelectableGroupWidget::cutSelected() {
    iGroupGroup.cutSelected();
}

void SelectableGroupWidget::copySelected() {
    iGroupGroup.copySelected();
}

void SelectableGroupWidget::paste(bool wrap) {
    paste(widgetAt(lastSelectedPosition()), wrap);
}

void SelectableGroupWidget::paste(SelectableWidget& dropWidget, bool wrap) {
    const QClipboard *clipboard = QApplication::clipboard();

    if(clipboard->mimeData()->hasFormat(mimeType())) {
        bool wasCut = handleMimeData(clipboard->mimeData()->data(mimeType()), dropWidget, wrap);

        if(wasCut) {
            iGroupGroup.copySelected();
        }
    }
}

// mime data handlers ------------------------

bool SelectableGroupWidget::handleMimeData(QByteArray mimeData, SelectableWidget& dropWidget, bool wrap, bool move) {
    Position position = widgetPosition(dropWidget);

    if(dropWidget.isSelected()) {
        select(dropWidget, false);  // unselect the drop widget
    }

    //qDebug("drop group number is %d", iGroupNumber);

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

    QList<Position> newPositions;

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
            Position newPosition = Position(newRow, newColumn);
            Position oldPosition = Position(oldRow, oldColumn);

            newPositions.append(newPosition);

            if(move) {
                moveItem(fromGroupNumber, oldPosition, newPosition);
            } else if(cut) {
                pasteClipboardItem(fromGroupNumber, oldPosition, newPosition);
            } else {
                cloneItem(fromGroupNumber, oldPosition, newPosition);
            }
        }
    }

    clearAllSelection();
    for(int i = 0; i < newPositions.count(); i++) {
        select(widgetAt(newPositions.at(i)), true);
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

void SelectableGroupWidget::mousePressEvent(QMouseEvent* event) {
    //qDebug("singleWidget mousePress");
    if (event->button() != Qt::LeftButton) {
        return;
    }

    clearAllSelection();

}
