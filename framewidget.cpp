/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "framewidget.h"

#include "frame.h"
#include "led.h"
#include "animation.h"
#include "colourgroupwidget.h"
#include "colourwidget.h"
#include "framelistwidget.h"

#include "exceptions.h"

#include <QPainter>

using namespace AnimatorUi;
using namespace Exception;

FrameWidget::FrameWidget(QWidget* parent, ColourGroupWidget& frameGroup, Frame& frame) :
    ColourWidget(parent, frameGroup, frame) {

    connect(&frame, SIGNAL(colourChanged()), this, SLOT(updated()));

    updated();
}

// events -----------------------------

void FrameWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setBrush(frame().colour());

    if(frame().isSelected() ){
        painter.setPen(Qt::white);
    } else {
        painter.setPen(Qt::black);
    }

    QRect rect(0, 0, width(), height());

    painter.drawRect(rect);

    painter.setBrush(Qt::NoBrush);
    painter.drawRect(0, 0, width() - 1, height() - 1);
}

