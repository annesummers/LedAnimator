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
    explicit Selectable(QObject *parent, int number);

    inline const bool isSelected() const { return iIsSelected; }
    void select(bool isSelected);

    inline const int number() const { return iNumber; }

protected:
    inline void setNumber(int newNumber) { iNumber = newNumber; }

signals:
    void selected();

private:
    bool    iIsSelected;
    int     iNumber;
};
}

#endif // SELECTABLE_H
