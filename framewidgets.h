#ifndef FRAMEWIDGET_H
#define FRAMEWIDGET_H

#include <QWidget>
#include <QHBoxLayout>

class Frame;
class Led;
class Animation;

class FrameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FrameWidget(QWidget *parent, Frame& frame);
    
signals:
    
public slots:
    void frameColourChanged();
    void frameSelected();

protected:
    void paintEvent(QPaintEvent *);
    void mouseDoubleClickEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent* event);

private:
    Frame& iFrame;
};

class FrameListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FrameListWidget(QWidget *parent, Animation &animation, Led& led);



   // void newWidth(int width);

signals:

public slots:
    void numFramesChanged(int numFrames);

protected:
    void resizeEvent(QResizeEvent *);

private:
    // TODO this has to be a boxlayout
   // QList<FrameWidget*> iFrames;
    QHBoxLayout*  iFramesList;

    Led& iLed;
};

#endif // FRAMEWIDGET_H
