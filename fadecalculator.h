#ifndef FADECALCULATOR_H
#define FADECALCULATOR_H

#include <QObject>
#include <QColor>
#include <QTimer>

class FadeCalculator : public QObject {
    Q_OBJECT

public:
    explicit FadeCalculator(QObject *parent,
                            const QColor startColour,
                            const QColor endColour,
                            const int iterations);

    void start();

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

    const int iRedIncrement;
    const int iGreenIncrement;
    const int iBlueIncrement;

    int iCurrentRedValue;
    int iCurrentGreenValue;
    int iCurrentBlueValue;

    int iCurrentIteration;

    QTimer* iTimer;
};

#endif // FADECALCULATOR_H
