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
    explicit LedWidget(QWidget* parent, Animation &animation, ColourGroupWidget& ledGroup, Led& led);

    void hideLed();

    inline Led &led() const { return static_cast<Led&>(iItem); }

    void setColour(QColor colour) {led().setCurrentColour(colour); }
    const QColor colour() const {return led().currentColour(); }

private slots:
    void cut();
    void renumber();
    void setGroup();
    void deleteLed();

protected:
    void paintEvent(QPaintEvent *event);

    inline QString mimeType() const { return LED_MIME_TYPE; }

    inline Qt::DropActions dropActions() const { return Qt::CopyAction | Qt::MoveAction; }
    inline Qt::DropActions dragActions() const { return Qt::CopyAction | Qt::MoveAction | Qt::LinkAction; }
    inline Qt::DropAction  defaultDropAction() const { return Qt::CopyAction; }
    inline Qt::DropAction  controlDropAction() const { return Qt::MoveAction; }

    void addCutAction(QMenu* menu);
    void addExtraActions(QMenu* menu);

    inline bool shouldMove() { return gridWidget().shouldMoveLeds(); }

private:
    LedGridWidget& gridWidget() { return static_cast<LedGridWidget&>(iSelectableGroup); }

    Animation& iAnimation;

    QAction* iCutAction;
    QAction* iRenumberAction;
    QAction* iSetGroupAction;
    QAction* iDeleteLedAction;
};
}

#endif // LEDWIDGETS_H
