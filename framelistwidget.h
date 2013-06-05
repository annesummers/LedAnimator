#ifndef FRAMELISTWIDGET_H
#define FRAMELISTWIDGET_H

#include <QtGui>

#include "colourgroupwidget.h"
#include "constants.h"

namespace AnimatorModel {
class Led;
class Animation;
}

using namespace AnimatorModel;

namespace AnimatorUi {

class FrameWidget;

class FrameListWidget : public ColourGroupWidget {
    Q_OBJECT

public:
    explicit FrameListWidget(QWidget *parent,
                             const Animation& animation,
                             const Led& led,
                             ColourGroupGroupWidget &framesListGroup,
                             int groupNumber);

    const Led& led() { return iLed; }
    void doResize();

signals:
    void resized();

private slots:
    void numFramesChanged(int numFrames);

protected:
    bool validKeyPress(Qt::Key key);
    void resizeEvent(QResizeEvent *);

    void cloneItem(int fromGroup, Position fromPosition, Position toPosition);

    inline void moveItem(int fromGroup, Position fromPosition, Position toPosition)
        { Q_UNUSED(fromGroup); Q_UNUSED(fromPosition); Q_UNUSED(toPosition); }
    inline void pasteItem(int fromGroup, Position fromPosition, Position toPosition)
        { Q_UNUSED(fromGroup); Q_UNUSED(fromPosition); Q_UNUSED(toPosition); }

    SelectableWidget& widgetAt(Position position);
    Position widgetPosition(SelectableWidget &widget);

private:
    QList<FrameWidget*> iFramesList;

    const Led& iLed;

    bool iResized;
};
}

#endif // FRAMELISTWIDGET_H
