/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef SELECTABLEWIDGET_H
#define SELECTABLEWIDGET_H

#include <QWidget>
#include <QColorDialog>

#include "selectable.h"

namespace Ui {

class ColourGroupWidget;

class ColourWidget : public QWidget {
    Q_OBJECT

public:
    ColourWidget(QWidget* parent, ColourGroupWidget& groupWidget, Selectable& item);

    inline void select(bool selected) { iItem.select(selected); }

    virtual const QColor colour() const = 0;
    virtual void setColour(QColor colour) = 0;

    virtual void handleExtraData(QDataStream& dataStream) = 0;
    virtual void addExtraData(QDataStream& dataStream) = 0;

protected slots:
    void colourChanged();

private slots:
    inline void selected() { update(); }

    void chooseColour();
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

    virtual QString mimeType() const = 0;
    virtual Qt::DropAction dropAction() const = 0;

    QPoint   iDragStartPosition;

    QAction* iSetColourAction;
    QAction* iFadeAction;
    QAction* iCopyAction;
    QAction* iPasteAction;

    ColourGroupWidget&  iColourGroup;
    Selectable&         iItem;

    bool iDoubleClick;

    QColorDialog* iColourDialog;
};
}

#endif // SELECTABLEWIDGET_H
