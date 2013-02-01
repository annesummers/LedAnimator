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

#include "defaults.h"
#include "exceptions.h"

using namespace Exception;
using namespace Ui;

ColourGroupWidget::ColourGroupWidget(QWidget *parent,
                                             int maxRow,
                                             int maxColumn) :
    QWidget(parent),
    iNumRows(maxRow),
    iNumColumns(maxColumn),
    iFirstSelectedRow(0),
    iLastSelectedRow(0),
    iFirstSelectedColumn(0),
    iLastSelectedColumn(0),
    iNextFrame(0),
    iFrameIncrement(true) {
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
    qDebug("setColour, %d", iSelected.count());
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
    return  !isMultipleSelected() &&
            !isSingleSelected();
}

bool ColourGroupWidget::isSingleSelected() {
    return !isMultipleSelected() &&
            iFirstSelectedRow == iLastSelectedRow &&
            iFirstSelectedColumn == iLastSelectedColumn;
}

// selecting ----------------------------

void ColourGroupWidget::select(ColourWidget &widget, bool selected) {
    qDebug("select %d", selected);
    doSelect(widget, selected);

    if(selectedCount() == 1) {
        setSingleSelected(widget);
    } else {
        clearGroupSelection();
    }
}

void ColourGroupWidget::selectOne(ColourWidget &widget) {
    qDebug("selectOne");
    bool selected = false;

    if(iSelected.count() > 1) {
        selected = true;
    }

    if(iSelected.count() == 1 && iSelected.at(0) != &widget) {
        selected = true;
    }

    if(iSelected.isEmpty()) {
        selected = true;
    }

    clearSelection();

    if(selected){
        doSelect(widget, selected);

        setSingleSelected(widget);
    } else {
        clearGroupSelection();
    }
}

void ColourGroupWidget::selectArea(ColourWidget& widget) {
    qDebug("selectArea");
    if(!isSingleSelected()) {
        return;
    }

    getWidgetPosition(widget, &iLastSelectedRow, &iLastSelectedColumn);

    doGroupSelection();
}

void ColourGroupWidget::selectDirection(Qt::Key direction) {
    qDebug("selectDirection");
    if(!isGroupSelected() && !isSingleSelected()) {
        return;
    }

    switch(direction) {
        case Qt::Key_Up:
        qDebug("Direction Up");
            if(iLastSelectedRow - 1 >=0 ) {
                iLastSelectedRow--;
            }
            break;
        case Qt::Key_Down:
        qDebug("Direction Down");
            if(iLastSelectedRow + 1 < iNumRows ) {
                iLastSelectedRow++;
            }
            break;
        case Qt::Key_Left:
        qDebug("Direction Left");
            if(iLastSelectedColumn - 1 >=0) {
                iLastSelectedColumn--;
            }
            break;
        case Qt::Key_Right:
        qDebug("Direction Right");
            if(iLastSelectedColumn + 1 < iNumColumns) {
                iLastSelectedColumn++;
            }
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

void ColourGroupWidget::fade() {
    if(!isGroupSelected()) {
        return;
    }

    QColor colour = QColorDialog::getColor(Qt::white,
                                           this,
                                           "Select Color",
                                           QColorDialog::DontUseNativeDialog);
    if(colour.isValid()) {
        FadeCalculator* fadeCalculator = new FadeCalculator(this,
                                            widgetAt(0, iFirstSelectedColumn).colour(),
                                            colour,
                                            iSelected.count() - 1);

        connect(fadeCalculator, SIGNAL(colourCalculated(QColor)), this, SLOT(nextColour(QColor)));

        iNextFrame = iFirstSelectedColumn;

        if(iFirstSelectedColumn < iLastSelectedColumn) {
            iFrameIncrement = true;
        }

        // TODO make it work with areas

        fadeCalculator->start();
    }
}

void ColourGroupWidget::nextColour(QColor colour) {
    widgetAt(0, iNextFrame).setColour(colour);

    if(iFrameIncrement) {
        iNextFrame++;
    } else {
        iNextFrame--;
    }
}

const QByteArray ColourGroupWidget::mimeData() {
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    int topRow;
    int bottomRow;
    int leftColumn;
    int rightColumn;

    getLeftRightTopBottomSelection(&topRow, &bottomRow, &leftColumn, &rightColumn);

    int num = (bottomRow + 1 - topRow) * (rightColumn + 1 - leftColumn);
    dataStream << num;

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

    int topRow;
    int bottomRow;
    int leftColumn;
    int rightColumn;

    getLeftRightTopBottomSelection(&topRow, &bottomRow, &leftColumn, &rightColumn);

    int rowSpan = bottomRow - topRow;
    int columnSpan = rightColumn - leftColumn;

    int row;
    int column;

    getWidgetPosition(dropWidget, &row, &column);

    if(rowSpan + row > iNumRows - 1) {
        rowSpan = iNumRows - 1 - row;
    }

    if(columnSpan + column > iNumColumns - 1) {
        columnSpan = iNumColumns - 1 - column;
    }

    selectOne(dropWidget);

    int num;

    dataStream >> num;

    for(int i = 0; i < rowSpan + 1; i++) {
        for(int j = 0; j < columnSpan + 1; j++) {
            ColourWidget& widget = widgetAt(i + row, j + column);

            QColor colour;
            dataStream >> colour;

            widget.setColour(colour);
            widget.handleExtraData(dataStream);

            if(i == rowSpan && j == columnSpan) {
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
    if((QApplication::keyboardModifiers() & Qt::ShiftModifier) != 0) {
        Qt::Key key = (Qt::Key)event->key();
        qDebug("key press is ");
        switch(key) {
        case Qt::Key_Shift:
            qDebug("Shift");
            break;
        case Qt::Key_Up:
            qDebug("Up");
            break;
        case Qt::Key_Down:
            qDebug("Down");
            break;
        case Qt::Key_Left:
            qDebug("Left");
            break;
        case Qt::Key_Right:
            qDebug("Right");
            break;
        }

        if(selectedCount() > 0 && key != Qt::Key_Shift) {
            if(validKeyPress(key)) {
                selectDirection(key);
            }
        }
    }
}
/*
void ColourGroupWidget::dragEnterEvent(QDragEnterEvent* event) {
    qDebug("ColourGroupWidget::dragEnter");
    // TODO mimetype from revelevant widget
    if (event->mimeData()->hasFormat("application/x-frameitemdata")) {
         if (event->source() != 0) {
             event->accept();
         }
     } else {
         event->ignore();
     }
}

void ColourGroupWidget::dragMoveEvent(QDragMoveEvent* event) {
    // TODO mimetype from revelevant widget
    if (event->mimeData()->hasFormat("application/x-frameitemdata")) {
        if (event->source() != 0) {
            event->accept();
        }
    } else {
        event->ignore();
    }
}

void ColourGroupWidget::dropEvent(QDropEvent *event) {
    // TODO mimetype from revelevant widget
     if (event->mimeData()->hasFormat("application/x-frameitemdata")) {
         QByteArray itemData = event->mimeData()->data("application/x-leditemdata");
         QDataStream dataStream(&itemData, QIODevice::ReadOnly);

    if (event->source() != 0) {
         event->setDropAction(Qt::CopyAction);
         event->accept();
        }
    } else {
        event->ignore();
    }
}*/
