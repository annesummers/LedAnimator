/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef FRAMEWIDGET_H
#define FRAMEWIDGET_H

#include <QtGui>
#include <QHBoxLayout>

#include "selectablegroupwidget.h"
#include "selectablewidget.h"
#include "frame.h"

class Frame;
class Led;
class Animation;

namespace Ui {

class FrameListWidget : public SelectableGroupWidget {
    Q_OBJECT

public:
    explicit FrameListWidget(QWidget *parent, const Animation &animation, const Led& led);

signals:
    void resized(int x, int newWidth);

private slots:
    void numFramesChanged(int numFrames);

protected:
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dropEvent(QDropEvent* event);

private:
    QHBoxLayout*  iFramesList;

    const Led& iLed;
};

class FrameWidget : public SelectableWidget {
    Q_OBJECT

public:
    explicit FrameWidget(QWidget* parent, SelectableGroupWidget &frameGroup, Frame& frame);

private slots:
    inline Frame& frame() const { return dynamic_cast<Frame&>(iItem); }

    void frameColourChanged();

protected:
    void paintEvent(QPaintEvent *);
    void mouseDoubleClickEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent *event) {}
};
}

#endif // FRAMEWIDGET_H
