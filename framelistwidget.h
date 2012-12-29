#ifndef FRAMELISTWIDGET_H
#define FRAMELISTWIDGET_H

#include <QtGui>

#include "selectablegroupwidget.h"

class Led;
class Animation;

namespace Ui {

class FrameWidget;

class FrameListWidget : public SelectableGroupWidget {
    Q_OBJECT

public:
    explicit FrameListWidget(QWidget *parent, const Animation &animation, const Led& led);

signals:
    void resized(int x, int newWidth);

private slots:
    void numFramesChanged(int numFrames);

protected:
    SelectableWidget& widgetAt(int row, int column);
    void getWidgetPosition(SelectableWidget& widget, int* row, int* column);

    void keyPressEvent(QKeyEvent *);

    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);

    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dropEvent(QDropEvent* event);

private:
    QList<FrameWidget*> iFramesList;

    const Led& iLed;
};
}

#endif // FRAMELISTWIDGET_H
