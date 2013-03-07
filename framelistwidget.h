#ifndef FRAMELISTWIDGET_H
#define FRAMELISTWIDGET_H

#include <QtGui>

#include "colourgroupwidget.h"

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

    void cloneItem(int fromGroup, int fromRow, int fromColumn, int toRow, int toColumn);

    inline void moveItem(int fromGroup, int fromRow, int fromColumn, int toRow, int toColumn)
        { Q_UNUSED(fromGroup); Q_UNUSED(fromRow); Q_UNUSED(fromColumn); Q_UNUSED(toRow); Q_UNUSED(toColumn); }
    inline void pasteItem(int fromGroup, int fromRow, int fromColumn, int toRow, int toColumn)
        { Q_UNUSED(fromGroup); Q_UNUSED(fromRow); Q_UNUSED(fromColumn); Q_UNUSED(toRow); Q_UNUSED(toColumn); }

    SelectableWidget& widgetAt(int row, int column);
    Position widgetPosition(SelectableWidget &widget);

private:

    QList<FrameWidget*> iFramesList;

    const Led& iLed;

    bool iResized;
};
}

#endif // FRAMELISTWIDGET_H
