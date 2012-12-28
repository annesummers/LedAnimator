/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "ledwidgets.h"

#include "led.h"
#include "ledgridwidget.h"
#include "leddetailswidgets.h"
#include "mainwindow.h"
#include "animation.h"

#include "defaults.h"

using namespace Ui;

LedWidget::LedWidget(QWidget* parent, const Animation& animation, SelectableGroupWidget& ledGroup, Led& led)  :
    SelectableWidget(parent, ledGroup, led) {

    setAcceptDrops(false);

    QObject::connect(&led, SIGNAL(currentColourChanged()), this, SLOT(ledColourChanged()));
    QObject::connect(&led, SIGNAL(selected()), this, SLOT(selected()));

    QObject::connect(&(animation), SIGNAL(currentFrameChanged(int)), this, SLOT(currentFrameChanged(int)));
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
    painter.setPen(led().isSelected() ? Qt::black : Qt::lightGray);
    painter.setBrush(led().currentColour());

    QRect rect(2, 2, width()-4, height()-4);

    painter.drawEllipse(rect);

    QPainter painter2(this);
    painter2.begin(&iPixmap);

    painter2.setRenderHint(QPainter::Antialiasing);
    painter2.setPen(led().isSelected() ? Qt::black : Qt::white);
    painter2.setBrush(led().currentColour());

    painter2.drawEllipse(rect);
    painter2.end();
}

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

void LedWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() != Qt::LeftButton) {
        return;
    }

    if ((event->pos() - iDragStartPosition).manhattanLength()
             < QApplication::startDragDistance()) {
       return;
    }

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    dataStream << led().row() << led().column();

     QMimeData *mimeData = new QMimeData;
     mimeData->setData("application/x-leditemdata", itemData);

     QDrag *drag = new QDrag(this);
     drag->setMimeData(mimeData);
     drag->setHotSpot(pos());

     drag->setPixmap(iPixmap);

     if (drag->exec(Qt::LinkAction, Qt::LinkAction) == Qt::LinkAction) {
         //qDebug(QString("Successfully dragged and dropped led %1,%2").arg(iLed.row()).arg(iLed.column()));
     } else {
         //qDebug(QString("Wrong! led %1,%2").arg(iLed.row()).arg(iLed.column()));
     }
}
