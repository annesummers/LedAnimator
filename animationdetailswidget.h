#ifndef FRAMEDETAILSWIDGET_H
#define FRAMEDETAILSWIDGET_H

#include <QWidget>
#include <QSlider>
#include <QVBoxLayout>
#include <QSignalMapper>
#include <QLabel>
#include <QPushButton>
#include <QHash>

#include "selectablegroupgroupwidget.h"
#include "led.h"

namespace AnimatorModel {
class Led;
class Animation;
}

using namespace AnimatorModel;

namespace AnimatorUi {

class LedDetails;

class AnimationDetailsWidget : public SelectableGroupGroupWidget {
    Q_OBJECT

public:
    explicit AnimationDetailsWidget(QWidget* parent, Animation &animation);

    void frameListPosition(int x, int width);
    void closeClicked(LedDetails &details);

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
    void addLed(int row, int column);
    bool handleDragDropEvent(QDropEvent* event);

    Animation&              iAnimation;

    int iFramesListX;
    int iFramesListWidth;

    QSlider*                iFrameSlider;
    QGridLayout*            iGridLayout;

    QSignalMapper*          iSignalMapper;
    QHash<int, LedDetails*> iLedDetails;
    QHash<int, Led*>        iShownLeds;

    bool iClosed;
};

class LedDetails : public QObject {
    Q_OBJECT

public:
    explicit LedDetails(AnimationDetailsWidget &parent, Led& led, QLabel& label, QPushButton& closeButton);

    inline int ledNumber() { return iLed.number(); }

private slots:
    void closeClicked();
    void ledUpdated();

private:
    Led& iLed;
    QLabel& iLabel;
    QPushButton& iCloseButton;

    AnimationDetailsWidget& iDetailsWidget;
};

}

#endif // FRAMEDETAILSWIDGET_H
