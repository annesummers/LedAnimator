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
                      int speed,
                      int priority,
                      bool isOpaque);

    void setSpeed(const int speed);
    void setLowValue(const int lowValue);
    void setHighValue(const int highValue);

    inline int speed() const { return iSpeed; }

    inline bool usesBackgroundColour() const { return iBackgroundColour.isValid(); }
    inline const QColor backgroundColour() const { return iBackgroundColour; }
    inline void setBackgroundColour(QColor backgroundColour) { iBackgroundColour = backgroundColour; }

    inline void setRepeating(bool repeat) { iRepeat = repeat; }
    inline bool isRepeating() { return iRepeat; }

    void play(bool repeat);
    void stop();

    inline bool isPlaying() const { return iIsPlaying; }

signals:
    void stopped();

public slots:
    void nextFrame();

private:
    const int iSpeed;
    QColor iBackgroundColour;

    bool iIsPlaying;
    bool iRepeat;

    QTimer* iPlayTimer;
};

}


#endif // TIMEAXIS_H
