/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "selectablewidget.h"

#include <QtGui>

#include "selectable.h"
#include "selectablegroupwidget.h"

using namespace Ui;

SelectableWidget::SelectableWidget(QWidget* parent, SelectableGroupWidget &groupWidget, Selectable& item) :
    QWidget(parent),
    iSelectableGroup(groupWidget),
    iItem(item) {

    connect(&item, SIGNAL(selected()), this, SLOT(selected()));
}

void SelectableWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        if((QApplication::keyboardModifiers() & Qt::ControlModifier) != 0) {

             iSelectableGroup.select(iItem, !iItem.isSelected());

        } else {
            iDragStartPosition = event->pos();

            iSelectableGroup.selectOne(iItem);
        }
    }
}
