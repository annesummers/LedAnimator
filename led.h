/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef LED_H
#define LED_H

#include <QObject>
#include <QColor>
#include <QSignalMapper>

#include "animation.h"
#include "frame.h"
#include "griditem.h"

#include "constants.h"

namespace AnimatorModel {

class Led : public GridItem {
    Q_OBJECT

public:
    explicit Led(QObject* parent, Animation& animation, int number, Position position);
    Led(const Led &copyLed);
    virtual ~Led();

    inline const QColor currentColour() const { return frameAt(iAnimation.currentFrame()).colour(); }

    Frame& frameAt(int frameNum) const;

    void setCurrentColour(QColor colour);

    inline void setNumber(int newNumber) { Selectable::setNumber(newNumber); emit ledUpdated(); }

    inline const int groupNumber() const { return iGroupNumber; }
    void setGroupNumber(int groupNumber);

    inline void setHidden(bool isHidden) { iHidden = isHidden; }
    inline bool isHidden() const { return iHidden; }

    void move(Position newPosition);
    void paste(Led &copyLed);

    void copyFrames(const Led& copyLed);

    inline Animation& animation() { return iAnimation; }

    Led& operator=(const Led& led);

signals:
    void ledUpdated();

public slots:
    void numFramesChanged(int numFrames);
    void colourChanged(int frameNum);

private:
    QList<Frame*> iFrames;
    QSignalMapper* iSignalMapper;

    int iGroupNumber;

    bool iHidden;
};
}
#endif // LED_H
