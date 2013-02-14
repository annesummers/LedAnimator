/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "ledwidget.h"

#include "led.h"
#include "ledgridwidget.h"
#include "leddetailswidgets.h"
#include "mainwindow.h"
#include "animation.h"

#include "defaults.h"

using namespace AnimatorUi;

LedWidget::LedWidget(QWidget* parent, const Animation& animation, ColourGroupWidget& ledGroup, Led& led)  :
    ColourWidget(parent, ledGroup, led) {

    connect(&led, SIGNAL(ledUpdated()), this, SLOT(updated()));
   // QObject::connect(&led, SIGNAL(selected()), this, SLOT(selected()));

    connect(&(animation), SIGNAL(currentFrameChanged(int)), this, SLOT(updated()));
}

// from DraggableWidget -----------------------

void LedWidget::addExtraData(QDataStream& dataStream) {
    dataStream << led().row() << led().column();
}

void LedWidget::handleExtraData(QDataStream &dataStream) {
    int row;
    int column;

    dataStream >> row >> column;
}

// events ------------------------------------

void LedWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(led().isSelected() ? Qt::white : Qt::lightGray);
    painter.setBrush(led().currentColour());

    QRect rect(2, 2, width()-4, height()-4);

    painter.drawEllipse(rect);

    if(led().isSelected()) {
        painter.setBrush(Qt::NoBrush);
        painter.setPen(Qt::DashLine);
        painter.drawEllipse(rect);

        QRect rect2(3, 3, width()-5, height()-5);
        painter.setPen(Qt::white);
        painter.drawEllipse(rect2);
        painter.setPen(Qt::DashLine);
        painter.drawEllipse(rect2);
    }

    int number = led().detailsNumber();
    if(number != INVALID) {
        painter.setPen(led().currentColour().value() > 100 ? Qt::black : Qt::white);
        QRect rect3(4, 4, width()-8, height()-8);
        painter.drawText(rect3, QString("%1").arg(number));
    }
}


