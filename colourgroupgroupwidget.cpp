#include "colourgroupgroupwidget.h"

#include "colourgroupwidget.h"

using namespace AnimatorUi;

ColourGroupGroupWidget::ColourGroupGroupWidget(QWidget *parent, MainWindow& mainWindow) :
    SelectableGroupGroupWidget(parent, mainWindow)
{
}

void ColourGroupGroupWidget::setColour(QColor colour) {
    int groupNum = 0;
    for(int i = 0; i < iSelectedGroups.count(); i++) {
        while(!iSelectedGroups.contains(groupNum)) {
            groupNum++;
        }

        static_cast<ColourGroupWidget&>(*iSelectedGroups.value(groupNum++)).doSetColour(colour);
    }
}
