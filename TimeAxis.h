#ifndef TIMEAXIS_H
#define TIMEAXIS_H

#include <QObject>
#include <QTimer>

#include "Axis.h"

namespace AnimatorModel {

class TimeAxis : public Axis {
    Q_OBJECT

public:
    explicit TimeAxis(QObject *parent,
                      Animation &animation,
                      int lowValue,
                      int highValue,
                      int speed);

    void setSpeed(const int speed);
    void setLowValue(const int lowValue);
    void setHighValue(const int highValue);

    inline const int speed() const { return iSpeed; }

    inline void setRepeating(bool repeat) { iRepeat = repeat; }
    inline bool isRepeating() { return iRepeat; }

    void play(bool repeat);
    void stop();

    inline const bool isPlaying() const { return iIsPlaying; }
    inline void setPlaying(bool isPlaying) { iIsPlaying = isPlaying; }

signals:
    void stopped();

public slots:
    void nextFrame();

private:
    const int iSpeed;

    bool iIsPlaying;
    bool iRepeat;

    QTimer* iPlayTimer;
};

class TimeAxisData : public AxisData {
    Q_OBJECT

public:
    explicit TimeAxisData(QObject *parent,
                          Animation& animation,
                            TimeAxis& axis,
                          QUndoStack& undoStack);

    void copyFrames(const AxisData &copyAxis);

    virtual void lowValueChanged(const int lowValue);
    virtual void highValueChanged(const int highValue);
    virtual void zeroValueChanged(const int zeroValue);
};

}


#endif // TIMEAXIS_H
