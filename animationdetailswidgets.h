#ifndef FRAMEDETAILSWIDGET_H
#define FRAMEDETAILSWIDGET_H

#include <QWidget>
#include <QSlider>

//#include "ledwidget.h"

class Led;
class Animation;

namespace Ui {

class LedDetailsListWidget;

class AnimationDetailsWidget : public QWidget {

    Q_OBJECT
public:
    explicit AnimationDetailsWidget(QWidget* parent, Animation &animation);

    void frameListPosition(int x, int width);

private slots:
    void currentFrameChanged(int currentFrame);
    void numFramesChanged(int numFrames);

protected:
    void resizeEvent(QResizeEvent*);
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dropEvent(QDropEvent* event);

    void paintEvent(QPaintEvent *);

private:
    Animation&              iAnimation;

    int iFramesListX;
    int iFramesListWidth;

    QSlider*                iFrameSlider;
    LedDetailsListWidget*   iLedDetailsList;

    int added;
};
}

#endif // FRAMEDETAILSWIDGET_H
