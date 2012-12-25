/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "leddetailswidgets.h"

#include "led.h"
#include "ledwidgets.h"
#include "framewidgets.h"
#include "mainwindow.h"
#include "animationdetailswidgets.h"
#include "animation.h"

using namespace Ui;

LedDetailsWidget::LedDetailsWidget(QWidget *parent, const Animation &animation, Led& led, SelectableGroupWidget& ledGroup) :
    QWidget(parent),
    iPositionLabel(NULL),
    iCurrentFrameWidget(NULL),
    iFramesListWidget(NULL) {

    iPositionLabel = new QLabel(this);
    iPositionLabel->setText(QString("(%1, %2)").arg(led.row()).arg(led.column()));
    iPositionLabel->move(0, 15);

    iCurrentFrameWidget = new LedWidget(this, animation, ledGroup, led);
    iCurrentFrameWidget->move(40, 10);
    iCurrentFrameWidget->resize(20, 20);

    iFramesListWidget = new Ui::FrameListWidget(this, animation, led);
    iFramesListWidget->move(iCurrentFrameWidget->width() + 50, 0);
    iFramesListWidget->resize(width() - iPositionLabel->width() - iCurrentFrameWidget->width(), 30);

    connect(iFramesListWidget, SIGNAL(resized(int, int)), static_cast<AnimationDetailsWidget*>(parentWidget()->parentWidget()), SLOT(frameListPosition(int, int)));
}

Led& LedDetailsWidget::led() {
    return iCurrentFrameWidget->led();
}

void LedDetailsWidget::resizeEvent(QResizeEvent *) {
    iFramesListWidget->resize(width() - iPositionLabel->width() - iCurrentFrameWidget->width(), iFramesListWidget->height());
  //  iFramesWidgetWidth = width() - iPositionLabel->width() - iCurrentFrameWidget->width();
}

// ------------------------------------

LedDetailsListWidget::LedDetailsListWidget(QWidget *parent, const Animation &animation) :
    SelectableGroupWidget(parent),
    iAnimation(animation) {

    iLedDetailsList = new QVBoxLayout(this);
    setLayout(iLedDetailsList);
}

void LedDetailsListWidget::addLed(int row, int column) {
    Led& led = iAnimation.ledAt(row, column);

    if(!iShownLeds.contains(&led)) {
        iShownLeds.append(&led);

        //LedDetailsWidget* ledDetails = new LedDetailsWidget(this, iAnimation, led, *this);
        QWidget* ledDetails = new QWidget(this);

        QLabel* iPositionLabel = new QLabel(ledDetails);
        iPositionLabel->setText(QString("(%1, %2)").arg(led.row()).arg(led.column()));
        iPositionLabel->move(0, 15);

        LedWidget* iCurrentFrameWidget = new LedWidget(ledDetails, iAnimation, *this, led);
        iCurrentFrameWidget->move(40, 10);
        iCurrentFrameWidget->resize(20, 20);

        FrameListWidget* iFramesListWidget = new FrameListWidget(ledDetails, iAnimation, led);
        iFramesListWidget->move(iCurrentFrameWidget->width() + 50, 0);
        iFramesListWidget->resize(width() - iPositionLabel->width() - iCurrentFrameWidget->width(), 30);

        connect(iFramesListWidget, SIGNAL(resized(int, int)),
                static_cast<AnimationDetailsWidget*>(parentWidget()), SLOT(frameListPosition(int, int)));

        ledDetails->setMaximumHeight(30);
        ledDetails->setMinimumHeight(30);

        iLedDetailsList->addWidget(ledDetails, 1);

        setMinimumHeight(iLedDetailsList->count()*32 + 22);
        setMaximumHeight(iLedDetailsList->count()*32 + 22);

        qDebug("add new led, %d, %d", row, column);
    }
}

// events -------------------------------------

void LedDetailsListWidget::resizeEvent(QResizeEvent *) {
   // for(int i = 0; i < iLedDetailsList->count(); i++) {
   //     static_cast<LedDetailsWidget*>(iLedDetailsList->itemAt(i)->widget())->resize(width(), 30);
   // }
}
