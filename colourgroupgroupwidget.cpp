#include "colourgroupgroupwidget.h"

#include "selectablegroupwidget.h"

using namespace AnimatorUi;

ColourGroupGroupWidget::ColourGroupGroupWidget(QWidget *parent) :
    QWidget(parent)
{
}

void ColourGroupGroupWidget::selected(SelectableGroupWidget& selectedWidget) {
    SelectableGroupWidget* widget = NULL;

    foreach(widget, iColourGroups) {
        if(widget != &selectedWidget) {
            widget->clearSelection();
        }
    }
}
