#ifndef FRAMELISTWIDGET_H
#define FRAMELISTWIDGET_H

#include <QtGui>

#include "colourgroupwidget.h"

class Led;
class Animation;

namespace Ui {

class FrameWidget;

class FrameListWidget : public ColourGroupWidget {
    Q_OBJECT

public:
    explicit FrameListWidget(QWidget *parent, const Animation &animation, const Led& led);

signals:
    void resized(int x, int newWidth);

private slots:
    void numFramesChanged(int numFrames);

protected:
    ColourWidget& widgetAt(int row, int column);
    void getWidgetPosition(ColourWidget& widget, int* row, int* column);

    void keyPressEvent(QKeyEvent *);

    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);

private:
    QList<FrameWidget*> iFramesList;

    const Led& iLed;
};
}

#endif // FRAMELISTWIDGET_H
