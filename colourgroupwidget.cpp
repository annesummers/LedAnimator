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
#include "selectablegroupgroupwidget.h"

#include "constants.h"
#include "exceptions.h"

using namespace Exception;
using namespace AnimatorUi;

ColourGroupWidget::ColourGroupWidget(QWidget *parent,
                                     int maxRow,
                                     int maxColumn,
                                     SelectableGroupGroupWidget *groupGroup) :
    SelectableGroupWidget(parent, maxRow, maxColumn, groupGroup),
    iFadeCalculator(NULL),
    iFadeParameters(NULL) {
}

void ColourGroupWidget::setColour(QColor colour) {
   // qDebug("setColour, %d", iSelected.count());
    if(colour.isValid()) {
        SelectableWidget* item = NULL;

        foreach(item, selectedItems()){
            static_cast<ColourWidget*>(item)->setColour(colour);
        }
    }
}

// fading ----------------------------

void ColourGroupWidget::fade() {
    fadeTo(static_cast<ColourWidget&>(widgetAt(lastSelectedRow(), lastSelectedColumn())).colour());
}

void ColourGroupWidget::fadeTo(QColor fadeToColour) {
    setupFade(fadeToColour);
    startFade();
}

void ColourGroupWidget::setupFade(QColor fadeToColour) {
    if(iFadeCalculator != NULL || iFadeParameters != NULL) {
        throw IllegalStateException("ColourGroupWidget::fade : Fade data is not NULL");
    }

    iFadeParameters = new FadeParameters;

    int rowSpan;
    int columnSpan;

    if(firstSelectedRow() > lastSelectedRow()) {
        rowSpan = firstSelectedRow() - lastSelectedRow() + 1;
        iFadeParameters->rowIncrement = false;
    } else {
        rowSpan = lastSelectedRow() - firstSelectedRow() + 1;
        iFadeParameters->rowIncrement = true;
    }

    if(firstSelectedColumn() > lastSelectedColumn()) {
        columnSpan =  firstSelectedColumn() - lastSelectedColumn() + 1;
        iFadeParameters->columnIncrement = false;
    } else {
        columnSpan = lastSelectedColumn() - firstSelectedColumn() + 1;
        iFadeParameters->columnIncrement = true;
    }

    iFadeParameters->maxWidgets = qMin(rowSpan, columnSpan);
    iFadeParameters->increments = rowSpan + columnSpan - 2;  // TODO why -2?

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

    iFadeParameters->firstRow = firstSelectedRow();
    iFadeParameters->firstColumn = firstSelectedColumn();

    iFadeParameters->lastRow = lastSelectedRow();
    iFadeParameters->lastColumn = lastSelectedColumn();

    iFadeParameters->maxWidgetsLineCount = 0;

    if(columnSpan > rowSpan) {
        iFadeParameters->numLinesMaxWidgets = columnSpan - rowSpan + 1;
    } else if(columnSpan < rowSpan) {
        iFadeParameters->numLinesMaxWidgets = rowSpan - columnSpan + 1;
    } else {
        iFadeParameters->numLinesMaxWidgets = 1;
    }

    iFadeParameters->numWidgets = 0;

    iFadeCalculator = new FadeCalculator(reinterpret_cast<QObject*>(this),
                                static_cast<ColourWidget&>(widgetAt(firstSelectedRow(), firstSelectedColumn())).colour(),
                                fadeToColour,
                                iFadeParameters->increments);

    connect(iFadeCalculator, SIGNAL(colourCalculated(QColor)), this, SLOT(colourCalculated(QColor)));
    connect(iFadeCalculator, SIGNAL(fadeComplete()), this, SLOT(fadeComplete()));
}

void ColourGroupWidget::startFade() {
    iFadeCalculator->start();
}

void ColourGroupWidget::colourCalculated(QColor colour) {
   // qDebug("\nNext row is %d, next column is %d, maxWidgets is %d", iFadeParameters->nextRow, iFadeParameters->nextColumn, iFadeParameters->maxWidgets);

    calculateNumWidgetsInLine();

    int row, rowEnd;
    calculateRowStartAndEnd(&row, &rowEnd);

    int column, columnEnd;
    calculateColumnStartAndEnd(&column, &columnEnd);

    bool rowIncrement = row <= rowEnd;
    bool columnIncrement = column <= columnEnd;

    for(;
        rowIncrement ? row <= rowEnd : row >= rowEnd,
        columnIncrement ? column <= columnEnd : column >= columnEnd;

        rowIncrement ? row++ : row--,
        columnIncrement ? column++ : column--) {

       // qDebug("widget at %d,%d", row, column);
        static_cast<ColourWidget&>(widgetAt(row, column)).setColour(colour);
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

void ColourGroupWidget::calculateNumWidgetsInLine() {
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

   // qDebug("numWidgets is %d", iFadeParameters->numWidgets);
}

void ColourGroupWidget::calculateRowStartAndEnd(int* rowStart, int* rowEnd) {
    int start;
    int end;

    if(iFadeParameters->rowIncrement) {
        start = iFadeParameters->nextRow + iFadeParameters->firstRow;
        if(start > iFadeParameters->lastRow) {
            start = iFadeParameters->lastRow;
        }
      //  qDebug("rowStart is %d", start);

        end = start - iFadeParameters->numWidgets + 1;
      //  qDebug("rowEnd is %d", end);
    } else {
        end = iFadeParameters->nextRow + iFadeParameters->lastRow;
        if(end > iFadeParameters->firstRow) {
            end = iFadeParameters->firstRow;
        }
     //   qDebug("rowEnd is %d", end);

        start =  end - iFadeParameters->numWidgets + 1;
     //   qDebug("rowStart is %d", start);
    }

    *rowStart = start;
    *rowEnd = end;
}

void ColourGroupWidget::calculateColumnStartAndEnd(int* columnStart, int* columnEnd) {
    int start;
    int end;

    if(iFadeParameters->columnIncrement) {
        end = iFadeParameters->nextColumn + iFadeParameters->firstColumn;
        if(end > iFadeParameters->lastColumn) {
            end = iFadeParameters->lastColumn;
        }
       // qDebug("columnEnd is %d", end);

        start = end - iFadeParameters->numWidgets + 1;
      //  qDebug("columnStart is %d", start);
    } else {
        start = iFadeParameters->nextColumn + iFadeParameters->lastColumn;
        if(start > iFadeParameters->firstColumn) {
            start = iFadeParameters->firstColumn;
        }
       // qDebug("columnStart is %d", start);

        end = start - iFadeParameters->numWidgets + 1;
       // qDebug("columnEnd is %d", end);
    }

    *columnStart = start;
    *columnEnd = end;
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
