/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef LEDWIDGETS_H
#define LEDWIDGETS_H

#include <QtGui>

#include "led.h"
#include "colourwidget.h"

namespace AnimatorUi {

class ColourGroupWidget;

class LedWidget : public ColourWidget {
    Q_OBJECT

public:
    explicit LedWidget(QWidget* parent, const Animation &animation, ColourGroupWidget& ledGroup, Led& led);

protected:
    void paintEvent(QPaintEvent *event);

    void addExtraData(QDataStream& dataStream);
    void handleExtraData(QDataStream &dataStream);

    void setColour(QColor colour) {led().setCurrentColour(colour); }
    const QColor colour() const {return led().currentColour(); }

    inline Qt::DropAction dropAction() const { return Qt::LinkAction; }
    inline QString mimeType() const { return LED_MIME_TYPE; }

private:
    inline Led &led() const { return static_cast<Led&>(iItem); }
};
}

#endif // LEDWIDGETS_H
