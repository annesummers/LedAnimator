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
    qDebug("singleWidget mousePress");
    if (event->button() != Qt::LeftButton) {
        return;
    }

    if((QApplication::keyboardModifiers() & Qt::ControlModifier) != 0) {
         iSelectableGroup.select(*this, !iItem.isSelected());
         return;
    }

    if((QApplication::keyboardModifiers() & Qt::ShiftModifier) != 0) {
        iSelectableGroup.selectArea(*this);
        return;
    }


        //iSelectableGroup.selectOne(*this);
}

void SelectableWidget::mouseReleaseEvent(QMouseEvent* event){
    if (event->button() != Qt::LeftButton) {
        return;
    }

    if((QApplication::keyboardModifiers() & Qt::ControlModifier) != 0 ||
       (QApplication::keyboardModifiers() & Qt::ShiftModifier) != 0) {
         return;
    }

    iSelectableGroup.selectOne(*this);
}
