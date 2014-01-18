#ifndef AXIS_H
#define AXIS_H

#include <QObject>
#include <QMap>
#include <QSignalMapper>

#include "Frame.h"

namespace AnimatorModel {

class Led;

class Axis : public QObject {
    Q_OBJECT

public:
    enum AxisType {
        ETime,
        EValue
    };

    explicit Axis(QObject *parent,
                  Animation &animation,
                  int lowValue,
                  int highValue,
                  int zeroValue,
                  int priority,
                  bool isOpaque);

    inline int lowValue() const { return iLowValue; }
    inline int highValue() const { return iHighValue; }
    inline int zeroValue() const { return iZeroValue; }

    //inline const void setLowValue(const int lowValue) { iLowValue = lowValue; }
   // inline const void setHighValue(const int highValue) { iHighValue = highValue; }
   // inline const void setZeroValue(const int zeroValue) { iZeroValue = zeroValue; }

    int numFrames() const;
    inline int currentFrameNum() const { return iCurrentFrame; }

    //const Frame& currentFrame() const;

    inline bool isOpaque() const { return iOpaque; }
    inline int priority() const { return iPriority; }

signals:
    void currentFrameChanged(int currentFrameNum);
    void lowValueChanged(const int lowValue);
    void highValueChanged(const int highValue);

public slots:
    void setCurrentFrame(int frame);

protected:
    int iLowValue;
    int iHighValue;

    int iZeroValue;

private:
    Animation& iAnimation;

    int iCurrentFrame;

    const QList<Axis*> iSubAxes;

    bool iOpaque;
    int iPriority;

};


class Range {
public:
    inline Range(int anchorValue,
                 int startValue,
                 int endValue,
                 int function) :
        iRange(startValue, endValue),
        iFunction(function),
        iAnchorValue(anchorValue){}

    inline Range(const Range& range) :
        iRange(range.iRange),
        iFunction(range.iFunction),
        iAnchorValue (range.iAnchorValue){}

    inline Range& operator=(const Range& range) {
        iRange = range.iRange;
        iFunction = range.iFunction;
        iAnchorValue = range.iAnchorValue;

        return *this;
    }

    inline int lowValue() const { return iRange.first; }
    inline int highValue() const { return iRange.second; }

    inline void setFunction(int function) { iFunction = function; }
    inline int function() const { return iFunction; }

    inline void setAnchor(int anchor) { iAnchorValue = anchor; }
    inline int anchor() const { return iAnchorValue; }

private:
    QPair<int, int>  iRange;
    int iFunction;
    int iAnchorValue;
};

class AxisData : public QObject {
    Q_OBJECT

public:
    explicit AxisData(QObject *parent,
                      Animation& animation,
                      Axis& axis,
                      Led& led,
                      QUndoStack& undoStack);

    inline Axis& axis() const { return iAxis; }

    inline Frame& frameAt(int frameNum) const { return *iFrames.find(frameNum).value(); }
    inline int currentFrameNum() const { return iAxis.currentFrameNum(); }

    inline const Frame& currentFrame() const { return frameAt(iAxis.currentFrameNum()); }

    void setFirstInRange(int number);
    void setLastInRange(int number, Function function);
    void setAnchorInRange(int number);

    virtual void copyFrames(const AxisData &copyAxis);

    virtual void lowValueChanged(const int lowValue) = 0;
    virtual void highValueChanged(const int highValue) = 0;
    virtual void zeroValueChanged(const int zeroValue) = 0;

protected:
    Axis& iAxis;

    QMap<int, Frame*> iFrames;

    Animation& iAnimation;

    QUndoStack &iUndoStack;
    QSignalMapper* iSignalMapper;

    QList<Range> iFunctionRanges;

    int iNewAnchor;
    int iNewFirst;
    bool iHasNewAnchor;
};

}


#endif // AXIS_H
