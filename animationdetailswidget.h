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
    explicit AnimationDetailsWidget(QWidget* parent, Animation &animation, Engine& engine);

    void frameListPosition(int x, int width);
    void deleteLed(LedDetails &details);

    inline bool canCut() const { return false; }

private slots:
    void currentFrameChanged(int currentFrame);
    void numFramesChanged(int numFrames);

    void ledDeleted(int row, int column, int ledNumber);
    void ledRenumbered(int row, int column, int oldNumber);

    void framesResized();
    void closeAllClicked();

protected:
    void resizeEvent(QResizeEvent*);
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dropEvent(QDropEvent* event);

private:
    void addLed(int row, int column);

    void doResize();

    bool handleDragDropEvent(QDropEvent* event);

    inline QString mimeType() const { return FRAME_MIME_TYPE; }

    Animation&              iAnimation;

    int iFramesListX;
    int iFramesListWidth;

    QSlider*                iFrameSlider;
    QGridLayout*            iGridLayout;
    ScrollContentsWidget*   iScrollAreaWidgetContents;
    QToolButton*            iCloseAll;

    QHash<int, LedDetails*> iLedDetails;

    int iResize;
};

class ScrollContentsWidget : public QWidget {
    Q_OBJECT

public:
    explicit ScrollContentsWidget(QWidget* parent, Animation &animation);

    inline void setFramesSize(QSize size) { iFramesSize = size; }
    inline void setFramesPos(QPoint pos) { iFramesPos = pos; }

    inline void setShowCurrentFrameLine(bool show) { iShowLine = show; }

protected:
    void paintEvent(QPaintEvent *);

private:
    Animation& iAnimation;
    QSize iFramesSize;
    QPoint iFramesPos;

    bool iShowLine;
};

class LedDetails : public QObject {
    Q_OBJECT

public:
    explicit LedDetails(AnimationDetailsWidget &parent, Led& led, QLabel& label, FrameListWidget& framesListWidget, QToolButton& closeButton);

    inline int ledNumber() { return iLed.number(); }
    inline FrameListWidget& frameList() { return iFramesListWidget; }

private slots:
    void closeClicked();
    void ledUpdated();

private:
    Led& iLed;

    QLabel&             iLabel;
    FrameListWidget&    iFramesListWidget;
    QToolButton&        iCloseButton;

    AnimationDetailsWidget& iDetailsWidget;
};

}

#endif // FRAMEDETAILSWIDGET_H
