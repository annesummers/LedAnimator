/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef FRAMEWIDGET_H
#define FRAMEWIDGET_H

#include <QtGui>

#include "colourwidget.h"
#include "Frame.h"

#include "constants.h"

namespace AnimatorUi {

class FrameWidget : public ColourWidget {
    Q_OBJECT

public:
    explicit FrameWidget(QWidget* parent, ColourGroupWidget &frameGroup, Frame& frame);
    inline Frame& frame() const { return static_cast<Frame&>(iItem); }

    const QColor colour() const { return frame().colour(); }

protected:
    void paintEvent(QPaintEvent *event);

    void addDefaultAction(QMenu* menu);
    void addExtraActions(QMenu *menu);

    void setValue(FrameValue &value) { frame().setValue(value); }

    inline Qt::DropActions dropActions() const { return Qt::CopyAction; }
    inline Qt::DropActions dragActions() const { return Qt::CopyAction; }
    inline Qt::DropAction  defaultDropAction() const { return Qt::CopyAction; }
    inline Qt::DropAction  controlDropAction() const { return Qt::IgnoreAction; }

    QAction* iFunctionFadeAction;
    QAction* iFunctionFadeToAction;

private slots:
    void functionFade();
    void functionFadeTo();
};
}

#endif // FRAMEWIDGET_H
