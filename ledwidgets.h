/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef LEDWIDGETS_H
#define LEDWIDGETS_H

#include <QtGui>

#include "led.h"
#include "selectablewidget.h"

namespace Ui {

class SelectableGroupWidget;

class LedWidget : public SelectableWidget {
    Q_OBJECT

public:
    explicit LedWidget(QWidget* parent, const Animation &animation, SelectableGroupWidget& ledGroup, Led& led);

    Led &led() const { return static_cast<Led&>(iItem); }
    
private slots:
    void    ledColourChanged();

    void    currentFrameChanged(int currentFrame);

protected:
    void paintEvent(QPaintEvent* event);
    void mouseDoubleClickEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent *event);

private:
    QPixmap iPixmap;
    void setColourToolTip();
};
}

#endif // LEDWIDGETS_H
