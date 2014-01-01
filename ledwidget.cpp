/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "ledwidget.h"

#include "Led.h"
#include "ledgridwidget.h"
#include "mainwindow.h"
#include "Animation.h"

#include "constants.h"

using namespace AnimatorUi;

LedWidget::LedWidget(QWidget *parent, Animation& animation, ColourGroupWidget& ledGroup, Led& led)  :
    ColourWidget(parent, ledGroup, led) {

    setObjectName("LedWidget");
    setMouseTracking(true);
    setAcceptDrops(false);

    connect(&led, SIGNAL(ledUpdated()), this, SLOT(ledUpdated()));
    connect(&led, SIGNAL(updated()), this, SLOT(updated()));

    iRenumberAction = new QAction(tr("&Renumber..."), this);
    iRenumberAction->setStatusTip(tr("Renumber this led"));

    connect(iRenumberAction, SIGNAL(triggered()), this, SLOT(renumber()));

    iDeleteLedAction = new QAction(tr("&Delete"), this);
    iDeleteLedAction->setStatusTip(tr("Delete selected leds"));

    connect(iDeleteLedAction, SIGNAL(triggered()), this, SLOT(deleteLed()));

    iSetGroupAction = new QAction(tr("&Set group..."), this);
    iSetGroupAction->setStatusTip(tr("Set selected led's group"));

    connect(iSetGroupAction, SIGNAL(triggered()), this, SLOT(setGroup()));
}

void LedWidget::renumber() {
    gridWidget().renumberLed(led());
}

void LedWidget::setGroup() {
    gridWidget().setSelectedLedsGroupNumber();
}

void LedWidget::deleteLed() {
    gridWidget().deleteSelectedLeds();
}

void LedWidget::ledUpdated() {
    updated();

    gridWidget().setCurrentLed(led());
}

// from SelectableWidget -----------------------

void LedWidget::addExtraActions(QMenu* menu) {
    ColourWidget::addExtraActions(menu);

    menu->addSeparator();

    if(gridWidget().isSingleSelected()) {
        menu->addAction(iRenumberAction);
    }

    menu->addAction(iDeleteLedAction);
    menu->addAction(iSetGroupAction);
}

// events ------------------------------------

void LedWidget::mouseMoveEvent(QMouseEvent *event) {
    SelectableWidget::mouseMoveEvent(event);
    gridWidget().setCurrentLed(led());
}

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

        /*if(gridWidget().ledNumbersShown()) {
            painter.setPen(led().currentColour().value() > 100 ? Qt::black : Qt::white);
            QRect rect3(4, 4, width()-8, height()-8);
            painter.drawText(rect3, QString("%1").arg(led().number()));
        }*/
    }
}


