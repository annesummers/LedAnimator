/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

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
    virtual void setValue(FrameValue& value) = 0;

protected slots:
    virtual void update();
    void chooseColour();

private slots:
    void colourDialogAccepted(int number);

    void fade();
    void fadeTo();

protected:
    void mouseDoubleClickEvent(QMouseEvent* event);

    virtual void addDefaultAction(QMenu* menu);
    virtual void addExtraActions(QMenu* menu);

    ColourGroupWidget& colourGroup() { return static_cast<ColourGroupWidget&>(iSelectableGroup); }
    bool iFunctionFading;

private:
    QSignalMapper* iSignalMapper;

    QAction* iSetColourAction;
    QAction* iFadeAction;
    QAction* iFadeToAction;

    bool iFading;
};
}

#endif // SELECTABLEWIDGET_H
