/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef SELECTABLEWIDGET_H
#define SELECTABLEWIDGET_H

#include <QWidget>
#include <QColorDialog>
#include <QMimeData>
#include <QMenu>

#include "selectable.h"

namespace AnimatorUi {

class ColourGroupWidget;

class ColourWidget : public QWidget {
    Q_OBJECT

public:
    ColourWidget(QWidget* parent, ColourGroupWidget& groupWidget, Selectable& item);

    inline void select(bool selected) { iItem.select(selected); }
    inline bool isSelected() { return iItem.isSelected(); }

    virtual const QColor colour() const = 0;
    virtual void setColour(QColor colour) = 0;

    virtual void handleExtraData(QDataStream& dataStream) = 0;
    virtual void addExtraData(QDataStream& dataStream) = 0;

protected slots:
    void updated();

private slots:
    inline void selected() { update(); }

    void chooseColour();
    void colourDialogAccepted();

    void fade();
    void fadeTo();
    void copy();
    void paste();

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void dragEnterEvent(QDragEnterEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dropEvent(QDropEvent *);

    void contextMenuEvent(QContextMenuEvent *);

    virtual QString mimeType() const = 0;
    virtual Qt::DropAction dropAction() const = 0;

    QMimeData* mimeData();

    QPoint   iDragStartPosition;

    QAction* iSetColourAction;
    QAction* iFadeAction;
    QAction* iCopyAction;
    QAction* iPasteAction;

    ColourGroupWidget&  iColourGroup;
    Selectable&         iItem;

    bool iDoubleClick;
    bool iFading;

    QColorDialog* iColourDialog;

private:
    bool handleDragMoves(QDropEvent *event);
};
}

#endif // SELECTABLEWIDGET_H
