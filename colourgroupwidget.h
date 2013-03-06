/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef COLOURGROUPWIDGET_H
#define COLOURGROUPWIDGET_H

#include <QWidget>

#include "selectablegroupwidget.h"
#include "colourgroupgroupwidget.h"

namespace AnimatorModel {
class Animation;
}

class FadeCalculator;

using namespace AnimatorModel;

namespace AnimatorUi {

class ColourWidget;

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
                               ColourGroupGroupWidget &groupGroupWidget,
                               int groupNumber);


    void fade();
    void fadeTo(QColor fadeToColour);
    void doSetColour(QColor colour);

public slots:
    void setColour(QColor colour);

private slots:
    void colourCalculated(QColor colour);
    void fadeComplete();
    void deleteFader();

protected:
    void setupFade(QColor fadeToColor);
    void startFade();

    FadeCalculator* iFadeCalculator;

private:
    inline ColourGroupGroupWidget& colourGroupGroup() { return static_cast<ColourGroupGroupWidget&>(iGroupGroup); }

    void calculateNumWidgetsInLine();
    void calculateRowStartAndEnd(int* rowStart, int* rowEnd);
    void calculateColumnStartAndEnd(int* columnStart, int* columnEnd);

    FadeParameters* iFadeParameters;
};
}

#endif // COLOURGROUPWIDGET_H
