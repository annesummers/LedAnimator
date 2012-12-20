#ifndef LEDWIDGETS_H
#define LEDWIDGETS_H

#include <QtGui>

#include "led.h"

class LedContainerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LedContainerWidget(QWidget *parent, Animation& animation);

signals:

public slots:

protected:
    virtual void    addLed(Led& led) = 0;
    virtual Led&    ledAt(int index) = 0;

    virtual int     count() = 0;

    void clearLedSelection();

    Animation&      iAnimation;
};

class LedWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LedWidget(QWidget* parent, Animation &animation, LedContainerWidget& ledGroup, Led& led);

    Led& led();
    
signals:
    
public slots:
    void    ledColourChanged();
    void    ledSelected();

    void    currentFrameChanged(int currentFrame);

protected:
    void paintEvent(QPaintEvent* event);
    void mouseDoubleClickEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent* event);

private:
    LedContainerWidget& iLedGroup;
    Led& iLed;
};

#endif // LEDWIDGETS_H
