/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "colourgroupwidget.h"

#include "Animation.h"
#include "colourwidget.h"
#include "framewidget.h"
#include "selectable.h"
#include "fadecalculator.h"
#include "colourgroupgroupwidget.h"
#include "ColourValue.h"
#include "FunctionValue.h"

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

void ColourGroupWidget::setValue(FrameValue &value) {
    colourGroupGroup().setValue(value);
}

void ColourGroupWidget::doSetValue(FrameValue &value) {
    SelectableWidget* item = NULL;

    foreach(item, selectedItems()){
        static_cast<ColourWidget*>(item)->setValue(value);
    }
}

// fading ----------------------------


// frameGroupWidget
FadeParameters& ColourGroupWidget::setupFunctionFade(QColor fadeToColour) {
    FadeParameters* fadeParameters = new FadeParameters();

    int columnSpan;

    FrameWidget& first = static_cast<FrameWidget&>(widgetAt(firstSelectedAreaPosition()));
    FrameWidget& last = static_cast<FrameWidget&>(widgetAt(lastSelectedAreaPosition()));

    if(first.frame().value().type() == kLinked || // we are fading to...
       (first.frame().value().type() == kColour &&
        first.frame().number() < last.frame().number())) // we have a fade range
    {
        if(firstSelectedAreaPosition().column() > lastSelectedAreaPosition().column()) {
            columnSpan = firstSelectedAreaPosition().column() - lastSelectedAreaPosition().column() + 1;
            fadeParameters->columnIncrement = false;
        } else {
            columnSpan = lastSelectedAreaPosition().column() - firstSelectedAreaPosition().column() + 1;
            fadeParameters->columnIncrement = true;
        }

        fadeParameters->firstPosition = firstSelectedAreaPosition();
        fadeParameters->lastPosition = lastSelectedAreaPosition();

        QColor fromColour = first.colour();

        fadeParameters->startColour = fromColour;
        fadeParameters->endColour = fadeToColour;
    } else {
        if(firstSelectedAreaPosition().column() > lastSelectedAreaPosition().column()) {
            columnSpan = firstSelectedAreaPosition().column() - lastSelectedAreaPosition().column() + 1;
            fadeParameters->columnIncrement = true;
        } else {
            columnSpan = lastSelectedAreaPosition().column() - firstSelectedAreaPosition().column() + 1;
            fadeParameters->columnIncrement = false;
        }

        fadeParameters->firstPosition = lastSelectedAreaPosition();
        fadeParameters->lastPosition = firstSelectedAreaPosition();

        QColor fromColour = last.colour();

        fadeParameters->startColour = fromColour;
        fadeParameters->endColour = fadeToColour;
    }

    if(firstSelectedAreaPosition().row() != lastSelectedAreaPosition().row()) {
        throw IllegalStateException("ColourGroupWidget::functionFade : can only function fade over a single row");
    }

    //fadeParameters->maxWidgets = columnSpan;
    fadeParameters->increments = columnSpan - 2;  // TODO why -2?

   // QColor fromColour = fromWidget.colour();

   // if(fadeParameters->columnIncrement) {
   //     fadeParameters->startColour = fromColour;
   //     fadeParameters->endColour = fadeToColour;
        //fadeParameters->nextColumn = 0;
   // } else {
    //    fadeParameters->startColour = fadeToColour;
    //    fadeParameters->endColour = fromColour;
   //     fadeParameters->nextColumn = 0;//iFadeParameters->increments;
    //    fadeParameters->columnIncrement = true;
   // }

    //fadeParameters->maxWidgetsLineCount = 0;
    //fadeParameters->numWidgets = 0;

    return *fadeParameters;
}

void ColourGroupWidget::fade() {
    fadeTo(static_cast<ColourWidget&>(widgetAt(lastSelectedAreaPosition())).colour());
}

void ColourGroupWidget::fadeTo(QColor fadeToColour) {
    FadeParameters& parameters = setupFade(fadeToColour);
    startFade(parameters);
}

void ColourGroupWidget::functionFade() {
    functionFadeTo(static_cast<ColourWidget&>(widgetAt(lastSelectedAreaPosition())).colour());
}

