/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef LED_H
#define LED_H

#include <QObject>
#include <QColor>

#include "Frame.h"
#include "griditem.h"
#include "Axis.h"
#include "TimeAxis.h"
#include "ValueAxis.h"
#include "constants.h"

namespace AnimatorModel {

class Animation;

class Led : public GridItem {
    Q_OBJECT

public:
    explicit Led(QObject* parent, Animation& animation, int number, Position position, QUndoStack& undoStack);
    Led(const Led &copyLed);
    virtual ~Led();

    const QColor currentColour() const;

    inline TimeAxisData* timeAxis() const { return iTimeAxisData; }
    ValueAxisData& axisAt(int axisNum) const;

    void setCurrentValue(FrameValue& value);

    inline void setNumber(int newNumber) { Selectable::setNumber(newNumber); emit ledUpdated(); }

    inline void setHidden(bool isHidden) { iHidden = isHidden; }
    inline bool isHidden() const { return iHidden; }

    void move(Position newPosition);
    void paste(Led &copyLed);

    void copyAxes(const Led& copyLed);
    void copyTimeAxis(const Led& copyLed);
    void copyCurrentFrame(const Led& copyLed, int frameNum);
    void copyValueAxis(const Led& copyLed, int axisNum);

    inline Animation& animation() { return iAnimation; }

    Led& operator=(const Led& led);

signals:
    void ledUpdated();
    void updated();

public slots:
    void addTimeAxis();
    void addValueAxis(int axisNumber);
    void currentFrameChanged(int currentFrame);
   // void framesInserted(int numFrames, int numFramesAdded);
    void colourChanged(int frameNum);

private:

    QList<ValueAxisData*> iAxesData;
    TimeAxisData* iTimeAxisData;

    QUndoStack& iUndoStack;

    bool iHidden;
};
}
#endif // LED_H
