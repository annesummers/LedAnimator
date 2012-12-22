/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef LEDWIDGETS_H
#define LEDWIDGETS_H

#include <QtGui>

#include "led.h"

namespace Ui {

class LedContainerWidget;

class LedWidget : public QWidget {
    Q_OBJECT

public:
    explicit LedWidget(QWidget* parent, const Animation &animation, LedContainerWidget& ledGroup, Led& led);

    Led &led() const;
    
private slots:
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

class LedContainerWidget : public QWidget {
    Q_OBJECT

public:
    explicit LedContainerWidget(QWidget *parent, const Animation& animation);

protected:
    virtual void    addLed(int row, int column) = 0;
    virtual Led&    ledAt(int index) = 0;

    virtual int     count() = 0;

    void clearLedSelection();

    const Animation& iAnimation;
};
}

#endif // LEDWIDGETS_H
