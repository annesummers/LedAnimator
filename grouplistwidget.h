#ifndef GROUPLISTWIDGET_H
#define GROUPLISTWIDGET_H

#include <QWidget>
#include <QComboBox>

#include "animation.h"

class GroupListWidget : public QWidget {
    Q_OBJECT
    
public:
    explicit GroupListWidget(QWidget *parent, Animation &animation);

private slots:
    void addGroup(int groupNumber);
    void removeGroup(int groupNumber);

    void comboChanged(int index);
    
private:
    Animation& iAnimation;
    QComboBox* iComboBox;
};

#endif // GROUPLISTWIDGET_H
