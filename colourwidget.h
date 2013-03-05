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
#include <QSignalMapper>

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

protected slots:
    void updated();

private slots:
    void chooseColour();
    void colourDialogAccepted(int number);

    void fade();
    void fadeTo();

protected:
    void mouseDoubleClickEvent(QMouseEvent* event);

    void addDefaultAction(QMenu* menu);
    virtual void addExtraActions(QMenu* menu);

private:
    ColourGroupWidget& colourGroup() { return static_cast<ColourGroupWidget&>(iSelectableGroup); }

    QSignalMapper* iSignalMapper;

    QAction* iSetColourAction;
    QAction* iFadeAction;
    QAction* iFadeToAction;

    bool iFading;
};
}

#endif // SELECTABLEWIDGET_H
