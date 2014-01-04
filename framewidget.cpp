
/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "framewidget.h"

#include "Frame.h"
#include "Led.h"
#include "Animation.h"
#include "colourgroupwidget.h"
#include "colourwidget.h"
#include "framelistwidget.h"

#include "exceptions.h"

#include <QPainter>

using namespace AnimatorUi;
using namespace Exception;

FrameWidget::FrameWidget(QWidget* parent, ColourGroupWidget& frameGroup, Frame& frame) :
    ColourWidget(parent, frameGroup, frame),
    iFunctionFadeAction(NULL),
    iFunctionFadeToAction(NULL) {

    iFunctionFadeAction = new QAction(tr("F&unction fade"), this);
    iFunctionFadeAction->setStatusTip("Function fade to last selected colour");

    connect(iFunctionFadeAction, SIGNAL(triggered()), this, SLOT(functionFade()));

    iFunctionFadeToAction = new QAction(tr("Fu&nction fade to..."), this);
    iFunctionFadeToAction->setStatusTip("Function fade to chosen colour");

    connect(iFunctionFadeToAction, SIGNAL(triggered()), this, SLOT(functionFadeTo()));

    connect(&frame, SIGNAL(valueChanged()), this, SLOT(updated()));

    updated();
}

void FrameWidget::addDefaultAction(QMenu* menu) {
    if(frame().value().type() == kColour) {
        ColourWidget::addDefaultAction(menu);
    }
}

void FrameWidget::addExtraActions(QMenu *menu) {
    if(frame().value().type() == kColour) {
        ColourWidget::addExtraActions(menu);
    }

    if(colourGroup().isAreaSelected()) {
        FrameWidget& first = static_cast<FrameWidget&>(colourGroup().widgetAt(colourGroup().firstSelectedAreaPosition()));
        FrameWidget& last = static_cast<FrameWidget&>(colourGroup().widgetAt(colourGroup().lastSelectedAreaPosition()));

        if(((first.frame().value().type() == kLinked) &&
                last.frame().value().type() == kFunction) ||
            (((last.frame().value().type() == kLinked) &&
                first.frame().value().type() == kFunction))    ) {
            menu->addSeparator();
            menu->addAction(iFunctionFadeToAction);
        }

        /*if((first.frame().value().type() == kColour &&
           last.frame().value().type() == kColour)) {

            menu->addSeparator();
            menu->addAction(iFunctionFadeAction);
        }*/  // AS TODO colour to colour fade
    }  
}

void FrameWidget::functionFade() {
    if(!colourGroup().isAreaSelected()) {
        return;
    }

    colourGroup().functionFade();
}

void FrameWidget::functionFadeTo() {
    if(!colourGroup().isAreaSelected()) {
        return;
    }

    iFunctionFading = true;

    chooseColour();
}


// events -----------------------------

void FrameWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setBrush(frame().colour());


    painter.setPen(Qt::NoPen);

    QRect rect(0, 0, width(), height());
    painter.drawRect(rect);

    if(frame().isSelected() ){
        painter.setPen(Qt::white);
    } else {
        painter.setPen(Qt::black);
    }

    painter.setBrush(Qt::NoBrush);
    painter.drawLine(0, 0, width() - 1, 0);
    painter.drawLine(0, height() - 1, width() - 1, height() - 1);

    if(frame().value().type() == kColour ||
            frame().value().type() == kLinked) {
        if(frame().previous() == NULL ||
           frame().previous()->value().type() == kFunction) {
            painter.drawLine(0, 0, 0, height() - 1);
        }

        painter.drawLine(width() - 1, 0, width() - 1, height() - 1);
    } else if(frame().value().type() == kFunction) {

    }
}

