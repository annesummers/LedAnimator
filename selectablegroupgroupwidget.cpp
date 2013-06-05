/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "selectablegroupgroupwidget.h"

#include <QApplication>
#include <QClipboard>

#include "selectablegroupwidget.h"

#include "constants.h"
#include "exceptions.h"

using namespace AnimatorUi;
using namespace Exception;

GroupSet::GroupSet() :
    iIterator(NULL),
    iHighestNumber(INVALID) {
}

GroupSetIterator& GroupSet::iterator() {
    if(iIterator != NULL) {
        delete iIterator;
    }
    iIterator = new GroupSetIterator(this);
    return *iIterator;
}

int GroupSet::addNumberedGroup(SelectableGroupWidget &group)  {
    int groupNumber;
    if(iFreeNumbers.count() > 0) {
        groupNumber = iFreeNumbers.takeFirst();
    } else {
        groupNumber = ++iHighestNumber;
    }

    insert(groupNumber, &group);

    return groupNumber;
}

void GroupSet::removeNumberedGroup(int groupNumber) {
    remove(groupNumber);

    if(groupNumber == iHighestNumber) {
        iHighestNumber--;
    } else {
        iFreeNumbers.append(groupNumber);
    }
}

GroupSetIterator::GroupSetIterator (const GroupSet* set) :
    QObject(NULL),
    iGroupSet(*set),
    iNextCounter(0),
    iPreviousCounter(-1) {

}

bool GroupSetIterator::findNext (const int group) {
    while(hasNext()) {
        if(next().groupNumber() == group) {
            return true;
        }
    }

    return false;
}

bool GroupSetIterator::findPrevious (const int group) {
    while(hasPrevious()) {
        if(previous().groupNumber() == group) {
            return true;
        }
    }

    return false;
}

bool GroupSetIterator::hasNext () const {
    if(iNextCounter > iGroupSet.iHighestNumber ||
       iNextCounter  > iGroupSet.count()) {
        return false;
    }

    return true;
}

bool GroupSetIterator::hasPrevious () const {
    if(iPreviousCounter < 0) {
        return false;
    }

    return true;
}

SelectableGroupWidget& GroupSetIterator::next () {
    while(!iGroupSet.contains(iNextCounter)  && iNextCounter < iGroupSet.iHighestNumber) {
        iNextCounter++;
    }

    if(iNextCounter > iGroupSet.iHighestNumber) {
        throw IllegalStateException("GroupSetIterator::next() : no next item");
    }

    iPreviousCounter = iNextCounter;
    return *iGroupSet.value(iNextCounter++);
}

const SelectableGroupWidget& GroupSetIterator::peekNext () const {
    int counter = iNextCounter;
    while(!iGroupSet.contains(counter) && counter < iGroupSet.iHighestNumber) {
        counter++;
    }

    return *iGroupSet.value(counter);
}

const SelectableGroupWidget& GroupSetIterator::peekPrevious () const {
    int counter = iPreviousCounter;
    while(!iGroupSet.contains(counter) && iPreviousCounter > INVALID) {
        counter--;
    }

    return *iGroupSet.value(counter);
}

SelectableGroupWidget& GroupSetIterator::previous () {
    while(!iGroupSet.contains(iPreviousCounter && iPreviousCounter > INVALID)) {
        iPreviousCounter--;
    }

    if(iPreviousCounter < 0) {
        throw IllegalStateException("GroupSetIterator::previous() : no previous item");
    }

    iNextCounter = iPreviousCounter;
    return *iGroupSet.value(iPreviousCounter--);
}

void GroupSetIterator::toBack() {
    iNextCounter = INVALID;
    iPreviousCounter = iGroupSet.iHighestNumber;
}

void GroupSetIterator::toFront() {
    iNextCounter = 0;
    iPreviousCounter = INVALID;
}

GroupSetIterator& GroupSetIterator::operator= (const GroupSetIterator &set) {

}

SelectableGroupGroupWidget::SelectableGroupGroupWidget(QWidget *parent) :
    QWidget(parent) {
}

