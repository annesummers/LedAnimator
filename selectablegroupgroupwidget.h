/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef SELECTABLEGROUPOFGROUPSWIDGET_H
#define SELECTABLEGROUPOFGROUPSWIDGET_H

#include <QWidget>
#include <QHash>

#include "constants.h"

namespace AnimatorUi {

class SelectableGroupWidget;

class SelectableGroupGroupWidget : public QWidget {
    Q_OBJECT

public:
    explicit SelectableGroupGroupWidget(QWidget *parent);

    void selectGroup(int groupNumber, bool isSelected, bool selectSingleGroup = false);

    int addGroup(SelectableGroupWidget& newGroup);
    void removeGroup(SelectableGroupWidget& group);

    void selectSingleGroup(SelectableGroupWidget& selectedWidget);

    void selectArea(int lastGroupNumber, Position end, bool multipleAreas);

    bool isOtherSelected(int groupNumber);

    const QByteArray writeMimeData(bool cut);
    bool handleMimeData(QByteArray itemData, int dropGroupNumber, Position dropPosition, bool wrap, bool move);

    inline SelectableGroupWidget& group(int groupNumber) { return *iGroups.value(groupNumber); }

protected:
    QHash<int, SelectableGroupWidget*>  iSelectedGroups;

private:
    QHash<int, SelectableGroupWidget*>  iGroups;

    QList<int> iFreeGroupNumbers;
    int iHighestGroupNumber;
};
}

#endif // SELECTABLEGROUPOFGROUPSWIDGET_H
