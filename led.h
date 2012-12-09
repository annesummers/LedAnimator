#ifndef LED_H
#define LED_H

#include <QObject>
#include <QColor>
#include <QPoint>

class LedWidget;

class Frame;
class Animation;

class Led : public QObject
{
    Q_OBJECT
public:
    explicit Led(Animation& animation, QPoint position);

    QColor      currentColour();
    void        setCurrentColour(QColor colour);

    QColor      colourAt(int frameNum);
    void        setColourAt(int frameNum, QColor colour);

    QPoint      position();

    bool        selected();
    void        select(bool selected);

signals:
    void        colourChanged(QPoint position, int frame);

private:
    Animation& iAnimation;

    QPoint iPosition;
    QList<Frame*> iFrames;

    bool    iIsSelected;
};

#endif // LED_H
