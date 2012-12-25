/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef SELECTABLEGROUPWIDGET_H
#define SELECTABLEGROUPWIDGET_H

#include <QWidget>

class Animation;
class Selectable;

namespace Ui {

class SelectableGroupWidget : public QWidget {
    Q_OBJECT

public:
    explicit SelectableGroupWidget(QWidget *parent);

    void clearSelection();
    void selectOne(Selectable& selectable);
    void select(Selectable& led, bool select);

    const QList<Selectable*>& selectedItems() { return iSelected; }

private:
    QList<Selectable*> iSelected;
};
}

#endif // SELECTABLEGROUPWIDGET_H
