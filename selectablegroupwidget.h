/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef SELECTABLEGROUPWIDGET_H
#define SELECTABLEGROUPWIDGET_H

#include <QWidget>

class Animation;

namespace Ui {

class SelectableWidget;

class SelectableGroupWidget : public QWidget {
    Q_OBJECT

public:
    explicit SelectableGroupWidget(QWidget *parent, int maxRow, int maxColumn);

    void selectOne(SelectableWidget &selectable);
    void select(SelectableWidget &led, bool select);
    void selectArea(SelectableWidget& widget);

    QList<SelectableWidget*>& selectedItems() { return iSelected; }

protected:
    virtual SelectableWidget& widgetAt(int row, int column) = 0;
    virtual void getWidgetPosition(SelectableWidget& widget, int* row, int* column) = 0;

    void clearSelection();

    void selectDirection(int direction);

    inline int maxRow() const { return iMaxRow; }
    inline int maxColumn () const { return iMaxColumn; }

    void setMaxRow(int max) { iMaxRow = max; }
    void setMaxColumn(int max) { iMaxColumn = max; }

private:
    void setFirstSelected(int row, int column);
    void selectArea(int rowStart, int rowEnd, int columnStart, int columnEnd);

    void selectRight();
    void selectLeft();
    void selectUp();
    void selectDown();

    int iMaxRow;
    int iMaxColumn;

    int iSelectedTopRow;
    int iSelectedLeftColumn;
    int iSelectedBottomRow;
    int iSelectedRightColumn;

    int iFirstSelectedRow;
    int iFirstSelectedColumn;

    QList<SelectableWidget*> iSelected;
};
}

#endif // SELECTABLEGROUPWIDGET_H
