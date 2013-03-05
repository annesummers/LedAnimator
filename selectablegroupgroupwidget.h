#ifndef SELECTABLEGROUPOFGROUPSWIDGET_H
#define SELECTABLEGROUPOFGROUPSWIDGET_H

#include <QWidget>
#include <QHash>

namespace AnimatorUi {

class SelectableGroupWidget;

class SelectableGroupGroupWidget : public QWidget {
    Q_OBJECT

public:
    explicit SelectableGroupGroupWidget(QWidget *parent);

    void selected(SelectableGroupWidget& selectedWidget, bool isSelected, bool selectSingle = false);

    int addGroup(SelectableGroupWidget& newGroup);
    void removeGroup(SelectableGroupWidget& group);

    void selectSingle(SelectableGroupWidget& selectedWidget);

    const QByteArray writeMimeData(bool cut);
    bool handleMimeData(QByteArray itemData, int dropGroupNumber, int dropRow, int dropColumn, bool wrap, bool move);
   // void handleOldMimeData(QByteArray itemData);

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
