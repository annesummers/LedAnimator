#ifndef AXIS_H
#define AXIS_H

#include <QObject>

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

    void setLowValue(const int lowValue);
    void setHighValue(const int highValue);

    int numFrames() const;
    inline int currentFrameNum() const { return iCurrentFrame; }

    //const Frame& currentFrame() const;

    inline bool isOpaque() const { return iOpaque; }
    inline int priority() const { return iPriority; }

signals:
    void currentFrameChanged(int currentFrameNum);
    void lowValueChanged(const int oldLowValue, const int lowValue);
    void highValueChanged(const int oldHighValue, const int highValue);

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

}


#endif // AXIS_H
