/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef SELECTABLEWIDGETGROUP_H
#define SELECTABLEWIDGETGROUP_H

#include <QWidget>
#include <QColorDialog>
#include <QMimeData>

#include "selectablegroupgroupwidget.h"

#include "constants.h"

namespace AnimatorUi {

class SelectableWidget;
class Area;

class Area {
public:
    inline Area(Position first, Position last) :
        iFirstSelected(first), iLastSelected(last) {}
    inline Area(const Area& area) :
        iFirstSelected(area.firstSelected()), iLastSelected(area.lastSelected()) {}

    inline const Position firstSelected() const { return iFirstSelected; }
    inline const Position lastSelected() const { return iLastSelected; }

    inline void setFirstSelected(Position firstSelected) { iFirstSelected = firstSelected; }
    inline void setLastSelected(Position lastSelected) { iLastSelected = lastSelected; }

    inline Area& operator=(const Area& area) { setFirstSelected(area.firstSelected()); setLastSelected(area.lastSelected()); return *this; }

private:
    Position iFirstSelected;
    Position iLastSelected;
};

class SelectableGroupWidget : public QWidget {
    Q_OBJECT

public:
    explicit SelectableGroupWidget(QWidget *parent,
                                   int numRows,
                                   int numColumns,
                                   SelectableGroupGroupWidget &groupGroupWidget);

    void selectOne(SelectableWidget &selectable, bool singleSelect);
    void select(SelectableWidget &widget, bool select);
    void selectArea(SelectableWidget& widget, bool multipleAreas);

    bool isAreaSelected() const;
    bool isMultipleSelected() const;
    bool isSingleSelected() const;
    inline bool isAnySelected() const { return selectedCount() > 0; }

    void clearAllSelection();

    void cutSelected();
    virtual void copySelected();
    void paste(bool wrap);
    void paste(SelectableWidget& dropWidget, bool wrap);

    virtual SelectableWidget& widgetAt(Position position);
    virtual Position widgetPosition(SelectableWidget& widget) = 0;

    inline QColorDialog& colourDialog() { return *iColourDialog; }

    inline void setMaxRow(int max) { iNumRows = max; }
    inline void setMaxColumn(int max) { iNumColumns = max; }

    inline QString mimeType() const { return iGroupGroup.mimeType(); }

    inline QMimeData* mimeData(bool cut) { return iGroupGroup.mimeData(cut); }
    bool handleMimeData(QByteArray mimeData, SelectableWidget &dropWidget, bool wrap, bool move = false);

    void doWriteMimeData(QDataStream& dataStream, bool cut);
    bool doHandleMimeData(QDataStream& dataStream, int fromGroupNumber, Position dropPosition, int *originRow, int *originColumn, bool wrap, bool move);
    void doSelectArea(Position start, Position end);
    void doSelectDirection(Qt::Key direction);

    Position lastSelectedPosition() const;
    Position firstSelectedPosition() const;

    inline int groupNumber() const { return iGroupNumber; }

    inline const QList<SelectableWidget*> selectedItems() const { return iSelected; }

    inline const Position firstSelectedAreaPosition() const { if(iAreas.count() == 1) {
            return iAreas.first().firstSelected(); }  else { return Position();}}
    inline const Position lastSelectedAreaPosition() const { if(iAreas.count() == 1) {
            return iAreas.first().lastSelected(); } else { return Position();} }

    virtual void moveToClipboard(int group, Position position)
        { Q_UNUSED(group); Q_UNUSED(position); }

protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent* event);

    virtual bool validKeyPress(Qt::Key key) = 0;

    virtual void moveItem(int fromGroup, Position fromPosition, Position toPosition) = 0;
    virtual void cloneItem(int fromGroup, Position fromPosition, Position toPosition) = 0;
    virtual void pasteClipboardItem(int fromGroup, Position fromPosition, Position toPosition) = 0;

    inline int selectedCount() const { return iSelected.count(); }

    void selectDirection(Qt::Key direction, bool singleSelect = false);

    inline int numRows() const { return iNumRows; }
    inline int numColumns () const { return iNumColumns; }

    SelectableGroupGroupWidget& iGroupGroup;
    int iGroupNumber;

private:
    const QByteArray writeMimeData(bool cut);

    void clearAreaSelection();
    void clearSelection();

    void setLastSelected(SelectableWidget *widget);
    void setFirstSelected(SelectableWidget *widget);

    void doGroupSelection();
    void doSelect(SelectableWidget &widget, bool selectedItems, bool singleSelect = false);

    void sortSelected();

    QList<SelectableWidget*> iSelected;

    QColorDialog* iColourDialog;

    int iNumRows;
    int iNumColumns;

    QList<Area> iAreas;

    Position iTopLeftSelected;
    Position iFirstSelected;
    Position iLastSelected;
};

}
#endif // SELECTABLEWIDGETGROUP_H
