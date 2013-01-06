/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef SELECTABLEWIDGET_H
#define SELECTABLEWIDGET_H

#include <QWidget>

#include "selectable.h"

namespace Ui {

class ColourGroupWidget;

class ColourWidget : public QWidget {
    Q_OBJECT

public:
    ColourWidget(QWidget* parent, ColourGroupWidget& groupWidget, Selectable& item);

    void select(bool selected) { iItem.select(selected); }

    virtual const QColor colour() const = 0;
    virtual void setColour(QColor colour) = 0;

    virtual void handleExtraData(QDataStream& dataStream) = 0;
    virtual void addExtraData(QDataStream& dataStream) = 0;

protected slots:
    void colourChanged();

private slots:
    void selected() { update(); }

    void fade();
    void copy();
    void paste();

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

    void dragEnterEvent(QDragEnterEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dropEvent(QDropEvent *);

    void contextMenuEvent(QContextMenuEvent *);

    const QByteArray mimeData();
    void handleMimeData(QByteArray mimeData);

    virtual QString mimeType() const = 0;
    virtual Qt::DropAction dropAction() const = 0;

    QPoint    iDragStartPosition;

    QAction* iFadeAction;
    QAction* iCopyAction;
    QAction* iPasteAction;

    ColourGroupWidget&  iColourGroup;

    Selectable&         iItem;
};
}

#endif // SELECTABLEWIDGET_H
