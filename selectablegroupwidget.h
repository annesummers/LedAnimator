/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef SELECTABLEWIDGETGROUP_H
#define SELECTABLEWIDGETGROUP_H

#include <QWidget>
#include <QColorDialog>

#include "constants.h"

namespace AnimatorUi {

class SelectableGroupGroupWidget;
class SelectableWidget;
class Area;

class SelectableGroupWidget : public QWidget {
    Q_OBJECT

public:
    explicit SelectableGroupWidget(QWidget *parent,
                                   int numRows,
                                   int numColumns,
                                   SelectableGroupGroupWidget &groupGroupWidget,
                                   int groupNumber);

    void selectOne(SelectableWidget &selectable, bool singleSelect = true);

    void toggleOne(SelectableWidget &widget, bool singleSelect = true);
    void toggle(SelectableWidget &widget);

    void selectArea(SelectableWidget& widget, bool multipleAreas);

    bool isAreaSelected();
    bool isMultipleSelected();
    bool isSingleSelected();
    inline bool isAnySelected() { return selectedCount() > 0; }

    void clearAllSelection();

    virtual SelectableWidget& widgetAt(int row, int column);
    virtual Position widgetPosition(SelectableWidget& widget) = 0;

    inline QColorDialog& colourDialog() { return *iColourDialog; }

    inline void setMaxRow(int max) { iNumRows = max; }
    inline void setMaxColumn(int max) { iNumColumns = max; }

    const QByteArray writeMimeData(bool cut);
    bool handleMimeData(QByteArray mimeData, SelectableWidget &dropWidget, bool wrap, bool move = false);

    void doWriteMimeData(QDataStream& dataStream, bool cut);
    bool doHandleMimeData(QDataStream& dataStream, int fromGroupNumber, Position dropPosition, int *originRow, int *originColumn, bool wrap, bool move);
    void doSelectArea(Position start, Position end, bool multipleAreas);
    void doSelectDirection(Qt::Key direction);

    Position lastSelected();

    inline const int groupNumber() const { return iGroupNumber; }

    inline const QList<SelectableWidget*> selectedItems() const { return iSelected; }

protected:
    void keyPressEvent(QKeyEvent *event);

    virtual bool validKeyPress(Qt::Key key) = 0;

    virtual void moveItem(int fromGroup, int fromRow, int fromColumn, int toRow, int toColumn) = 0;
    virtual void cloneItem(int fromGroup, int fromRow, int fromColumn, int toRow, int toColumn) = 0;
    virtual void pasteItem(int fromGroup, int fromRow, int fromColumn, int toRow, int toColumn) = 0;

    virtual void moveToClipboard(int group, Position position)
        { Q_UNUSED(group); Q_UNUSED(position); }

    inline int selectedCount() { return iSelected.count(); }

    void selectDirection(Qt::Key direction, bool singleSelect = false);

    inline int numRows() const { return iNumRows; }
    inline int numColumns () const { return iNumColumns; }

 /*   inline const int firstSelectedRow() { return iFirstSelectedRow; }
    inline const int lastSelectedRow() { return iLastSelectedRow; }
    inline const int firstSelectedColumn() { return iFirstSelectedColumn; }
    inline const int lastSelectedColumn() { return iLastSelectedColumn; }*/

    SelectableGroupGroupWidget& iGroupGroup;
    int iGroupNumber;

private:
    void clearAreaSelection();
    void clearSelection();

    void setLastSelected(SelectableWidget& widget);

    void doGroupSelection();
    void doSelect(SelectableWidget &widget, bool selectedItems, bool singleSelect = false);

    void sortSelected();

    QList<SelectableWidget*> iSelected;

    QColorDialog* iColourDialog;

    int iNumRows;
    int iNumColumns;

  /*  int iFirstSelectedRow;
    int iLastSelectedRow;
    int iFirstSelectedColumn;
    int iLastSelectedColumn;*/

    QList<Area> iAreas;

    Position iTopLeftSelected;
    //Position iBottomRightSelected;

    Position iLastSelected;

   /* int iTopLeftSelectedRow;
    int iTopLeftSelectedColumn;
    int iBottomRightSelectedRow;
    int iBottomRightSelectedColumn;*/
};

class Area {
public:
    inline Area(Position first, Position last) :
        iFirstSelected(first), iLastSelected(last) {}

    inline const Position firstSelected() const { return iFirstSelected; }
    inline const Position lastSelected() const { return iLastSelected; }

    inline void setFirstSelected(Position firstSelected) { iFirstSelected = firstSelected; }
    inline void setLastSelected(Position lastSelected) { iLastSelected = lastSelected; }

private:
    Position iFirstSelected;
    Position iLastSelected;
};
}
#endif // SELECTABLEWIDGETGROUP_H
