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
class FrameValue;
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

    Position firstPosition;
    Position lastPosition;

    int rowSpan;
    int columnSpan;

    int maxWidgetsLineCount;
    int numLinesMaxWidgets;

    int numWidgets;
    int maxWidgets;

    int increments;

    QColor startColour;
    QColor endColour;
};

class ColourGroupWidget : public SelectableGroupWidget {
    Q_OBJECT

public:
    explicit ColourGroupWidget(QWidget *parent,
                               int numRows,
                               int numColumns,
                               ColourGroupGroupWidget &groupGroupWidget);


    void fade();
    void fadeTo(QColor fadeToColour);
    void functionFade();
    void functionFadeTo(QColor fadeToColour);
    void doSetValue(FrameValue& value);

public slots:
    void setValue(FrameValue& value);

private slots:
    void colourCalculated(QColor colour);
    void fadeComplete();
    void deleteFader();

protected:
    FadeParameters & setupFade(QColor fadeToColor);
    FadeParameters & setupFunctionFade(QColor fadeToColor);
    void startFade(FadeParameters &parameters);

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
