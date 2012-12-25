/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef SELECTABLE_H
#define SELECTABLE_H

#include <QObject>

class Selectable : public QObject
{
    Q_OBJECT
public:
    explicit Selectable(QObject *parent = 0);

    inline const bool isSelected() const { return iIsSelected; }

    void select(bool isSelected);

signals:
    void selected();
    
public slots:

private:
    bool    iIsSelected;
    
};

#endif // SELECTABLE_H
