/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef SELECTABLEWIDGET_H
#define SELECTABLEWIDGET_H

#include <QWidget>

class Selectable;

namespace Ui {

class SelectableGroupWidget;

class SelectableWidget : public QWidget {
    Q_OBJECT

public:
    SelectableWidget(QWidget* parent, SelectableGroupWidget& groupWidget, Selectable& item);

    Selectable& item() {return iItem; }

protected slots:
    void selected() { update(); }

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);

    SelectableGroupWidget&  iSelectableGroup;

    Selectable&             iItem;
};
}

#endif // SELECTABLEWIDGET_H
