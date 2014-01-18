/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef SELECTABLEGROUPOFGROUPSWIDGET_H
#define SELECTABLEGROUPOFGROUPSWIDGET_H

#include <QWidget>
#include <QHash>
#include <QMimeData>

#include "Position.h"

class Engine;

namespace AnimatorUi {

class SelectableGroupWidget;
class GroupSetIterator;
class MainWindow;

class GroupSet : public QHash<int, SelectableGroupWidget*> {
public:
    GroupSet();
    int addNumberedGroup(SelectableGroupWidget &group);
    void removeNumberedGroup(int groupNumber);

    GroupSetIterator& iterator();
private:
    GroupSetIterator* iIterator;

    int iHighestNumber;
    QList<int> iFreeNumbers;

    friend class GroupSetIterator;
};

class GroupSetIterator : public QObject {
    Q_OBJECT

public:
    GroupSetIterator (const GroupSet *set);
    bool findNext (const int group);
    bool findPrevious (const int group);
    bool hasNext () const;
    bool hasPrevious () const;
    SelectableGroupWidget& next ();
    const SelectableGroupWidget& peekNext () const;
    const SelectableGroupWidget& peekPrevious () const;
    SelectableGroupWidget& previous ();
    void toBack ();
    void toFront ();
private:
    GroupSetIterator &	operator= (const GroupSetIterator &set);
    const GroupSet& iGroupSet;
    int iNextCounter;
    int iPreviousCounter;
};

class SelectableGroupGroupWidget : public QWidget {
    Q_OBJECT

public:
    explicit SelectableGroupGroupWidget(QWidget *parent, Engine& engine);

    void selectGroup(int groupNumber, bool isSelected, bool selectSingleGroup = false);

    int addGroup(SelectableGroupWidget& newGroup);
    void removeGroup(SelectableGroupWidget& group);

    void selectSingleGroup(SelectableGroupWidget& selectedWidget);

    void selectArea(int startGroupNumber, Position end, bool multipleAreas);

    bool isOtherSelected(int groupNumber);
    bool isAnySelected();

    bool isGroupGroupSelected();

    int lastSelectedGroupNumber();

    virtual bool canCut() const = 0;

    const QByteArray writeMimeData(bool cut);
    bool handleMimeData(QByteArray itemData,
                        int dropGroupNumber, Position dropPosition,
                        bool wrap, bool move);

    virtual QString mimeType() const = 0;
    QMimeData* mimeData(bool cut);

    inline SelectableGroupWidget& group(int groupNumber) { return *iGroups.value(groupNumber); }

public slots:
    void cutSelected();
    void copySelected();
    void paste();
    void pasteWrap();

protected:
    QHash<int, SelectableGroupWidget*>  iSelectedGroups;

private:
    void clearClipboard();

    Engine& iEngine;

    GroupSet  iGroups;
    QList<int>  iLastSelectedGroups;
};
}

#endif // SELECTABLEGROUPOFGROUPSWIDGET_H
