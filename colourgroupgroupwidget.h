#ifndef SELECTABLEGROUPOFGROUPSWIDGET_H
#define SELECTABLEGROUPOFGROUPSWIDGET_H

#include <QWidget>

namespace AnimatorUi {

class SelectableGroupWidget;

class ColourGroupGroupWidget : public QWidget {
    Q_OBJECT

public:
    explicit ColourGroupGroupWidget(QWidget *parent);

    void selected(SelectableGroupWidget& selectedWidget);
    inline void addGroup(SelectableGroupWidget& newGroup) { iColourGroups.append(&newGroup); }

private:
    QList<SelectableGroupWidget*>   iColourGroups;
    
};
}

#endif // SELECTABLEGROUPOFGROUPSWIDGET_H
