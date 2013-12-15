#ifndef ANIMATIONDETAILS_H
#define ANIMATIONDETAILS_H

#include <QObject>

class AxisDetails : public QObject {
    Q_OBJECT

public:
    AxisDetails();

    enum AxisType {
        ETime,
        EValue
    };

private:
    const AxisType iType;

    const int iLowValue;
    const int iHighValue;

    const int iZeroValue;

    const QList<AxisDetails> iSubAxes;
};

class AnimationDetails : public QObject {
    Q_OBJECT

public:
    AnimationDetails();

private:
    const int iNumRows;
    const int iNumColumns;

    const int iNumLeds;

    const QList<AxisDetails> iAxes;
};

#endif // ANIMATIONDETAILS_H
