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
#include "griditem.h"

#include "constants.h"

namespace AnimatorModel {

class Led : public GridItem {
    Q_OBJECT

public:
    explicit Led(QObject* parent, Animation& animation, int number, int row, int column);
    virtual ~Led();

    inline const QColor currentColour() const { return frameAt(iAnimation.currentFrame()).colour(); }

    Frame& frameAt(int frameNum) const;

    void setCurrentColour(QColor colour);

    inline int number() { return iNumber; }
    inline void setNumber(int newNumber) { iNumber = newNumber; emit ledUpdated(); }

    inline void setHidden(bool isHidden) { iHidden = isHidden; }
    inline bool isHidden() { return iHidden; }

    void move(int newRow, int newColumn);
    void paste(Led &copyLed);

    void copyFrames(Led& copyLed);

signals:
    void ledUpdated();

public slots:
    void numFramesChanged(int numFrames);
    void colourChanged(int frameNum);

private:
    Animation& iAnimation;

    QList<Frame*> iFrames;
    QSignalMapper* iSignalMapper;

    int iNumber;
    int iGroupNumber;

    bool iHidden;
};
}
#endif // LED_H
