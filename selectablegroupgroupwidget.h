#ifndef SELECTABLEGROUPOFGROUPSWIDGET_H
#define SELECTABLEGROUPOFGROUPSWIDGET_H

#include <QWidget>

namespace AnimatorUi {

class SelectableGroupWidget;

class SelectableGroupGroupWidget : public QWidget {
    Q_OBJECT

public:
    explicit SelectableGroupGroupWidget(QWidget *parent);

    void selected(SelectableGroupWidget& selectedWidget);
    int addGroup(SelectableGroupWidget& newGroup);

    inline SelectableGroupWidget& group(int groupNumber) { return *iColourGroups.at(groupNumber); }

private:
    QList<SelectableGroupWidget*>   iColourGroups;
    
};
}

#endif // SELECTABLEGROUPOFGROUPSWIDGET_H
