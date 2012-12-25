/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "selectablegroupwidget.h"

#include "animation.h"
#include "selectable.h"

using namespace Ui;

SelectableGroupWidget::SelectableGroupWidget(QWidget *parent) :
    QWidget(parent) {
}

void SelectableGroupWidget::clearSelection() {
    Selectable* selectable;
    foreach(selectable, iSelected) {
        selectable->select(false);
    }

    iSelected.clear();
}

void SelectableGroupWidget::select(Selectable &selectable, bool selected) {
    if(selected) {
        iSelected.append(&selectable);
    } else {
        iSelected.removeOne(&selectable);
    }

    selectable.select(selected);
}

void SelectableGroupWidget::selectOne(Selectable &selectable) {
    bool selected = false;

    if(iSelected.count() > 1) {
        selected = true;
    }

    if(iSelected.count() == 1 && iSelected.at(0) != &selectable) {
        selected = true;
    }

    if(iSelected.isEmpty()) {
        selected = true;
    }

    clearSelection();

    if(selected){
        select(selectable, selected);
    }
}
