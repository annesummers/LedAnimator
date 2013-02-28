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
                                   SelectableGroupGroupWidget* groupGroupWidget = NULL);

    void selectOne(SelectableWidget &selectable);

    void toggleOne(SelectableWidget &widget);
    void toggle(SelectableWidget &widget);

    void selectArea(SelectableWidget& widget);

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

    virtual const QByteArray writeMimeData();
    virtual void handleMimeData(QByteArray writeMimeData, SelectableWidget &dropWidget, bool move, bool wrap);

    void handleOldMimeData(QByteArray itemData);

    inline const QList<SelectableWidget*> selectedItems() const { return iSelected; }

protected:
    void keyPressEvent(QKeyEvent *event);

    virtual bool validKeyPress(Qt::Key key) = 0;

    virtual void deleteIfNeeded(int row, int column) { Q_UNUSED(row); Q_UNUSED(column); }

    virtual void moveItem(int fromGroup, int fromRow, int fromColumn, int toRow, int toColumn) = 0;
    virtual void copyItem(int fromGroup, int fromRow, int fromColumn, int toRow, int toColumn) = 0;

    inline int selectedCount() { return iSelected.count(); }

    void selectDirection(Qt::Key direction);

    inline int numRows() const { return iNumRows; }
    inline int numColumns () const { return iNumColumns; }

    inline const int firstSelectedRow() { return iFirstSelectedRow; }
    inline const int lastSelectedRow() { return iLastSelectedRow; }
    inline const int firstSelectedColumn() { return iFirstSelectedColumn; }
    inline const int lastSelectedColumn() { return iLastSelectedColumn; }

    void getLeftRightTopBottomSelection(int *const topRow, int *const bottomRow, int *const leftColumn, int *const rightColumn) const ;

    SelectableGroupGroupWidget* iGroupGroup;
    int iGroupNumber;

private:
    void setSingleSelected(SelectableWidget &widget);

    void doGroupSelection();
    void doSelect(SelectableWidget &widget, bool selectedItems);

    void clearGroupSelection();

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
