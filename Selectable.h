/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef SELECTABLE_H
#define SELECTABLE_H

#include <QObject>

namespace AnimatorModel {

class Animation;

class Selectable : public QObject {
    Q_OBJECT

public:
    explicit Selectable(QObject *parent, Animation& animation, int number);

    inline bool isSelected() const { return iIsSelected; }
    void select(bool isSelected);

    inline int number() const { return iNumber; }

protected:
    inline void setNumber(int newNumber) { iNumber = newNumber; }

    Animation& iAnimation;

signals:
    void selected();

private:
    bool    iIsSelected;
    int     iNumber;
};
}

#endif // SELECTABLE_H
