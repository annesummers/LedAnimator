/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "colourgroupwidget.h"

#include "animation.h"
#include "colourwidget.h"
#include "selectable.h"
#include "fadecalculator.h"
#include "colourgroupgroupwidget.h"

#include "defaults.h"
#include "exceptions.h"

using namespace Exception;
using namespace Ui;

ColourGroupWidget::ColourGroupWidget(QWidget *parent,
                                     int maxRow,
                                     int maxColumn,
                                     ColourGroupGroupWidget *groupGroup) :
    QWidget(parent),
    iFadeCalculator(NULL),
    iNumRows(maxRow),
    iNumColumns(maxColumn),
    iFirstSelectedRow(0),
    iLastSelectedRow(0),
    iFirstSelectedColumn(0),
    iLastSelectedColumn(0),
    iFadeParameters(NULL),
    iGroupGroup(groupGroup) {

    if(iGroupGroup != NULL) {
        iGroupGroup->addGroup(*this);
    }
}

ColourWidget& ColourGroupWidget::widgetAt(int row, int column) {
#ifndef NDEBUG
    if(row < 0) {
        throw IllegalArgumentException("ColourGroupWidget::widgetAt : Row is negative");
    }

    if(numRows() != 0 && row >= numRows()) {
        throw IllegalArgumentException("ColourGroupWidget::widgetAt : Row is greater than number of rows");
    }

    if(column < 0) {
        throw IllegalArgumentException("ColourGroupWidget::widgetAt : Column is negative");
    }

    if(numColumns() != 0 && column >= numColumns()) {
        throw IllegalArgumentException("ColourGroupWidget::widgetAt : Column is greater than number of columns");
    }
#endif
}

void ColourGroupWidget::getWidgetPosition(ColourWidget &widget, int *row, int *column) {
     Q_UNUSED(widget);
#ifndef NDEBUG
    if(*row < 0) {
        throw IllegalArgumentException("ColourGroupWidget::getWidgetPosition : Row is negative");
    }

    if(*row > iNumRows) {
        throw IllegalArgumentException("ColourGroupWidget::getWidgetPosition : Row is greater than number of rows");
    }

    if(*column < 0) {
        throw IllegalArgumentException("ColourGroupWidget::getWidgetPosition : Column is negative");
    }

    if(*column >= iNumColumns) {
        throw IllegalArgumentException("ColourGroupWidget::getWidgetPosition : Column is greater than number of columns");
    }
#endif
}

void ColourGroupWidget::setColour(QColor colour) {
   // qDebug("setColour, %d", iSelected.count());
    if(colour.isValid()) {
        ColourWidget* item = NULL;

        foreach(item, iSelected){
            item->setColour(colour);
        }
    }
}

bool ColourGroupWidget::isMultipleSelected() {
    return  iFirstSelectedRow == INVALID &&
            iFirstSelectedColumn == INVALID &&
            iLastSelectedRow == INVALID &&
            iLastSelectedColumn == INVALID;
}

bool ColourGroupWidget::isGroupSelected() {
    return  !isSingleSelected() &&
            !isMultipleSelected();
}

bool ColourGroupWidget::isSingleSelected() {
    return !isMultipleSelected() &&
            iFirstSelectedRow == iLastSelectedRow &&
            iFirstSelectedColumn == iLastSelectedColumn;
}

// selecting ----------------------------

void ColourGroupWidget::toggle(ColourWidget &widget) {
    //qDebug("select %d", selected);
    doSelect(widget, !widget.isSelected());

    if(selectedCount() == 1) {
        setSingleSelected(widget);
    } else {
        clearGroupSelection();
    }
}

