/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef FRAMEWIDGET_H
#define FRAMEWIDGET_H

#include <QtGui>

#include "colourwidget.h"
#include "frame.h"

#include "defaults.h"

namespace Ui {

class FrameWidget : public ColourWidget {
    Q_OBJECT

public:
    explicit FrameWidget(QWidget* parent, ColourGroupWidget &frameGroup, Frame& frame);

protected:
    void paintEvent(QPaintEvent *event);

    void addExtraData(QDataStream& dataStream) {}
    void handleExtraData(QDataStream &dataStream) {}

    void setColour(QColor colour) { frame().setColour(colour); }
    const QColor colour() const { return frame().colour(); }

    inline Qt::DropAction dropAction() const { return Qt::CopyAction; }
    inline QString mimeType() const { return FRAME_MIME_TYPE; }

private:
    inline Frame& frame() const { return static_cast<Frame&>(iItem); }
};
}

#endif // FRAMEWIDGET_H
