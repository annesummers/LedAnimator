#ifndef AXIS_H
#define AXIS_H

#include <QObject>
#include <QMap>

#include "Frame.h"

namespace AnimatorModel {

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
    inline const int currentFrame() const { return iCurrentFrame; }
    void setCurrentFrame(int frame);

signals:
    void currentFrameChanged(int currentFrame);
    void lowValueChanged(const int lowValue);
    void highValueChanged(const int highValue);

public slots:

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
                      QUndoStack& undoStack);

    inline Axis& axis() const { return iAxis; }

    inline Frame& frameAt(int frameNum) const { return *iFrames.find(frameNum).value(); }
    inline const int numFrames() const { return iAxis.numFrames(); }
    inline const int currentFrame() const { return iAxis.currentFrame(); }

    virtual void copyFrames(const AxisData &copyAxis) = 0;

    virtual void lowValueChanged(const int lowValue) = 0;
    virtual void highValueChanged(const int highValue) = 0;
    virtual void zeroValueChanged(const int zeroValue) = 0;

protected:
    Axis& iAxis;

    QMap<int, Frame*> iFrames;

    Animation& iAnimation;

    QUndoStack &iUndoStack;
};

}


#endif // AXIS_H
