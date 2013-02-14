#ifndef SELECTABLEGROUPOFGROUPSWIDGET_H
#define SELECTABLEGROUPOFGROUPSWIDGET_H

#include <QWidget>

namespace AnimatorUi {

class ColourGroupWidget;

class ColourGroupGroupWidget : public QWidget {
    Q_OBJECT

public:
    explicit ColourGroupGroupWidget(QWidget *parent);

    void selected(ColourGroupWidget& selectedWidget);
    inline void addGroup(ColourGroupWidget& newGroup) { iColourGroups.append(&newGroup); }

private:
    QList<ColourGroupWidget*>   iColourGroups;
    
};
}

#endif // SELECTABLEGROUPOFGROUPSWIDGET_H
