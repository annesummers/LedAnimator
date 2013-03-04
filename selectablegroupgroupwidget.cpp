#include "selectablegroupgroupwidget.h"

#include "selectablegroupwidget.h"

#include "constants.h"

using namespace AnimatorUi;

SelectableGroupGroupWidget::SelectableGroupGroupWidget(QWidget *parent) :
    QWidget(parent),
    iHighestGroupNumber(INVALID) {
}

void SelectableGroupGroupWidget::selected(SelectableGroupWidget& selectedWidget, bool isSelected, bool singleSelect) {
    if(!isSelected) {
        iSelectedGroups.remove(selectedWidget.groupNumber());
    } else {
        if(singleSelect) {
            selectSingle(selectedWidget);
        } else {
            if(!iSelectedGroups.contains(selectedWidget.groupNumber())) {
                iSelectedGroups.insert(selectedWidget.groupNumber(), &selectedWidget);
            }
        }
    }
}

void SelectableGroupGroupWidget::selectSingle(SelectableGroupWidget& selectedWidget) {
    SelectableGroupWidget* group;
    foreach(group, iGroups) {
        if(group != &selectedWidget) {
            group->clearSelection();
        }
    }

    iSelectedGroups.clear();
    iSelectedGroups.insert(selectedWidget.groupNumber(), &selectedWidget);
}

int SelectableGroupGroupWidget::addGroup(SelectableGroupWidget& newGroup) {
    int groupNumber;
    if(iFreeGroupNumbers.count() > 0) {
        groupNumber = iFreeGroupNumbers.takeFirst();
    } else {
        groupNumber = ++iHighestGroupNumber;
    }

    iGroups.insert(groupNumber, &newGroup);

    return groupNumber;
}

void SelectableGroupGroupWidget::removeGroup(SelectableGroupWidget& group) {
    iGroups.remove(group.groupNumber());

    if(group.groupNumber() == iHighestGroupNumber) {
        iHighestGroupNumber--;
    } else {
        iFreeGroupNumbers.append(group.groupNumber());
    }
}

const QByteArray SelectableGroupGroupWidget::writeMimeData() {
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    dataStream << iSelectedGroups.count();

    int groupNum = 0;
    for(int i = 0; i < iSelectedGroups.count(); i++) {
        while(!iSelectedGroups.contains(groupNum)) {
            groupNum++;
        }

        iSelectedGroups.value(groupNum++)->doWriteMimeData(dataStream);
    }

    return itemData;
}

void SelectableGroupGroupWidget::handleMimeData(QByteArray itemData, int dropGroupNumber, int dropRow, int dropColumn, bool wrap, bool move) {
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);

    int numCopyGroups;

    dataStream >> numCopyGroups;

    int originRow = INVALID;
    int originColumn = INVALID;

    int groupNumber = dropGroupNumber;

    for(int i = 0; i < numCopyGroups && groupNumber <= iHighestGroupNumber; i++) {
        while(!iGroups.contains(groupNumber)) {
            groupNumber++;
        }

        iGroups.value(groupNumber++)->doHandleMimeData(dataStream, dropRow, dropColumn, &originRow, &originColumn, wrap, move);
    }
}

void SelectableGroupGroupWidget::handleOldMimeData(QByteArray itemData) {
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);

    int numGroups;

    dataStream >> numGroups;

    int groupNum;

    for(int i = 0; i < numGroups; i++) {
        dataStream >> groupNum;

        iGroups.value(groupNum)->doHandleOldMimeData(dataStream);
    }
}
