#include "ledwidget.h"

#include "led.h"
#include "defaults.h"

LedWidget::LedWidget(LedGridWidget& parent, Led& led)  :
    QWidget(&parent),
    iLedGridWidget(parent),
    iLed(led)
{
    move(led.position().x() * LED_OFFSET_X, led.position().y() * LED_OFFSET_Y);
    resize(LED_RADIUS*2, LED_RADIUS*2);
}

Led& LedWidget::led() {
    return iLed;
}

// events --------------------

void LedWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(iLed.selected() ? Qt::DashLine : Qt::NoPen);
    painter.setBrush(iLed.currentColour());

    QRect rect(1, 1, width() - 2, height() - 2);

    painter.drawEllipse(rect);
}

void LedWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        iLed.setCurrentColour(QColorDialog::getColor(Qt::green, this, "Select Color", QColorDialog::DontUseNativeDialog));

        update();
    }
}

void LedWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        if((QApplication::keyboardModifiers() & Qt::ControlModifier) != 0) {
             led().select(!led().selected());

             update();
        } else {
            bool selected = led().selected();

            iLedGridWidget.clearLedSelection();

            if(!selected) {
                led().select(true);

                update();
            }
        }
    }
}

// -------------------------
