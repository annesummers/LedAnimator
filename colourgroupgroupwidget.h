#ifndef SELECTABLEGROUPOFGROUPSWIDGET_H
#define SELECTABLEGROUPOFGROUPSWIDGET_H

#include <QWidget>

class ColourGroupWidget;

namespace Ui {

class ColourGroupGroupWidget : public QWidget {
    Q_OBJECT

public:
    explicit ColourGroupGroupWidget(QWidget *parent, int numGroups);
    
protected:
    void addGroup(ColourGroupWidget& newGroup) { iColourGroups.append(&newGroup); }

private:
    QList<ColourGroupWidget*>   iColourGroups;
    
};
}

#endif // SELECTABLEGROUPOFGROUPSWIDGET_H
