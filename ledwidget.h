/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef LEDWIDGETS_H
#define LEDWIDGETS_H

#include <QtGui>

#include "led.h"
#include "draggablewidget.h"

namespace Ui {

class SelectableGroupWidget;

class LedWidget : public DragDropWidget {
    Q_OBJECT

public:
    explicit LedWidget(QWidget* parent, const Animation &animation, SelectableGroupWidget& ledGroup, Led& led);

    inline Led &led() const { return static_cast<Led&>(iItem); }
    
private slots:
    void    ledColourChanged();
    void    currentFrameChanged(int currentFrame);

protected:
    void paintEvent(QPaintEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

    inline Qt::DropAction dropAction() const { return Qt::LinkAction; }
    const QByteArray dragData() const;
    inline QString mimeType() const { return LED_MIME_TYPE; }
    void handleDragData(QByteArray itemData);

private:
    void setColourToolTip();
};
}

#endif // LEDWIDGETS_H