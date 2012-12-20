#ifndef FRAMEDETAILSWIDGET_H
#define FRAMEDETAILSWIDGET_H

#include <QWidget>
#include <QSlider>

#include "ledwidgets.h"

class MainWindow;
class Led;
class LedDetailsListWidget;

class AnimationDetailsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AnimationDetailsWidget(QWidget* parent, Animation &animation);
    
signals:
    
public slots:
    void newLed(Led& led);
    void currentFrameChanged(int currentFrame);
    void numFramesChanged(int numFrames);

protected:
    void resizeEvent(QResizeEvent*);
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dropEvent(QDropEvent* event);

private:
    Animation&     iAnimation;

    QSlider*                iFrameSlider;
    LedDetailsListWidget*   iLedDetailsList;

    int added;
};

#endif // FRAMEDETAILSWIDGET_H
