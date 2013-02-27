#include "selectablegroupgroupwidget.h"

#include "selectablegroupwidget.h"

using namespace AnimatorUi;

SelectableGroupGroupWidget::SelectableGroupGroupWidget(QWidget *parent) :
    QWidget(parent)
{
}

void SelectableGroupGroupWidget::selected(SelectableGroupWidget& selectedWidget) {
    SelectableGroupWidget* widget = NULL;

    foreach(widget, iColourGroups) {
        if(widget != &selectedWidget) {
            widget->clearSelection();
        }
    }
}

int SelectableGroupGroupWidget::addGroup(SelectableGroupWidget& newGroup) {
    iColourGroups.append(&newGroup);
    return iColourGroups.count() - 1;
}
