#ifndef FRAMEDETAILSWIDGET_H
#define FRAMEDETAILSWIDGET_H

#include <QWidget>
#include <QSlider>
#include <QVBoxLayout>
#include <QSignalMapper>

#include "colourgroupgroupwidget.h"

class Led;
class Animation;

//namespace Ui {
//class AnimationDetailsWidget;
//}

namespace AnimatorUi {

class LedDetailsListWidget;

class AnimationDetailsWidget : public ColourGroupGroupWidget {

    Q_OBJECT
public:
    explicit AnimationDetailsWidget(QWidget* parent, Animation &animation);
    void frameListPosition(int x, int width);

private slots:
    void currentFrameChanged(int currentFrame);
    void numFramesChanged(int numFrames);

    void closeClicked(int whichLed);
    void closeClicked();

protected:
    void resizeEvent(QResizeEvent*);
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dropEvent(QDropEvent* event);

    void paintEvent(QPaintEvent *);

private:
    void addLed(int row, int column);
    Animation&              iAnimation;

    int iFramesListX;
    int iFramesListWidth;

    QSlider*                iFrameSlider;
    QGridLayout*            iGridLayout;

    QSignalMapper*          iSignalMapper;
    QList<Led*>             iShownLeds;

 //   ColourGroupGroupWidget* iFramesListGroup;

    bool iClosed;
};
}

#endif // FRAMEDETAILSWIDGET_H
