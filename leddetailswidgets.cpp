/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "leddetailswidgets.h"

#include "led.h"
#include "ledwidget.h"
#include "framelistwidget.h"
#include "mainwindow.h"
#include "animationdetailswidgets.h"
#include "animation.h"

#include "exceptions.h"

using namespace Ui;
using namespace Exception;

LedDetailsWidget::LedDetailsWidget(QWidget *parent, const Animation &animation, Led& led, SelectableGroupWidget& ledGroup) :
    QWidget(parent),
    iPositionLabel(NULL),
    iCurrentFrameWidget(NULL),
    iFramesListWidget(NULL) {

    iPositionLabel = new QLabel(this);
    iPositionLabel->setText(QString("(%1, %2)").arg(led.row()).arg(led.column()));
    iPositionLabel->move(5, 10);

    iCurrentFrameWidget = new LedWidget(this, animation, ledGroup, led);
    iCurrentFrameWidget->move(50, 10);
    iCurrentFrameWidget->resize(20, 20);

    iFramesListWidget = new FrameListWidget(this, animation, led);
    iFramesListWidget->move(iCurrentFrameWidget->width() + 60, 5);

    connect(iFramesListWidget, SIGNAL(resized(int,int)), this, SLOT(framesListResized(int, int)));
}

Led& LedDetailsWidget::led() {
    return iCurrentFrameWidget->led();
}

void LedDetailsWidget::framesListResized(int x, int width) {
    static_cast<AnimationDetailsWidget*>(parentWidget()->parentWidget())->frameListPosition(x, width);

}

void LedDetailsWidget::paintEvent(QPaintEvent*) {

  /*  QPainter painter(this);
    painter.setBrush(Qt::magenta);
    painter.drawRect(0, 0, width(), height());*/
}

void LedDetailsWidget::resizeEvent(QResizeEvent *) {
    iFramesListWidget->resize(width() - iPositionLabel->width() - iCurrentFrameWidget->width(), iFramesListWidget->height());
}

// ------------------------------------

LedDetailsListWidget::LedDetailsListWidget(QWidget *parent, const Animation &animation) :
    SelectableGroupWidget(parent, 0, 0),
    iAnimation(animation) {

    iLedDetailsList = new QVBoxLayout(this);
    setLayout(iLedDetailsList);
}

void LedDetailsListWidget::addLed(int row, int column) {
    Led& led = iAnimation.ledAt(row, column);

    if(!iShownLeds.contains(&led)) {
        iShownLeds.append(&led);

        LedDetailsWidget* ledDetails = new LedDetailsWidget(this, iAnimation, led, *this);
        iLedDetailsList->addWidget(ledDetails, 1);

        ledDetails->setMaximumHeight(40);
        ledDetails->setMinimumHeight(40);

        setMinimumHeight(iLedDetailsList->count() * 50);
        setMaximumHeight(iLedDetailsList->count() * 50);

        setMaxColumn(iLedDetailsList->count());

        qDebug("add new led, %d, %d", row, column);
    }
}

SelectableWidget& LedDetailsListWidget::widgetAt(int row, int column) {
    if(column != 0) {
        throw new IllegalArgumentException("Column should be zero");
    }

    return static_cast<SelectableWidget&>(*iLedDetailsList->itemAt(row)->widget()->childAt(1, 0));
}

void LedDetailsListWidget::getWidgetPosition(SelectableWidget &widget, int *row, int *column) {
    row = 0;
    column= 0;

    //TODO
}

// events -------------------------------------

void LedDetailsListWidget::resizeEvent(QResizeEvent *) {
}

void LedDetailsListWidget::paintEvent(QPaintEvent *) {
   /* QPainter painter(this);
    painter.setBrush(Qt::cyan);
    painter.drawRect(0, 0, width(), height());*/
}
