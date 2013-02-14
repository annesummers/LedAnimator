#include "colourgroupgroupwidget.h"

#include "colourgroupwidget.h"

using namespace AnimatorUi;

ColourGroupGroupWidget::ColourGroupGroupWidget(QWidget *parent) :
    QWidget(parent)
{
}

void ColourGroupGroupWidget::selected(ColourGroupWidget& selectedWidget) {
    ColourGroupWidget* widget = NULL;

    foreach(widget, iColourGroups) {
        if(widget != &selectedWidget) {
            widget->clearSelection();
        }
    }
}
