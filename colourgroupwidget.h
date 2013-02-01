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

    inline void setMaxRow(int max) { iNumRows = max; }
    inline void setMaxColumn(int max) { iNumColumns = max; }

    void fade();

    const QByteArray mimeData();
    void handleMimeData(QByteArray mimeData, ColourWidget &dropWidget);

    bool isGroupSelected();
    bool isMultipleSelected();
    bool isSingleSelected();

public slots:
    void setColour(QColor colour);

private slots:
    void nextColour(QColor colour);

protected:
   // void dragEnterEvent(QDragEnterEvent* event);
   // void dragMoveEvent(QDragMoveEvent* event);
  //  void dropEvent(QDropEvent* event);
    void keyPressEvent(QKeyEvent *event);

    virtual ColourWidget& widgetAt(int row, int column);
    virtual void getWidgetPosition(ColourWidget& widget, int* row, int* column);

    virtual bool validKeyPress(Qt::Key key) = 0;

    inline int selectedCount() { return iSelected.count(); }

    void clearSelection();
    void selectDirection(Qt::Key direction);

    inline int numRows() const { return iNumRows; }
    inline int numColumns () const { return iNumColumns; }

    QList<ColourWidget*> iSelected;

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

    int     iNextFrame;
    bool    iFrameIncrement;
};
}

#endif // SELECTABLEGROUPWIDGET_H
