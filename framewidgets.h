/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef FRAMEWIDGET_H
#define FRAMEWIDGET_H

#include <QWidget>
#include <QHBoxLayout>

class Frame;
class Led;
class Animation;

namespace Ui {

class FrameListWidget : public QWidget {
    Q_OBJECT

public:
    explicit FrameListWidget(QWidget *parent, const Animation &animation, const Led& led);

signals:
    void resized(int x, int newWidth);

private slots:
    void numFramesChanged(int numFrames);

protected:
    void resizeEvent(QResizeEvent *);

private:
    QHBoxLayout*  iFramesList;

    const Led& iLed;
};

class FrameWidget : public QWidget {
    Q_OBJECT

public:
    explicit FrameWidget(Frame& frame);

private slots:
    void frameColourChanged();
    void frameSelected();

protected:
    void paintEvent(QPaintEvent *);
    void mouseDoubleClickEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent* event);

private:
    Frame& iFrame;
};
}

#endif // FRAMEWIDGET_H
