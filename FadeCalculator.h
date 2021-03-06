/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef FADECALCULATOR_H
#define FADECALCULATOR_H

#include <QObject>
#include <QColor>
#include <QTimer>

#include "Function.h"

using namespace AnimatorModel;

class FadeCalculator : public QObject {
    Q_OBJECT

public:
    explicit FadeCalculator(QObject *parent,
                            const QColor startColour,
                            const QColor endColour,
                            const int iterations);

    void start();

    inline const Function function() const { return Function(iRedIncrement, iGreenIncrement, iBlueIncrement); }

signals:
    void colourCalculated(QColor colour);
    void fadeComplete();

public slots:
    void nextColour();

private:
    void resetColour();
    QColor currentColour();

    const QColor iStartColour;
    const QColor iEndColour;

    const int iIterations;

    const float iRedIncrement;
    const float iGreenIncrement;
    const float iBlueIncrement;

    float iHueIncrement;
    float iSatIncrement;
    float iValIncrement;

    float iCurrentRedValue;
    float iCurrentGreenValue;
    float iCurrentBlueValue;

    float iCurrentHueValue;
    float iCurrentSatValue;
    float iCurrentValValue;

    int iCurrentIteration;

    QTimer* iTimer;
};

#endif // FADECALCULATOR_H
