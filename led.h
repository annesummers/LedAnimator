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

#include "defaults.h"
#include "frame.h"

class Frame;

class Led : public QObject {
    Q_OBJECT

public:
    explicit Led(QObject* parent, const Animation& animation, int row, int column);

    inline const QColor currentColour() const { return frameAt(iAnimation.currentFrame()).colour(); }

    inline const bool isSelected() const { return iIsSelected; }

    inline const int row() const { return iRow; }
    inline const int column() const {return iColumn; }

    Frame& frameAt(int frameNum) const;

    void setCurrentColour(QColor colour);

    void select(bool isSelected);

signals:
    void selected();
    void currentColourChanged();

public slots:
    void numFramesChanged(int numFrames);
    void colourChanged(int frameNum);

private:
    const Animation& iAnimation;
    QList<Frame*> iFrames;
    QSignalMapper* iSignalMapper;

    int     iRow;
    int     iColumn;
    bool    iIsSelected;
};

#endif // LED_H
