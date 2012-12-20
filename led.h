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
    explicit Led(Animation& animation, int row, int column);

    QColor      currentColour();
    void        setCurrentColour(QColor colour);

    QColor      colourAt(int frameNum);
    void        setColourAt(int frameNum, QColor colour);

    int         row();
    int         column();

    bool        isSelected();
    void        select(bool isSelected);

    Frame&      frameAt(int frameNum);

signals:
    void        selected();
    void        currentColourChanged();

private:
    Animation& iAnimation;

    int iRow;
    int iColumn;
    QList<Frame*> iFrames;

    bool    iIsSelected;
};

#endif // LED_H