void ColourGroupWidget::toggleOne(ColourWidget &widget) {
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

void ColourGroupWidget::selectOne(ColourWidget &widget) {
    //qDebug("selectOne");
    clearSelection();

    doSelect(widget, true);

    setSingleSelected(widget);
}

void ColourGroupWidget::selectArea(ColourWidget& widget) {
    //qDebug("selectArea");
    if(!isSingleSelected()) {
        return;
    }

    getWidgetPosition(widget, &iLastSelectedRow, &iLastSelectedColumn);

    doGroupSelection();
}

void ColourGroupWidget::selectDirection(Qt::Key direction) {
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

void ColourGroupWidget::setSingleSelected(ColourWidget& widget) {
    int row;
    int column;

    getWidgetPosition(widget, &row, &column);

    iFirstSelectedRow = iLastSelectedRow = row;
    iFirstSelectedColumn = iLastSelectedColumn = column;
}

void ColourGroupWidget::clearGroupSelection() {
    iFirstSelectedRow = iLastSelectedRow = iFirstSelectedColumn = iLastSelectedColumn = INVALID;
}

void ColourGroupWidget::clearSelection() {
    ColourWidget* widget;
    foreach(widget, iSelected) {
        widget->select(false);
    }

    iSelected.clear();
}

void ColourGroupWidget::doSelect(ColourWidget &widget, bool selected) {
    if(selected) {
        iSelected.append(&widget);

        if(iGroupGroup != NULL) {
            iGroupGroup->selected(*this);
        }
    } else {
        iSelected.removeOne(&widget);
    }

    widget.select(selected);
}

void ColourGroupWidget::doGroupSelection() {
    clearSelection();

    if(iFirstSelectedRow < iLastSelectedRow) {
        if(iFirstSelectedColumn < iLastSelectedColumn) {
            for(int i = iFirstSelectedRow; i < iLastSelectedRow + 1; i++) {
                for(int j = iFirstSelectedColumn; j < iLastSelectedColumn + 1; j++) {
                    doSelect(widgetAt(i, j), true);
                }
            }
        } else {
            for(int i = iFirstSelectedRow; i < iLastSelectedRow + 1; i++) {
                for(int j = iFirstSelectedColumn; j > iLastSelectedColumn - 1; j--) {
                    doSelect(widgetAt(i, j), true);
                }
            }
        }
    } else {
        if(iFirstSelectedColumn < iLastSelectedColumn) {
            for(int i = iFirstSelectedRow; i > iLastSelectedRow - 1; i--) {
                for(int j = iFirstSelectedColumn; j < iLastSelectedColumn + 1; j++) {
                    doSelect(widgetAt(i, j), true);
                }
            }
        } else {
            for(int i = iFirstSelectedRow; i > iLastSelectedRow - 1; i--) {
                for(int j = iFirstSelectedColumn; j > iLastSelectedColumn - 1; j--) {
                    doSelect(widgetAt(i, j), true);
                }
            }
        }
    }
}

// fading ----------------------------

void ColourGroupWidget::setupFade(QColor fadeToColour) {
    if(iFadeCalculator != NULL || iFadeParameters != NULL) {
        throw IllegalStateException("ColourGroupWidget::fade : Fade data is not NULL");
    }

    iFadeParameters = new FadeParameters;

    int rowSpan;
    int columnSpan;

    if(iFirstSelectedRow > iLastSelectedRow) {
        rowSpan = iFirstSelectedRow - iLastSelectedRow + 1;
        iFadeParameters->rowIncrement = false;
    } else {
        rowSpan = iLastSelectedRow - iFirstSelectedRow + 1;
        iFadeParameters->rowIncrement = true;
    }

    if(iFirstSelectedColumn > iLastSelectedColumn) {
        columnSpan =  iFirstSelectedColumn - iLastSelectedColumn + 1;
        iFadeParameters->columnIncrement = false;
    } else {
        columnSpan = iLastSelectedColumn - iFirstSelectedColumn + 1;
        iFadeParameters->columnIncrement = true;
    }

    iFadeParameters->maxWidgets = qMin(rowSpan, columnSpan);
    iFadeParameters->increments = rowSpan + columnSpan - 2;

    if(iFadeParameters->columnIncrement) {
        iFadeParameters->nextColumn = 0;
    } else {
        iFadeParameters->nextColumn = iFadeParameters->increments;
    }

    if(iFadeParameters->rowIncrement) {
        iFadeParameters->nextRow = 0;
    } else {
        iFadeParameters->nextRow = iFadeParameters->increments;
    }

    iFadeCalculator = new FadeCalculator(static_cast<QObject*>(this),
                                widgetAt(iFirstSelectedRow, iFirstSelectedColumn).colour(),
                                fadeToColour,
                                iFadeParameters->increments);

    connect(iFadeCalculator, SIGNAL(colourCalculated(QColor)), this, SLOT(colourCalculated(QColor)));
    connect(iFadeCalculator, SIGNAL(fadeComplete()), this, SLOT(fadeComplete()));

    iFadeParameters->rowSpan = rowSpan;
    iFadeParameters->columnSpan = columnSpan;

    iFadeParameters->maxWidgetsLineCount = 0;

    if(columnSpan > rowSpan) {
        iFadeParameters->numLinesMaxWidgets = columnSpan - rowSpan + 1;
    } else if(columnSpan < rowSpan) {
        iFadeParameters->numLinesMaxWidgets = rowSpan - columnSpan + 1;
    } else {
        iFadeParameters->numLinesMaxWidgets = 1;
    }

    iFadeParameters->numWidgets = 0;
}

void ColourGroupWidget::startFade() {
    iFadeCalculator->start();
}

void ColourGroupWidget::colourCalculated(QColor colour) {
    qDebug("\nNext row is %d, next column is %d, maxWidgets is %d", iFadeParameters->nextRow, iFadeParameters->nextColumn, iFadeParameters->maxWidgets);

    if(iFadeParameters->numWidgets < iFadeParameters->maxWidgets) {
        if(iFadeParameters->maxWidgetsLineCount == iFadeParameters->numLinesMaxWidgets) {
            iFadeParameters->numWidgets--;
        } else {
            iFadeParameters->numWidgets++;
        }
    }

    if(iFadeParameters->numWidgets == iFadeParameters->maxWidgets) {
        if(iFadeParameters->maxWidgetsLineCount < iFadeParameters->numLinesMaxWidgets) {
            iFadeParameters->maxWidgetsLineCount++;
        } else if(iFadeParameters->maxWidgetsLineCount == iFadeParameters->numLinesMaxWidgets) {
            iFadeParameters->numWidgets--;
        }
    }

    qDebug("numWidgets is %d", iFadeParameters->numWidgets);

    int rowStart, rowEnd;

    if(iFadeParameters->rowIncrement) {
        rowStart = iFadeParameters->nextRow;
        if(rowStart > iFadeParameters->rowSpan - 1) {
            rowStart = iFadeParameters->rowSpan - 1;
        }
        qDebug("rowStart is %d", rowStart);

        rowEnd = rowStart - iFadeParameters->numWidgets + 1;
        qDebug("rowEnd is %d", rowEnd);
    } else {
        rowEnd = iFadeParameters->nextRow;
        if(rowEnd > iFadeParameters->rowSpan - 1) {
            rowEnd = iFadeParameters->rowSpan - 1;
        }
        qDebug("rowEnd is %d", rowEnd);

        rowStart = rowEnd - iFadeParameters->numWidgets + 1;
        qDebug("rowStart is %d", rowStart);
    }

    int columnStart, columnEnd;

    if(iFadeParameters->columnIncrement) {
        columnEnd = iFadeParameters->nextColumn;
        if(columnEnd > iFadeParameters->columnSpan - 1) {
            columnEnd = iFadeParameters->columnSpan - 1;
        }
        qDebug("columnEnd is %d", columnEnd);

        columnStart = columnEnd - iFadeParameters->numWidgets + 1;
        qDebug("columnStart is %d", columnStart);
    } else {
        columnStart = iFadeParameters->nextColumn;
        if(columnStart > iFadeParameters->columnSpan - 1) {
            columnStart = iFadeParameters->columnSpan - 1;
        }
        qDebug("columnStart is %d", columnStart);

        columnEnd = columnStart - iFadeParameters->numWidgets + 1;
        qDebug("columnEnd is %d", columnEnd);
    }

    int row, column;

    for(row = rowStart, column = columnStart;
        iFadeParameters->rowIncrement?row >= rowEnd:row<=rowEnd,
        iFadeParameters->columnIncrement?column <= columnEnd:column >= columnEnd;
        iFadeParameters->rowIncrement?row--:row++,
        iFadeParameters->columnIncrement?column++:column--) {
        qDebug("widget at %d,%d", row, column);
        widgetAt(row, column).setColour(colour);
    }

    if(iFadeParameters->columnIncrement) {
        iFadeParameters->nextColumn++;
    } else {
        iFadeParameters->nextColumn--;
    }

    if(iFadeParameters->rowIncrement) {
        iFadeParameters->nextRow++;
    } else {
        iFadeParameters->nextRow--;
    }
}

void ColourGroupWidget::fadeComplete() {
    if(iFadeCalculator == NULL || iFadeParameters == NULL) {
        throw IllegalStateException("ColourGroupWIdget::fadeComplete fade data is NULL");
    }

    QTimer::singleShot(5, this, SLOT(deleteFader()));

    delete iFadeParameters;
    iFadeParameters = NULL;
}

void ColourGroupWidget::deleteFader() {
    if(iFadeCalculator == NULL) {
        throw IllegalStateException("ColourGroupWIdget::fadeComplete fade calculator is NULL");
    }

    delete iFadeCalculator;
    iFadeCalculator = NULL;
}

const QByteArray ColourGroupWidget::mimeData() {
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    int topRow;
    int bottomRow;
    int leftColumn;
    int rightColumn;

    getLeftRightTopBottomSelection(&topRow, &bottomRow, &leftColumn, &rightColumn);

    dataStream << bottomRow + 1 - topRow
               << rightColumn + 1 - leftColumn;

    for(int i = topRow; i < bottomRow + 1; i++) {
        for(int j = leftColumn; j < rightColumn + 1; j++) {
            ColourWidget& widget = widgetAt(i, j);

            dataStream << widget.colour();
            widget.addExtraData(dataStream);
        }
    }

    return itemData;
}

void ColourGroupWidget::handleMimeData(QByteArray itemData, ColourWidget& dropWidget) {
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);

    int row;
    int column;

    getWidgetPosition(dropWidget, &row, &column);

    selectOne(dropWidget);

    int rowSpan;
    int columnSpan;

    dataStream >> rowSpan >> columnSpan;

    int rowEnd = row + rowSpan;
    int columnEnd = column + columnSpan;

    for(int i = row; i < rowEnd; i++) {
        for(int j = column; j < columnEnd; j++) {
            ColourWidget& widget = widgetAt(i, j);

            QColor colour;
            dataStream >> colour;

            widget.setColour(colour);
            widget.handleExtraData(dataStream);

            if(i == rowEnd - 1 && j == columnEnd - 1) {
                selectArea(widget);
            }
        }
    }
}

void ColourGroupWidget::getLeftRightTopBottomSelection(int* topRow, int* bottomRow, int* leftColumn, int* rightColumn) {
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

void ColourGroupWidget::keyPressEvent(QKeyEvent *event) {
    Qt::Key key = (Qt::Key)event->key();

    if((QApplication::keyboardModifiers() & Qt::ShiftModifier) != 0) {
        if(selectedCount() > 0 && key != Qt::Key_Shift) {
            if(validKeyPress(key)) {
                selectDirection(key);
                return;
            }
        }
    }

    if(isGroupSelected() || isSingleSelected()) {
        if(((key == Qt::Key_V) || (key == Qt::Key_C)) &&
            (QApplication::keyboardModifiers() & Qt::ControlModifier) != 0) {
            QCoreApplication::sendEvent(&widgetAt(iFirstSelectedRow, iFirstSelectedColumn), event);
        }
    }
}
