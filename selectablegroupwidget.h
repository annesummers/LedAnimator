#ifndef SELECTABLEWIDGETGROUP_H
#define SELECTABLEWIDGETGROUP_H

#include <QWidget>
#include <QColorDialog>

namespace AnimatorUi {

class SelectableGroupGroupWidget;
class SelectableWidget;

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

    void selectArea(SelectableWidget& widget, bool singleSelect = false);

    bool isGroupSelected();
    bool isMultipleSelected();
    bool isSingleSelected();
    inline bool isAnySelected() { return selectedCount() > 0; }

    void clearSelection();

    virtual SelectableWidget& widgetAt(int row, int column);
    virtual void getWidgetPosition(SelectableWidget& widget, int* row, int* column);

    inline QColorDialog& colourDialog() { return *iColourDialog; }

    inline void setMaxRow(int max) { iNumRows = max; }
    inline void setMaxColumn(int max) { iNumColumns = max; }

    const QByteArray writeMimeData(bool cut);
    bool handleMimeData(QByteArray mimeData, SelectableWidget &dropWidget, bool wrap, bool move = false);

    void doWriteMimeData(QDataStream& dataStream, bool cut);
    bool doHandleMimeData(QDataStream& dataStream, int dropRow, int dropColumn, int *originRow, int *originColumn, bool wrap, bool move);

    inline const int groupNumber() const { return iGroupNumber; }

    inline const QList<SelectableWidget*> selectedItems() const { return iSelected; }

protected:
    void keyPressEvent(QKeyEvent *event);

    virtual bool validKeyPress(Qt::Key key) = 0;

    virtual void moveItem(int fromGroup, int fromRow, int fromColumn, int toRow, int toColumn) = 0;
    virtual void cloneItem(int fromGroup, int fromRow, int fromColumn, int toRow, int toColumn) = 0;
    virtual void pasteItem(int fromGroup, int fromRow, int fromColumn, int toRow, int toColumn) = 0;

    virtual void moveToClipboard(int group, int row, int column)
        { Q_UNUSED(group); Q_UNUSED(row); Q_UNUSED(column); }

    inline int selectedCount() { return iSelected.count(); }

    void selectDirection(Qt::Key direction, bool singleSelect = false);

    inline int numRows() const { return iNumRows; }
    inline int numColumns () const { return iNumColumns; }

    inline const int firstSelectedRow() { return iFirstSelectedRow; }
    inline const int lastSelectedRow() { return iLastSelectedRow; }
    inline const int firstSelectedColumn() { return iFirstSelectedColumn; }
    inline const int lastSelectedColumn() { return iLastSelectedColumn; }

    SelectableGroupGroupWidget& iGroupGroup;
    int iGroupNumber;

private:
    void setNoneSelected();
    void setLastSelected(SelectableWidget& widget);

    void doGroupSelection();
    void doSelect(SelectableWidget &widget, bool selectedItems, bool singleSelect = false);

    void sortSelected();

    QList<SelectableWidget*> iSelected;

    QColorDialog* iColourDialog;

    int iNumRows;
    int iNumColumns;

    int iFirstSelectedRow;
    int iLastSelectedRow;
    int iFirstSelectedColumn;
    int iLastSelectedColumn;

    int iTopLeftSelectedRow;
    int iTopLeftSelectedColumn;
    int iBottomRightSelectedRow;
    int iBottomRightSelectedColumn;
};
}
#endif // SELECTABLEWIDGETGROUP_H