void SelectableGroupGroupWidget::selectGroup(int groupNumber, bool isSelected, bool singleSelect) {
    if(!isSelected) {
        iSelectedGroups.remove(groupNumber);
        iLastSelectedGroups.removeAt(iLastSelectedGroups.lastIndexOf(groupNumber));
    } else {
        if(singleSelect) {
            selectSingleGroup(*iGroups.value(groupNumber));
        } else {
            if(!iSelectedGroups.contains(groupNumber)) {
                iSelectedGroups.insert(groupNumber, iGroups.value(groupNumber));
            }

            iLastSelectedGroups.append(groupNumber);
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

    iLastSelectedGroups.clear();
    iLastSelectedGroups.append(selectedWidget.groupNumber());
}

int SelectableGroupGroupWidget::addGroup(SelectableGroupWidget& newGroup) {
    return iGroups.addNumberedGroup(newGroup);
}

void SelectableGroupGroupWidget::removeGroup(SelectableGroupWidget& group) {
    iGroups.removeNumberedGroup(group.groupNumber());
}

void SelectableGroupGroupWidget::selectArea(int startGroupNumber, Position end, bool multipleAreas) {
    int firstGroupNumber;
    int endGroupNumber = INVALID;
    Position start;

    GroupSetIterator& iterator = iGroups.iterator();

   /* while(iterator.hasNext()) {
        const SelectableGroupWidget& group = iterator.next();

        if(endGroupNumber == INVALID && group.isAnySelected()) {
            start = group.lastSelected();
            endGroupNumber = group.groupNumber();
            break;
        }
    }*/

    iterator.findNext(iLastSelectedGroups.last());
    iterator.previous();

    const SelectableGroupWidget& group = iterator.next();
    start = group.lastSelected();
    endGroupNumber = group.groupNumber();

    int numGroups;
    if(endGroupNumber < startGroupNumber) {
        firstGroupNumber = endGroupNumber;
        numGroups = startGroupNumber - endGroupNumber + 1;
    } else {
        firstGroupNumber = startGroupNumber;
        numGroups = endGroupNumber - startGroupNumber + 1;
    }

    int counter = 0;

    iterator.toFront();

    while(iterator.hasNext()) {
        SelectableGroupWidget& group = iterator.next();
        if(group.groupNumber() == firstGroupNumber) {
            break;
        }

        if(!multipleAreas) {
            group.clearAllSelection();
        }
    }

    iterator.previous();

    while(iterator.hasNext() && counter++ < numGroups) {
        iterator.next().doSelectArea(start, end, multipleAreas);
    }

    if(!multipleAreas) {
        while(iterator.hasNext()) {
            iterator.next().clearAllSelection();
        }
    }
}

bool SelectableGroupGroupWidget::isOtherSelected(int groupNumber) {
    SelectableGroupWidget* group;
    foreach(group, iSelectedGroups) {
        if(group->groupNumber() != groupNumber) {
            return true;
        }
    }

    return false;
}

bool SelectableGroupGroupWidget::isAnySelected() {
    return iSelectedGroups.count() > 0;
}

int SelectableGroupGroupWidget::lastSelectedGroupNumber() {
    return iLastSelectedGroups.last();
}

void SelectableGroupGroupWidget::cutSelected() {
    if(hasFocus()) {
        clearClipboard();

        QApplication::clipboard()->setMimeData(mimeData(true));
    }
}

void SelectableGroupGroupWidget::copySelected() {
    if(hasFocus()) {
        clearClipboard();

        QApplication::clipboard()->setMimeData(mimeData(false));
    }
}

void SelectableGroupGroupWidget::paste() {
    if(hasFocus()) {
        iSelectedGroups.value(lastSelectedGroupNumber())->paste(false);
    }
}

void SelectableGroupGroupWidget::pasteWrap() {
    if(hasFocus()) {
        iSelectedGroups.value(lastSelectedGroupNumber())->paste(true);
    }
}

void SelectableGroupGroupWidget::clearClipboard() {
    const QClipboard *clipboard = QApplication::clipboard();

    if(clipboard->mimeData()->hasFormat(mimeType())) {
        QApplication::clipboard()->clear();
    }
}

QMimeData* SelectableGroupGroupWidget::mimeData(bool cut) {
    QMimeData *data = new QMimeData;
    data->setData(mimeType(), writeMimeData(cut));

    return data;
}

const QByteArray SelectableGroupGroupWidget::writeMimeData(bool cut) {
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    dataStream << iSelectedGroups.count();

    qDebug("Selected group count is %d", iSelectedGroups.count());

    SelectableGroupWidget* group;
    foreach(group, iSelectedGroups) {
        group->doWriteMimeData(dataStream, cut);
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

    int counter = 0;

    GroupSetIterator& iterator = iGroups.iterator();

    iterator.findNext(dropGroupNumber);
    iterator.previous();

    while(iterator.hasNext() && counter++ < numCopyGroups) {
        dataStream >> fromGroupNumber;

        if(lastFromGroupNumber == INVALID) {
            lastFromGroupNumber = fromGroupNumber;
        }

        groupNumberGap = fromGroupNumber - lastFromGroupNumber;

        groupNumber+=groupNumberGap;

        wasCut = iterator.next().doHandleMimeData(dataStream,
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

    iterator.toFront();

    while(iterator.hasNext()) {
        while(iterator.hasNext() &&pasteGroups.contains(iterator.peekNext().groupNumber())) {
            iterator.next();
        }

        if(iterator.hasNext()) {
            SelectableGroupWidget& group = iterator.next();
            group.clearAllSelection();
            selectGroup(group.groupNumber(), false);
        }
    }

    return wasCut;
}
