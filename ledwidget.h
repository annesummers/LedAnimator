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
#include "ledgridwidget.h"

namespace AnimatorUi {

class LedWidget : public ColourWidget {
    Q_OBJECT

public:
    explicit LedWidget(QWidget* parent, const Animation &animation, ColourGroupWidget& ledGroup, Led& led);

    void hideLed();

private slots:
    void cut();

protected:
    void paintEvent(QPaintEvent *event);

    void handleMimeData(QDataStream& dataStream, bool move);
    void writeMimeData(QDataStream &dataStream);

    void setColour(QColor colour) {led().setCurrentColour(colour); }
    const QColor colour() const {return led().currentColour(); }

    inline QString mimeType() const { return LED_MIME_TYPE; }

    inline Qt::DropActions dropActions() const { return Qt::CopyAction | Qt::MoveAction; }
    inline Qt::DropActions dragActions() const { return Qt::CopyAction | Qt::MoveAction | Qt::LinkAction; }
    inline Qt::DropAction  defaultDropAction() const { return Qt::CopyAction; }
    inline Qt::DropAction  controlDropAction() const { return Qt::MoveAction; }

    void addCutAction(QMenu* menu);

private:
    inline Led &led() const { return static_cast<Led&>(iItem); }
    LedGridWidget& gridWidget() { return static_cast<LedGridWidget&>(iSelectableGroup); }

    const Animation& iAnimation;

    QAction* iCutAction;
};
}

#endif // LEDWIDGETS_H
