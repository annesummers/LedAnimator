#ifndef FRAMEDETAILSWIDGET_H
#define FRAMEDETAILSWIDGET_H

#include <QWidget>
#include <QSlider>
#include <QVBoxLayout>
#include <QSignalMapper>
#include <QLabel>
#include <QPushButton>
#include <QHash>

#include "colourgroupgroupwidget.h"
#include "led.h"
#include "framelistwidget.h"

namespace AnimatorModel {
class Led;
class Animation;
}

using namespace AnimatorModel;

namespace AnimatorUi {

class LedDetails;
class ScrollContentsWidget;

class AnimationDetailsWidget : public ColourGroupGroupWidget {
    Q_OBJECT

public:
    explicit AnimationDetailsWidget(QWidget* parent, Animation &animation);

    void frameListPosition(int x, int width);
    void deleteLed(LedDetails &details);

private slots:
    void currentFrameChanged(int currentFrame);
    void numFramesChanged(int numFrames);
    void deleteLed(int row, int column);
    void renumberLed(int row, int column, int oldNumber);

    void framesResized();

protected:
    void resizeEvent(QResizeEvent*);
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dropEvent(QDropEvent* event);

private:
    void addLed(int row, int column);

    void doResize();

    bool handleDragDropEvent(QDropEvent* event);

    Animation&              iAnimation;

    int iFramesListX;
    int iFramesListWidth;

    QSlider*                iFrameSlider;
    QGridLayout*            iGridLayout;
    ScrollContentsWidget* iScrollAreaWidgetContents;

    QHash<int, LedDetails*> iLedDetails;
    QHash<int, Led*>        iShownLeds;

    bool iClosed;
};

class ScrollContentsWidget : public QWidget {
    Q_OBJECT

public:
    explicit ScrollContentsWidget(QWidget* parent, Animation &animation);
    void setFramesSize(QSize size);
    void setFramesPos(QPoint pos);

protected:
    void paintEvent(QPaintEvent *);

private:
    Animation& iAnimation;
    QSize iFramesSize;
    QPoint iFramesPos;
};

class LedDetails : public QObject {
    Q_OBJECT

public:
    explicit LedDetails(AnimationDetailsWidget &parent, Led& led, QLabel& label, FrameListWidget& framesListWidget, QPushButton& closeButton);

    inline int ledNumber() { return iLed.number(); }
    inline FrameListWidget& frameList() { return iFramesListWidget; }

private slots:
    void closeClicked();
    void ledUpdated();

private:
    Led& iLed;

    QLabel&             iLabel;
    FrameListWidget&    iFramesListWidget;
    QPushButton&        iCloseButton;

    AnimationDetailsWidget& iDetailsWidget;
};

}

#endif // FRAMEDETAILSWIDGET_H
