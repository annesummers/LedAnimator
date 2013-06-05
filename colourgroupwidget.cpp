/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "colourgroupwidget.h"

#include "animation.h"
#include "colourwidget.h"
#include "selectable.h"
#include "fadecalculator.h"
#include "colourgroupgroupwidget.h"

#include "constants.h"
#include "exceptions.h"

using namespace Exception;
using namespace AnimatorUi;

ColourGroupWidget::ColourGroupWidget(QWidget *parent,
                                     int maxRow,
                                     int maxColumn,
                                     ColourGroupGroupWidget &groupGroup,
                                     int groupNumber) :
    SelectableGroupWidget(parent, maxRow, maxColumn, groupGroup, groupNumber),
    iFadeCalculator(NULL),
    iFadeParameters(NULL) {
}

void ColourGroupWidget::setColour(QColor colour) {
    colourGroupGroup().setColour(colour);
}

void ColourGroupWidget::doSetColour(QColor colour) {
    SelectableWidget* item = NULL;

    foreach(item, selectedItems()){
        static_cast<ColourWidget*>(item)->setColour(colour);
    }
}

// fading ----------------------------

void ColourGroupWidget::fade() {
    fadeTo(static_cast<ColourWidget&>(widgetAt(lastSelectedPosition())).colour());
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

    if(firstSelectedPosition().row() > lastSelectedPosition().row()) {
        rowSpan = firstSelectedPosition().row() - lastSelectedPosition().row() + 1;
        iFadeParameters->rowIncrement = false;
    } else {
        rowSpan = lastSelectedPosition().row() - firstSelectedPosition().row() + 1;
        iFadeParameters->rowIncrement = true;
    }

    if(firstSelectedPosition().column() > lastSelectedPosition().column()) {
        columnSpan =  firstSelectedPosition().column() - lastSelectedPosition().column() + 1;
        iFadeParameters->columnIncrement = false;
    } else {
        columnSpan = lastSelectedPosition().column() - firstSelectedPosition().column() + 1;
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

    iFadeParameters->firstPosition = firstSelectedPosition();
    iFadeParameters->lastPosition = lastSelectedPosition();

    iFadeParameters->maxWidgetsLineCount = 0;

    if(columnSpan > rowSpan) {
        iFadeParameters->numLinesMaxWidgets = columnSpan - rowSpan + 1;
    } else if(columnSpan < rowSpan) {
        iFadeParameters->numLinesMaxWidgets = rowSpan - columnSpan + 1;
    } else {
        iFadeParameters->numLinesMaxWidgets = 1;
    }

    iFadeParameters->numWidgets = 0;

    ColourWidget& fromWidget = static_cast<ColourWidget&>(widgetAt(firstSelectedPosition()));
    QColor fromColour = fromWidget.colour();

    iFadeCalculator = new FadeCalculator(reinterpret_cast<QObject*>(this),
                                fromColour,
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
        static_cast<ColourWidget&>(widgetAt(Position(row, column))).setColour(colour);
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
        start = iFadeParameters->nextRow + iFadeParameters->firstPosition.row();
        if(start > iFadeParameters->lastPosition.row()) {
            start = iFadeParameters->lastPosition.row();
        }
      //  qDebug("rowStart is %d", start);

        end = start - iFadeParameters->numWidgets + 1;
      //  qDebug("rowEnd is %d", end);
    } else {
        end = iFadeParameters->nextRow + iFadeParameters->lastPosition.row();
        if(end > iFadeParameters->firstPosition.row()) {
            end = iFadeParameters->firstPosition.row();
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
        end = iFadeParameters->nextColumn + iFadeParameters->firstPosition.column();
        if(end > iFadeParameters->lastPosition.column()) {
            end = iFadeParameters->lastPosition.column();
        }
       // qDebug("columnEnd is %d", end);

        start = end - iFadeParameters->numWidgets + 1;
      //  qDebug("columnStart is %d", start);
    } else {
        start = iFadeParameters->nextColumn + iFadeParameters->lastPosition.column();
        if(start > iFadeParameters->firstPosition.column()) {
            start = iFadeParameters->firstPosition.column();
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
