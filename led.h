/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef LED_H
#define LED_H

#include <QObject>
#include <QColor>
#include <QPoint>
#include <QSignalMapper>

#include "animation.h"
#include "frame.h"
#include "selectable.h"

#include "defaults.h"

class Animation;

class Led : public Selectable {
    Q_OBJECT

public:
    explicit Led(QObject* parent, Animation& animation, int row, int column);

    inline const QColor currentColour() const { return frameAt(iAnimation.currentFrame()).colour(); }

    inline const int row() const { return iRow; }
    inline const int column() const {return iColumn; }

    Frame& frameAt(int frameNum) const;

    void setCurrentColour(QColor colour);

signals:
    void currentColourChanged();

public slots:
    void numFramesChanged(int numFrames);
    void colourChanged(int frameNum);

private:
    int     iRow;
    int     iColumn;

    Animation& iAnimation;

    QList<Frame*> iFrames;
    QSignalMapper* iSignalMapper;
};

#endif // LED_H
