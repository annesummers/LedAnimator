/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

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
            group->clearAllSelection();
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

void SelectableGroupGroupWidget::selectArea(int lastGroupNumber, Position end, bool multipleAreas) {
    int groupNumber = 0;
    int firstGroupNumber = INVALID;
    SelectableGroupWidget* group;
    Position start;

    for(int i = 0; i < iGroups.count()&&  groupNumber <= iHighestGroupNumber; i++) {
        while(!iGroups.contains(groupNumber)) {
            groupNumber++;
        }

        group = iGroups.value(groupNumber);

        if(firstGroupNumber == INVALID) {
            if(group->isAnySelected()) {
                start = group->lastSelected();
                firstGroupNumber = groupNumber;
            }
        }

        groupNumber++;
    }

    int endGroupNumber;
    if(firstGroupNumber < lastGroupNumber) {
        groupNumber = firstGroupNumber;
        endGroupNumber = lastGroupNumber;
    } else {
        groupNumber = lastGroupNumber;
        endGroupNumber = firstGroupNumber;
    }

    for(int i = 0; i < iGroups.count() && groupNumber <= endGroupNumber; i++) {
        while(!iGroups.contains(groupNumber)) {
            groupNumber++;
        }

        iGroups.value(groupNumber)->doSelectArea(start, end, multipleAreas);
        groupNumber++;
    }
}

bool SelectableGroupGroupWidget::isOtherSelected(int groupNumber) {
    int otherGroupNumber = 0;
    for(int i = 0; otherGroupNumber <= iHighestGroupNumber; i++) {
        while(!iGroups.contains(otherGroupNumber)) {
            otherGroupNumber++;
        }

        if(groupNumber != otherGroupNumber) {
            return true;
        }

        otherGroupNumber++;
    }

    return false;
}

const QByteArray SelectableGroupGroupWidget::writeMimeData(bool cut) {
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    dataStream << iSelectedGroups.count();

    qDebug("Selected group count is %d", iSelectedGroups.count());

    int fromGroupNumber;

    int groupNum = 0;
    for(int i = 0; i < iSelectedGroups.count(); i++) {
        while(!iSelectedGroups.contains(groupNum)) {
            groupNum++;
        }

        iSelectedGroups.value(groupNum++)->doWriteMimeData(dataStream, cut);
    }

    return itemData;
}

bool SelectableGroupGroupWidget::handleMimeData(QByteArray itemData,
                                                int dropGroupNumber,
                                                Position dropPosition,
                                                bool wrap,
                                                bool move) {
    bool wasCut = false;

    QDataStream dataStream(&itemData, QIODevice::ReadOnly);

    int numCopyGroups;

    dataStream >> numCopyGroups;

    int originRow = INVALID;
    int originColumn = INVALID;

    int groupNumber = dropGroupNumber;
    int fromGroupNumber;
    int lastFromGroupNumber = INVALID;
    int groupNumberGap;

    QList<int> pasteGroups;

    for(int i = 0; i < numCopyGroups && groupNumber <= iHighestGroupNumber; i++) {
        dataStream >> fromGroupNumber;

        if(lastFromGroupNumber == INVALID) {
            lastFromGroupNumber = fromGroupNumber;
        }

        groupNumberGap = fromGroupNumber - lastFromGroupNumber;

        groupNumber+=groupNumberGap;

        while(!iGroups.contains(groupNumber)) {
            groupNumber++;
        }

        wasCut = iGroups.value(groupNumber)->doHandleMimeData(dataStream,
                                                                fromGroupNumber,
                                                                dropPosition,
                                                                &originRow,
                                                                &originColumn,
                                                                wrap,
                                                                move);
        pasteGroups.append(groupNumber);

        lastFromGroupNumber = fromGroupNumber;
    }

    groupNumber = 0;

   for(int i = 0; i < iGroups.count() && groupNumber <= iHighestGroupNumber; i++) {
        while(!iGroups.contains(groupNumber) ||
              pasteGroups.contains(groupNumber)) {
            groupNumber++;

            if(groupNumber > iHighestGroupNumber) {
                break;
            }
        }

        iGroups.value(groupNumber)->clearAllSelection();
        selectGroup(groupNumber, false);
        groupNumber++;
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
