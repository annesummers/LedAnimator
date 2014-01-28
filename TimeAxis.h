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
                      int frameRate,
                      int priority,
                      bool isOpaque);

    void setframeRate(const int frequency);

    inline int frameRate() const { return iFrameRate; }

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
    int iFrameRate;
    QColor iBackgroundColour;

    bool iIsPlaying;
    bool iRepeat;

    QTimer* iPlayTimer;
};

}


#endif // TIMEAXIS_H
