#include "ledwidgets.h"

#include "led.h"
#include "ledgridwidget.h"
#include "leddetailswidgets.h"
#include "mainwindow.h"
#include "animation.h"

#include "defaults.h"

LedWidget::LedWidget(QWidget* parent, Animation& animation, LedContainerWidget& ledGroup, Led& led)  :
    QWidget(parent),
    iLedGroup(ledGroup),
    iLed(led) {

    setAcceptDrops(false);

    QObject::connect(&led, SIGNAL(currentColourChanged()), this, SLOT(ledColourChanged()));
    QObject::connect(&led, SIGNAL(selected()), this, SLOT(ledSelected()));

    QObject::connect(&(animation), SIGNAL(currentFrameChanged(int)), this, SLOT(currentFrameChanged(int)));
}

Led& LedWidget::led() {
    return iLed;
}

void LedWidget::ledColourChanged() {
    setToolTip(QString("h: %1\ns: %2\nv: %3")
               .arg(iLed.currentColour().hue())
               .arg(iLed.currentColour().saturation())
               .arg(iLed.currentColour().value()));

    update();
}

void LedWidget::ledSelected() {
    update();
}

void LedWidget::currentFrameChanged(int currentFrame) {
    Q_UNUSED(currentFrame);

    setToolTip(QString("h: %1\ns: %2\nv: %3")
               .arg(iLed.currentColour().hue())
               .arg(iLed.currentColour().saturation())
               .arg(iLed.currentColour().value()));

    update();
}

// events --------------------

void LedWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(iLed.isSelected() ? Qt::DashLine : Qt::NoPen);
    painter.setBrush(iLed.currentColour());

    QRect rect(2, 2, width()-2, height()-2);

    painter.drawEllipse(rect);
}

void LedWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QColor colour = QColorDialog::getColor(Qt::green, this, "Select Color", QColorDialog::DontUseNativeDialog);
        if(colour.isValid()) {
            iLed.setCurrentColour(colour);

            update();
        }
    }
}

void LedWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        if((QApplication::keyboardModifiers() & Qt::ControlModifier) != 0) {
        // TODO selection is fucked
             led().select(!led().isSelected());

             update();

        } else {

            //TODO: drag and drop
            QByteArray itemData;
            QDataStream dataStream(&itemData, QIODevice::WriteOnly);
            dataStream << led().row() << led().column();

             QMimeData *mimeData = new QMimeData;
             mimeData->setData("application/x-leditemdata", itemData);

             QDrag *drag = new QDrag(this);
             drag->setMimeData(mimeData);
             drag->setHotSpot(pos());

             QPixmap pixmap;

             QPainter painter(this);
             painter.begin(&pixmap);

             painter.setRenderHint(QPainter::Antialiasing);
             painter.setPen(iLed.isSelected() ? Qt::DashLine : Qt::NoPen);
             painter.setBrush(iLed.currentColour());

             QRect rect(2, 2, width()-2, height()-2);

             painter.drawEllipse(rect);
             painter.end();

             drag->setPixmap(pixmap);

             if (drag->exec(Qt::LinkAction, Qt::LinkAction) == Qt::LinkAction) {
                 //qDebug(QString("Successfully dragged and dropped led %1,%2").arg(iLed.row()).arg(iLed.column()));
             } else {
                 //qDebug(QString("Wrong! led %1,%2").arg(iLed.row()).arg(iLed.column()));
             }
            /*bool selected = led().isSelected();

            iLedGroup.clearLedSelection();

            if(!selected) {
                led().select(true);

                update();
            }*/
        }
    }
}

LedContainerWidget::LedContainerWidget(QWidget *parent, Animation &animation) :
    QWidget(parent),
    iAnimation(animation) {
}

void LedContainerWidget::clearLedSelection() {
    for(int i = 0; i < count(); i++) {
        if(ledAt(i).isSelected()) {
            ledAt(i).select(false);
        }
    }
}
