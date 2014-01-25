#ifndef AXISDATA_H
#define AXISDATA_H

#include <QObject>
#include <QUndoStack>
#include <QMap>
#include <QSignalMapper>

#include "Range.h"
#include "Function.h"
#include "Axis.h"

namespace AnimatorModel {

class Animation;
class Led;
class Frame;

class AxisData : public QObject {
    Q_OBJECT

public:
    explicit AxisData(QObject *parent,
                      Animation& animation,
                      Axis& axis,
                      Led& led,
                      QUndoStack *undoStack);

    inline Axis& axis() const { return iAxis; }

    inline Frame& frameAt(int frameNum) const { return *iFrames.find(frameNum).value(); }
    inline int currentFrameNum() const { return iAxis.currentFrameNum(); }

    inline const Frame& currentFrame() const { return frameAt(iAxis.currentFrameNum()); }

    void setFirstInRange(int number);
    void setLastInRange(int number, Function function);
    void setAnchorInRange(int number);

    void setRange(int first, int last, int anchor, Function function);
    void calculateRanges();

    Range rangeAt(int index) { return iFunctionRanges.at(index); }
    inline int numRanges() { return iFunctionRanges.count(); }

    virtual void copyFrames(const AxisData &copyAxis);

    virtual void lowValueChanged(const int lowValue) = 0;
    virtual void highValueChanged(const int highValue) = 0;
    virtual void zeroValueChanged(const int zeroValue) = 0;

protected:
    Axis& iAxis;

    QMap<int, Frame*> iFrames;

    Animation& iAnimation;

    QUndoStack *iUndoStack;
    QSignalMapper* iSignalMapper;

    QList<Range> iFunctionRanges;

    int iNewAnchor;
    int iNewFirst;
    bool iHasNewAnchor;
};

}

#endif // AXISDATA_H
