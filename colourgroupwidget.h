/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef COLOURGROUPWIDGET_H
#define COLOURGROUPWIDGET_H

#include <QWidget>

#include "selectablegroupwidget.h"

namespace AnimatorModel {
class Animation;
}

class FadeCalculator;

using namespace AnimatorModel;

namespace AnimatorUi {

class ColourWidget;
class SelectableGroupGroupWidget;

struct FadeParameters {
    int nextColumn;
    int nextRow;

    bool rowIncrement;
    bool columnIncrement;

    int firstRow;
    int firstColumn;

    int lastRow;
    int lastColumn;

    int rowSpan;
    int columnSpan;

    int maxWidgetsLineCount;
    int numLinesMaxWidgets;

    int numWidgets;
    int maxWidgets;

    int increments;
};

class ColourGroupWidget : public SelectableGroupWidget {
    Q_OBJECT

public:
    explicit ColourGroupWidget(QWidget *parent,
                               int numRows,
                               int numColumns,
                               SelectableGroupGroupWidget *groupGroupWidget = NULL);


    void fade();
    void fadeTo(QColor fadeToColour);

public slots:
    void setColour(QColor colour);

private slots:
    void colourCalculated(QColor colour);
    void fadeComplete();
    void deleteFader();

protected:
    void setupFade(QColor fadeToColor);
    void startFade();

    void readMimeData(QDataStream data);
    void writeMimeData(QDataStream data);

    FadeCalculator* iFadeCalculator;

private:
    void calculateNumWidgetsInLine();
    void calculateRowStartAndEnd(int* rowStart, int* rowEnd);
    void calculateColumnStartAndEnd(int* columnStart, int* columnEnd);

    FadeParameters* iFadeParameters;
};
}

#endif // COLOURGROUPWIDGET_H
