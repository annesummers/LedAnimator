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

using namespace Ui;

LedWidget::LedWidget(QWidget* parent, const Animation& animation, SelectableGroupWidget& ledGroup, Led& led)  :
    DragDropWidget(parent, ledGroup, led) {

    QObject::connect(&led, SIGNAL(currentColourChanged()), this, SLOT(ledColourChanged()));
    QObject::connect(&led, SIGNAL(selected()), this, SLOT(selected()));

    QObject::connect(&(animation), SIGNAL(currentFrameChanged(int)), this, SLOT(ledColurChanged()));
}

void LedWidget::setColourToolTip() {
    setToolTip(QString("h: %1\ns: %2\nv: %3")
       .arg(led().currentColour().hue())
       .arg(led().currentColour().saturation())
       .arg(led().currentColour().value()));
}

void LedWidget::ledColourChanged() {
    setColourToolTip();

    update();
}

void LedWidget::currentFrameChanged(int currentFrame) {
    Q_UNUSED(currentFrame);

    setColourToolTip();

    update();
}

// events --------------------

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
}

// from DraggableWidget -----------------------

const QByteArray LedWidget::dragData() const {
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    dataStream << led().row() << led().column() << led().currentColour();

    return itemData;
}

void LedWidget::handleDragData(QByteArray itemData) {
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);

    int row;
    int column;
    QColor colour;

    dataStream >> row >> column >> colour;

    led().setCurrentColour(colour);
}

// events ------------------------------------

void LedWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    qDebug("singleWidget mouseDoubleClick");
    if (event->buttons() != Qt::LeftButton) {
        return;
    }

    iSelectableGroup.select(*this, true);

    QColor colour = QColorDialog::getColor(Qt::white,
                                           this,
                                           "Select Color",
                                           QColorDialog::DontUseNativeDialog);
    if(colour.isValid()) {
        SelectableWidget* item = NULL;

        foreach(item, iSelectableGroup.selectedItems()){
            static_cast<Led&>(item->item()).setCurrentColour(colour);
        }
    }
}


