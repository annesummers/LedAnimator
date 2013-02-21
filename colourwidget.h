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
#include "selectablewidget.h"
#include "selectablegroupwidget.h"
#include "colourgroupwidget.h"

namespace AnimatorUi {

class ColourGroupWidget;

class ColourWidget : public SelectableWidget {
    Q_OBJECT

public:
    ColourWidget(QWidget* parent, ColourGroupWidget& groupWidget, Selectable& item);

    virtual const QColor colour() const = 0;
    virtual void setColour(QColor colour) = 0;

    virtual void handleMimeData(QDataStream& dataStream, bool move);
    virtual void writeMimeData(QDataStream& dataStream);

protected slots:
    void updated();
    void copy();

private slots:
    void chooseColour();
  //  void setAllFramesColour();
    void colourDialogAccepted();

    void fade();
    void fadeTo();

protected:
    void contextMenuEvent(QContextMenuEvent *event);
    void mouseDoubleClickEvent(QMouseEvent* event);

    void keyPressEvent(QKeyEvent *event);

    virtual void cut() {}

    bool iFading;
  //  bool iSetAllFrames;

    QColorDialog* iColourDialog;

private:
    ColourGroupWidget& colourGroup() { return static_cast<ColourGroupWidget&>(iSelectableGroup); }

    QAction* iSetColourAction;
 //   QAction* iSetAllFramesColourAction;
    QAction* iFadeAction;
    QAction* iFadeToAction;
    QAction* iCopyAction;
};
}

#endif // SELECTABLEWIDGET_H
