#include "TimeAxis.h"

#include "exceptions.h"
#include "ColourValue.h"

using namespace AnimatorModel;
using namespace Exception;

TimeAxis::TimeAxis(QObject *parent,
                   Animation& animation,
                   int lowValue,
                   int highValue,
                   int frequency,
                   int priority,
                   bool isOpaque) :
    Axis(parent, animation, lowValue, highValue, lowValue, priority, isOpaque),
    iFrequency(frequency),
    iBackgroundColour(QColor(-1, -1, -1)),
    iIsPlaying(false),
    iRepeat(false),
    iPlayTimer(NULL) {

    if(iFrequency < 100) {
        iFrequency = 100;
    }

    // this is a massive hack to get over the fact that the
    // frequency is not being saved out properly

    // a proper fix will fix the codec to use an unsigned int to
    // store the frequency so it will not overflow
}

void TimeAxis::play(bool repeat) {
    if(!isPlaying()) {
        iRepeat = repeat;
        iPlayTimer = new QTimer(this);
        connect(iPlayTimer, SIGNAL(timeout()), this, SLOT(nextFrame()));
        iPlayTimer->start(frequency());

        iIsPlaying = true;
    }
}

void TimeAxis::stop() {
    if(isPlaying()) {
        iPlayTimer->stop();
        delete iPlayTimer;
        iPlayTimer = NULL;

        iIsPlaying = false;

        emit stopped();
    }
}

void TimeAxis::nextFrame() {
    if(currentFrameNum() == highValue()) {
        if(iRepeat) {
            setCurrentFrame(lowValue());
        } else {
            stop();
        }
    } else {
        setCurrentFrame(currentFrameNum() + 1);
    }
}
