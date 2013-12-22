/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "colourwidget.h"

#include <QtGui>
#include <QApplication>

#include "selectable.h"
#include "colourgroupwidget.h"
#include "ColourValue.h"

using namespace AnimatorUi;

ColourWidget::ColourWidget(QWidget* parent, ColourGroupWidget &groupWidget, Selectable& item) :
    SelectableWidget(parent, groupWidget, item),
    iSignalMapper(NULL),
    iSetColourAction(NULL),
    iFadeAction(NULL),
    iFadeToAction(NULL),
    iFading(false),
    iFunctionFading(false){

    iSetColourAction = new QAction(tr("&Set colour..."), this);
    iSetColourAction->setStatusTip(tr("Choose a colour"));

    connect(iSetColourAction, SIGNAL(triggered()), this, SLOT(chooseColour()));

    iFadeAction = new QAction(tr("&Fade"), this);
    iFadeAction->setStatusTip("Fade to last selected colour");

    connect(iFadeAction, SIGNAL(triggered()), this, SLOT(fade()));

    iFadeToAction = new QAction(tr("&Fade to..."), this);
    iFadeToAction->setStatusTip("Fade to chosen colour");

    connect(iFadeToAction, SIGNAL(triggered()), this, SLOT(fadeTo()));

    connect(&item, SIGNAL(selected()), this, SLOT(selected()));

    iSignalMapper = new QSignalMapper(this);
    iSignalMapper->setMapping(&colourGroup().colourDialog(), iItem.number());
    connect(&colourGroup().colourDialog(), SIGNAL(accepted()), iSignalMapper, SLOT(map()));
}

void ColourWidget::chooseColour() {
    connect(iSignalMapper, SIGNAL(mapped(int)), this, SLOT(colourDialogAccepted(int)));
    colourGroup().colourDialog().setCurrentColor(colour());
    colourGroup().colourDialog().exec();
}

void ColourWidget::colourDialogAccepted(int number) {
    if(number == iItem.number()) {
        QColor newColour = colourGroup().colourDialog().currentColor();
        if(newColour.isValid()) {
            if(iFading) {
                colourGroup().fadeTo(newColour);

                iFading = false;
            } else if(iFunctionFading) {
                colourGroup().functionFadeTo(newColour);

                iFunctionFading = false;
            } else {
                colourGroup().setValue(*(new ColourValue(this, newColour)));
            }
        }
    }

    iSignalMapper->disconnect(this);
}

void ColourWidget::updated() {
    int hue = colour().hue();
    if(hue == -1) {
        hue = 0;
    }
    setToolTip(QString("h: %1\ns: %2\nv: %3")
               .arg(hue)
               .arg(colour().saturation())
               .arg(colour().value()));

    update();
}

void ColourWidget::fade() {
    if(!colourGroup().isAreaSelected()) {
        return;
    }

    colourGroup().fade();
}

void ColourWidget::fadeTo() {
    if(!colourGroup().isAreaSelected()) {
        return;
    }

    iFading = true;

    chooseColour();
}

// from SelectableWidget ------------------

void ColourWidget::addDefaultAction(QMenu* menu) {
    menu->addAction(iSetColourAction);
    menu->addSeparator();
}

void ColourWidget::addExtraActions(QMenu *menu) {
    if(colourGroup().isAreaSelected()) {
        menu->addSeparator();
        menu->addAction(iFadeAction);
        menu->addAction(iFadeToAction);
    }
}

// events -----------------------------------

void ColourWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    SelectableWidget::mouseDoubleClickEvent(event);

    chooseColour();
}
