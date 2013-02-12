/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef SELECTABLEGROUPWIDGET_H
#define SELECTABLEGROUPWIDGET_H

#include <QtGui>

class Animation;
class FadeCalculator;

namespace Ui {

class ColourWidget;
class ColourGroupGroupWidget;

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

class ColourGroupWidget : public QWidget {
    Q_OBJECT

public:
    explicit ColourGroupWidget(QWidget *parent,
                               int numRows,
                               int numColumns,
                               ColourGroupGroupWidget* groupGroupWidget = NULL);

    void selectOne(ColourWidget &selectable);
    void toggleOne(ColourWidget &widget);
    void toggle(ColourWidget &widget);
    void selectArea(ColourWidget& widget);

    void clearSelection();

    inline void setMaxRow(int max) { iNumRows = max; }
    inline void setMaxColumn(int max) { iNumColumns = max; }

    void startFade();

    const QByteArray mimeData();
    void handleMimeData(QByteArray mimeData, ColourWidget &dropWidget);

    bool isGroupSelected();
    bool isMultipleSelected();
    bool isSingleSelected();
    inline bool isAnySelected() { return selectedCount() > 0; }

signals:
    void fadeSetup();

public slots:
    void setColour(QColor colour);
    void setupFade(QColor fadeToColour);

private slots:
    void colourCalculated(QColor colour);
    void fadeComplete();
    void deleteFader();

protected:
    void keyPressEvent(QKeyEvent *event);

    virtual ColourWidget& widgetAt(int row, int column);
    virtual void getWidgetPosition(ColourWidget& widget, int* row, int* column);

    virtual bool validKeyPress(Qt::Key key) = 0;

    inline int selectedCount() { return iSelected.count(); }

    void selectDirection(Qt::Key direction);

    inline int numRows() const { return iNumRows; }
    inline int numColumns () const { return iNumColumns; }

    QList<ColourWidget*> iSelected;

    FadeCalculator* iFadeCalculator;

private:
    void setSingleSelected(ColourWidget &widget);

    void doGroupSelection();
    void doSelect(ColourWidget &widget, bool selected);

    void clearGroupSelection();

    void getLeftRightTopBottomSelection(int* topRow, int* bottomRow, int* leftColumn, int* rightColumn);

    void calculateNumWidgetsInLine();
    void calculateRowStartAndEnd(int* rowStart, int* rowEnd);
    void calculateColumnStartAndEnd(int* columnStart, int* columnEnd);

    int iNumRows;
    int iNumColumns;

    int iFirstSelectedRow;
    int iLastSelectedRow;
    int iFirstSelectedColumn;
    int iLastSelectedColumn;

    FadeParameters* iFadeParameters;

    ColourGroupGroupWidget* iGroupGroup;
};
}

#endif // SELECTABLEGROUPWIDGET_H
