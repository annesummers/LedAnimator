#include "selectablegroupgroupwidget.h"

#include "selectablegroupwidget.h"

#include "constants.h"

using namespace AnimatorUi;

SelectableGroupGroupWidget::SelectableGroupGroupWidget(QWidget *parent) :
    QWidget(parent),
    iHighestGroupNumber(INVALID) {
}

void SelectableGroupGroupWidget::selectGroup(int groupNumber, bool isSelected, bool singleSelect) {
    if(!isSelected) {
        iSelectedGroups.remove(groupNumber);
    } else {
        if(singleSelect) {
            selectSingleGroup(*iGroups.value(groupNumber));
        } else {
            if(!iSelectedGroups.contains(groupNumber)) {
                iSelectedGroups.insert(groupNumber, iGroups.value(groupNumber));
            }
        }
    }
}

void SelectableGroupGroupWidget::selectSingleGroup(SelectableGroupWidget& selectedWidget) {
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

void SelectableGroupGroupWidget::selectArea(int endGroupNumber, int endRow, int endColumn) {
    int groupNum = 0;
    int startGroupNum = INVALID;
    SelectableGroupWidget* group;
    int startRow = INVALID;
    int startColumn = INVALID;

    for(int i = 0; i <= endGroupNumber, groupNum <= endGroupNumber; i++) {
        while(!iGroups.contains(groupNum)) {
            groupNum++;
        }

        group = iGroups.value(groupNum);

        if(startGroupNum == INVALID) {
            if(group->isAnySelected()) {
                group->getLastSelected(&startRow, &startColumn);
                startGroupNum = groupNum;
            }
        }

        if(startGroupNum != INVALID){
            //selectGroup(group->groupNumber(), true);
            group->doSelectArea(startRow, startColumn, endRow, endColumn);
        }

        groupNum++;
    }
}

const QByteArray SelectableGroupGroupWidget::writeMimeData(bool cut) {
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    dataStream << iSelectedGroups.count();

    qDebug("Selected group count is %d", iSelectedGroups.count());

    int groupNum = 0;
    for(int i = 0; i < iSelectedGroups.count(); i++) {
        while(!iSelectedGroups.contains(groupNum)) {
            groupNum++;
        }

        iSelectedGroups.value(groupNum++)->doWriteMimeData(dataStream, cut);
    }

    return itemData;
}

bool SelectableGroupGroupWidget::handleMimeData(QByteArray itemData, int dropGroupNumber, int dropRow, int dropColumn, bool wrap, bool move) {
    bool wasCut = false;

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

        wasCut = iGroups.value(groupNumber++)->doHandleMimeData(dataStream, dropRow, dropColumn, &originRow, &originColumn, wrap, move);
    }

    return wasCut;
}

/*void SelectableGroupGroupWidget::handleOldMimeData(QByteArray itemData) {
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);

    int numGroups;

    dataStream >> numGroups;

    int groupNum;

    for(int i = 0; i < numGroups; i++) {
        dataStream >> groupNum;

        iGroups.value(groupNum)->doHandleOldMimeData(dataStream);
    }
}*/
