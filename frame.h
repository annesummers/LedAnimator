/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef FRAME_H
#define FRAME_H

#include <QObject>
#include <QColor>

#include "selectable.h"

namespace AnimatorModel {

class Frame : public Selectable {
    Q_OBJECT

public:
    explicit Frame(QObject *parent, int number);
    Frame(QObject* parent, QColor colour, int number);
    
    const QColor colour() const;
    void setColour(QColor colour);

signals:
    void colourChanged();

private:
    QColor iColour;
};
}

#endif // FRAME_H
