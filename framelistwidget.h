#ifndef FRAMELISTWIDGET_H
#define FRAMELISTWIDGET_H

#include <QtGui>

#include "colourgroupwidget.h"
#include "constants.h"

namespace AnimatorModel {
class Led;
class AxisData;
}

using namespace AnimatorModel;

namespace AnimatorUi {

class FrameWidget;

class FrameListWidget : public ColourGroupWidget {
    Q_OBJECT

public:
    explicit FrameListWidget(QWidget *parent,
                             const AxisData& axisData,
                             ColourGroupGroupWidget &framesListGroup);
    void doResize();

signals:
    void resized();

private slots:
    void handleLowValueChanged(int oldLowValue, int lowValue);
    void handleHighValueChanged(int oldHighValue, int highValue);
    void handleFramesInserted(int numFrames, int framesAdded);

protected:
    bool validKeyPress(Qt::Key key);
    void resizeEvent(QResizeEvent *);

    void cloneItem(int fromGroup, Position fromPosition, Position toPosition);

    inline void moveItem(int fromGroup, Position fromPosition, Position toPosition)
        { Q_UNUSED(fromGroup); Q_UNUSED(fromPosition); Q_UNUSED(toPosition); }
    inline void pasteClipboardItem(int fromGroup, Position fromPosition, Position toPosition)
        { Q_UNUSED(fromGroup); Q_UNUSED(fromPosition); Q_UNUSED(toPosition); }

    SelectableWidget& widgetAt(Position position);
    Position widgetPosition(SelectableWidget &widget);

private:
    void setSize();

    QList<FrameWidget*> iFramesList;

    const AxisData& iAxisData;

    bool iResized;
};
}

#endif // FRAMELISTWIDGET_H