void ColourGroupWidget::functionFadeTo(QColor fadeToColour) {
    FadeParameters& parameters = setupFunctionFade(fadeToColour);


    FadeCalculator* fadeCalculator = new FadeCalculator(reinterpret_cast<QObject*>(this),
                                parameters.startColour,
                                parameters.endColour,
                                parameters.increments);

    if(parameters.columnIncrement) {
        for(int i = parameters.firstPosition.column() + 1; i <= parameters.lastPosition.column(); i++) {
            static_cast<ColourWidget&>(widgetAt(Position(0, i))).setValue(*(new FunctionValue(this, fadeCalculator->function())));
        }
    } else {
        for(int i = parameters.firstPosition.column() - 1; i >= parameters.lastPosition.column(); i--) {
            static_cast<ColourWidget&>(widgetAt(Position(0, i))).setValue(*(new FunctionValue(this, fadeCalculator->function())));
        }
    }
}

FadeParameters& ColourGroupWidget::setupFade(QColor fadeToColour) {
    if(iFadeCalculator != NULL || iFadeParameters != NULL) {
        throw IllegalStateException("ColourGroupWidget::fade : Fade data is not NULL");
    }

    iFadeParameters = new FadeParameters();

    int rowSpan;
    int columnSpan;

    if(firstSelectedAreaPosition().row() > lastSelectedAreaPosition().row()) {
        rowSpan = firstSelectedAreaPosition().row() - lastSelectedAreaPosition().row() + 1;
        iFadeParameters->rowIncrement = false;
    } else {
        rowSpan = lastSelectedAreaPosition().row() - firstSelectedAreaPosition().row() + 1;
        iFadeParameters->rowIncrement = true;
    }

    if(firstSelectedAreaPosition().column() > lastSelectedAreaPosition().column()) {
        columnSpan =  firstSelectedAreaPosition().column() - lastSelectedAreaPosition().column() + 1;
        iFadeParameters->columnIncrement = false;
    } else {
        columnSpan = lastSelectedAreaPosition().column() - firstSelectedAreaPosition().column() + 1;
        iFadeParameters->columnIncrement = true;
    }

    iFadeParameters->maxWidgets = qMin(rowSpan, columnSpan);
    iFadeParameters->increments = rowSpan + columnSpan - 2;  // TODO why -2?

    if(iFadeParameters->columnIncrement) {
        iFadeParameters->nextColumn = 0;
    } else {
        iFadeParameters->nextColumn = iFadeParameters->increments;
    }

    ColourWidget& fromWidget = static_cast<ColourWidget&>(widgetAt(firstSelectedAreaPosition()));
    QColor fromColour = fromWidget.colour();

    if(iFadeParameters->rowIncrement) {
        iFadeParameters->nextRow = 0;
    } else {
        iFadeParameters->nextRow = iFadeParameters->increments;
    }

    iFadeParameters->firstPosition = firstSelectedAreaPosition();
    iFadeParameters->lastPosition = lastSelectedAreaPosition();

    iFadeParameters->maxWidgetsLineCount = 0;

    if(columnSpan > rowSpan) {
        iFadeParameters->numLinesMaxWidgets = columnSpan - rowSpan + 1;
    } else if(columnSpan < rowSpan) {
        iFadeParameters->numLinesMaxWidgets = rowSpan - columnSpan + 1;
    } else {
        iFadeParameters->numLinesMaxWidgets = 1;
    }

    iFadeParameters->numWidgets = 0;
    iFadeParameters->startColour = fromColour;
    iFadeParameters->endColour = fadeToColour;

    return *iFadeParameters;

}

void ColourGroupWidget::startFade(FadeParameters& parameters) {
    iFadeCalculator = new FadeCalculator(reinterpret_cast<QObject*>(this),
                                parameters.startColour,
                                parameters.endColour,
                                parameters.increments);

    connect(iFadeCalculator, SIGNAL(colourCalculated(QColor)), this, SLOT(colourCalculated(QColor)));
    connect(iFadeCalculator, SIGNAL(fadeComplete()), this, SLOT(fadeComplete()));

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
        static_cast<ColourWidget&>(widgetAt(Position(row, column))).setValue(*(new ColourValue(this, colour)));
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
