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
                  int zeroValue);

    inline const int lowValue() const { return iLowValue; }
    inline const int highValue() const { return iHighValue; }
    inline const int zeroValue() const { return iZeroValue; }

    //inline const void setLowValue(const int lowValue) { iLowValue = lowValue; }
   // inline const void setHighValue(const int highValue) { iHighValue = highValue; }
   // inline const void setZeroValue(const int zeroValue) { iZeroValue = zeroValue; }

    const int numFrames() const;
    inline const int currentFrameNum() const { return iCurrentFrame; }

    //const Frame& currentFrame() const;

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
   // inline const int numFrames() const { return iAxis.numFrames(); }
    inline const int currentFrameNum() const { return iAxis.currentFrameNum(); }

    inline const Frame& currentFrame() const { return frameAt(iAxis.currentFrameNum()); }

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
};

}


#endif // AXIS_H
