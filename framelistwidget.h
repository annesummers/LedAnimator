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
                             ColourGroupGroupWidget &framesListGroup);

    const Led& led() { return iLed; }
signals:
    void resized(int x, int newWidth);

private slots:
    void numFramesChanged(int numFrames);

protected:
    void moveItem(int fromRow, int fromColumn, int toRow, int toColumn) {}
    void copyItem(int fromRow, int fromColumn, int toRow, int toColumn);

    SelectableWidget& widgetAt(int row, int column);
    void getWidgetPosition(SelectableWidget &widget, int* row, int* column);

    bool validKeyPress(Qt::Key key);

    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);

private:
    QList<FrameWidget*> iFramesList;

    const Led& iLed;
};
}

#endif // FRAMELISTWIDGET_H
