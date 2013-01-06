#ifndef FADECALCULATOR_H
#define FADECALCULATOR_H

#include <QObject>
#include <QColor>
#include <QTimer>

class FadeCalculator : public QObject
{
    Q_OBJECT
public:
    explicit FadeCalculator(QObject *parent, QColor startColor, QColor endColor, int iterations, bool backwards = false);
    
    void start();

signals:
    void colourCalculated(QColor colour);
    
public slots:
    void nextColour();

private:
    void resetColour();
    QColor currentColour();

    const QColor iStartColour;
    const QColor iEndColour;

    const int iIterations;

    const int iRedIncrement;
    const int iGreenIncrement;
    const int iBlueIncrement;

    bool iBackwards;

    int iCurrentRedValue;
    int iCurrentGreenValue;
    int iCurrentBlueValue;

    int iCurrentIteration;

    QTimer* iTimer;
};

#endif // FADECALCULATOR_H
