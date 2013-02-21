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

#include "constants.h"

using namespace AnimatorUi;

LedWidget::LedWidget(QWidget *parent, const Animation& animation, ColourGroupWidget& ledGroup, Led& led)  :
    ColourWidget(parent, ledGroup, led),
    iAnimation(animation) {

    setObjectName("LedWidget");

    connect(&led, SIGNAL(ledUpdated()), this, SLOT(updated()));
   // QObject::connect(&led, SIGNAL(selected()), this, SLOT(selected()));

    connect(&animation, SIGNAL(currentFrameChanged(int)), this, SLOT(updated()));

    iCutAction = new QAction(tr("&Cut"), this);
    iCutAction->setStatusTip(tr("Cut this led"));

    connect(iCutAction, SIGNAL(triggered()), this, SLOT(cut()));
}

// from SelectableWidget -----------------------

void LedWidget::writeMimeData(QDataStream& dataStream) {
    ColourWidget::writeMimeData(dataStream);


    dataStream << led().row() << led().column();
    qDebug("row %d, column %d", led().row(), led().column());
}

void LedWidget::handleMimeData(QDataStream &dataStream, bool move) {
    ColourWidget::handleMimeData(dataStream, move);

    int row;
    int column;

    dataStream >> row >> column;
}

void LedWidget::addCutAction(QMenu* menu) {
    menu->addAction(iCutAction);
}

void LedWidget::cut() {
    copy();

    gridWidget().hideSelectedLeds();
}

// events ------------------------------------

void LedWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    if(!isHidden()) {
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

        if(gridWidget().ledNumbersShown()) {
            painter.setPen(led().currentColour().value() > 100 ? Qt::black : Qt::white);
            QRect rect3(4, 4, width()-8, height()-8);
            painter.drawText(rect3, QString("%1").arg(led().number()));
        }
    }
}


