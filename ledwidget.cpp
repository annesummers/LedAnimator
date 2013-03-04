/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "ledwidget.h"

#include "led.h"
#include "ledgridwidget.h"
#include "mainwindow.h"
#include "animation.h"

#include "constants.h"

using namespace AnimatorUi;

LedWidget::LedWidget(QWidget *parent, Animation& animation, ColourGroupWidget& ledGroup, Led& led)  :
    ColourWidget(parent, ledGroup, led),
    iAnimation(animation) {

    setObjectName("LedWidget");

    connect(&led, SIGNAL(ledUpdated()), this, SLOT(updated()));
    connect(&animation, SIGNAL(currentFrameChanged(int)), this, SLOT(updated()));

    iCutAction = new QAction(tr("&Cut"), this);
    iCutAction->setStatusTip(tr("Cut selected"));

    connect(iCutAction, SIGNAL(triggered()), this, SLOT(cut()));

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

// from SelectableWidget -----------------------

void LedWidget::addCutAction(QMenu* menu) {
    Q_UNUSED(menu);
    menu->addAction(iCutAction);
}

void LedWidget::addExtraActions(QMenu* menu) {
    if(gridWidget().isSingleSelected()) {
        menu->addAction(iRenumberAction);
    }
    menu->addAction(iDeleteLedAction);
    menu->addAction(iSetGroupAction);
}

void LedWidget::cut() {
    copy();

    gridWidget().hideSelectedLeds();
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


