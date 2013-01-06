/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef SELECTABLEGROUPWIDGET_H
#define SELECTABLEGROUPWIDGET_H

#include <QtGui>

class Animation;

namespace Ui {

class ColourWidget;

class ColourGroupWidget : public QWidget {
    Q_OBJECT

public:
    explicit ColourGroupWidget(QWidget *parent, int numRows, int numColumns);

    void selectOne(ColourWidget &selectable);
    void select(ColourWidget &widget, bool select);
    void selectArea(ColourWidget& widget);

    void setColour(QColor colour);

    void setMaxRow(int max) { iNumRows = max; }
    void setMaxColumn(int max) { iNumColumns = max; }

    void copy();
    void paste();
    void fade();

    const QByteArray mimeData();
    void handleMimeData(QByteArray &mimeData, ColourWidget &dropWidget);

    bool isGroupSelected();
    bool isMultipleSelected();
    bool isSingleSelected();

private slots:
    void nextColour(QColor colour);

protected:
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dropEvent(QDropEvent* event);

    virtual ColourWidget& widgetAt(int row, int column);
    virtual void getWidgetPosition(ColourWidget& widget, int* row, int* column);

    int selectedCount() { return iSelected.count(); }

    void clearSelection();
    void selectDirection(int direction);

    inline int numRows() const { return iNumRows; }
    inline int numColumns () const { return iNumColumns; }

private:
    void setSingleSelected(ColourWidget &widget);

    void doGroupSelection();
    void doSelect(ColourWidget &widget, bool selected);

    void clearGroupSelection();

    void getLeftRightTopBottomSelection(int* topRow, int* bottomRow, int* leftColumn, int* rightColumn);

    int iNumRows;
    int iNumColumns;

    int iFirstSelectedRow;
    int iLastSelectedRow;

    int iFirstSelectedColumn;
    int iLastSelectedColumn;

    int iNextFrame;
    bool iFrameIncrement;

    QList<ColourWidget*> iSelected;
};
}

#endif // SELECTABLEGROUPWIDGET_H
