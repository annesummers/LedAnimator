/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef SELECTABLE_H
#define SELECTABLE_H

#include <QObject>

namespace AnimatorModel {

class Selectable : public QObject {
    Q_OBJECT

public:
    explicit Selectable(QObject *parent);

    inline const bool isSelected() const { return iIsSelected; }
    void select(bool isSelected);

signals:
    void selected();

private:
    bool    iIsSelected;   
};
}

#endif // SELECTABLE_H
