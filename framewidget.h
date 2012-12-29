/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef FRAMEWIDGET_H
#define FRAMEWIDGET_H

#include <QtGui>

#include "draggablewidget.h"
#include "frame.h"

#include "defaults.h"

namespace Ui {

class FrameWidget : public DragDropWidget {
    Q_OBJECT

public:
    explicit FrameWidget(QWidget* parent, SelectableGroupWidget &frameGroup, Frame& frame);

private:
    inline Frame& frame() const { return dynamic_cast<Frame&>(iItem); }

private slots:
    void frameColourChanged();

    void copy();
    void paste();

protected:
    void paintEvent(QPaintEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

    inline Qt::DropAction dropAction() const { return Qt::CopyAction; }
    const QByteArray dragData() const;
    inline QString mimeType() const { return FRAME_MIME_TYPE; }
    void handleDragData(QByteArray itemData);

private:
    QAction* copyAct;
    QAction* pasteAct;
};
}

#endif // FRAMEWIDGET_H
