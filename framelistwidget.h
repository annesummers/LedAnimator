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
                             const Animation &animation,
                             const Led& led,
                             ColourGroupGroupWidget &framesListGroup,
                             int groupNumber);

    const Led& led() { return iLed; }

signals:
    void resized();

private slots:
    void numFramesChanged(int numFrames);

protected:
    void moveItem(int fromGroup, int fromRow, int fromColumn, int toRow, int toColumn) { Q_UNUSED(fromGroup); Q_UNUSED(fromRow); Q_UNUSED(fromColumn); Q_UNUSED(toRow); Q_UNUSED(toColumn); }
    void copyItem(int fromGroup, int fromRow, int fromColumn, int toRow, int toColumn);

    SelectableWidget& widgetAt(int row, int column);
    void getWidgetPosition(SelectableWidget &widget, int* row, int* column);

    bool validKeyPress(Qt::Key key);

    void resizeEvent(QResizeEvent *);
private:
    QList<FrameWidget*> iFramesList;

    const Led& iLed;

    bool iResized;
};
}

#endif // FRAMELISTWIDGET_H
