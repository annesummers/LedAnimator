#include "ledgridwidget.h"

#include "ledwidget.h"
#include "defaults.h"

LedGridWidget::LedGridWidget(QWidget* parent) :
    QWidget(parent),
    iLedGridLayout(NULL) {

    iLedGridLayout = new QGridLayout(this);
    setLayout(iLedGridLayout);

    iDragStartPosition.setX(0);
    iDragStartPosition.setY(0);

    iDragArea.setRect(0, 0, 0, 0);
}

void LedGridWidget::addWidget(LedWidget& widget, int row, int column) {
    iLedGridLayout->addWidget(&widget, row, column);

    iLedGridLayout->setColumnMinimumWidth(column, widget.width() + 2);
    iLedGridLayout->setRowMinimumHeight(row, widget.height() + 2);

    resize(gridWidth(), gridHeight());

    setMaximumHeight(gridHeight());
    setMaximumWidth(gridWidth());

    setMinimumHeight(gridHeight());
    setMinimumWidth(gridWidth());
}

void LedGridWidget::clearLedSelection() {
    for(int i = 0; i < iLedGridLayout->count(); i++) {
        selectLed(i, false);
    }

    update();
}

void LedGridWidget::selectLed(int index, bool select) {
    LedWidget* widget = static_cast<LedWidget*>(iLedGridLayout->itemAt(index)->widget());
    widget->led().select(select);
}

void LedGridWidget::selectLed(int x, int y, bool select) {
    LedWidget* widget = static_cast<LedWidget*>(iLedGridLayout->itemAtPosition(x, y)->widget());
    widget->led().select(select);
}

int LedGridWidget::gridWidth() {
    int columnCount = iLedGridLayout->columnCount();
    if(columnCount > 0) {
        return columnCount * iLedGridLayout->columnMinimumWidth(0);
    } else {
        return 0;
    }
}

int LedGridWidget::gridHeight() {
    int rowCount = iLedGridLayout->rowCount();
    if(rowCount > 0) {
        return rowCount * iLedGridLayout->rowMinimumHeight(0);
    } else {
        return 0;
    }
}

void LedGridWidget::currentFrameChanged(int currentFrame) {
    for(int i = 0; i < iLedGridLayout->count(); i++) {
        LedWidget* widget = static_cast<LedWidget*>(iLedGridLayout->itemAt(i)->widget());
        widget->setToolTip(QString("h: %1\ns: %2\nv: %3")
                           .arg(widget->led().currentColour().hue())
                           .arg(widget->led().currentColour().saturation())
                           .arg(widget->led().currentColour().value()));
    }

    update();
}

void LedGridWidget::ledColourChanged(int row, int column, int frame) {
    //if(frame == )
    LedWidget* widget = static_cast<LedWidget*>(iLedGridLayout->itemAtPosition(row, column)->widget());
    widget->setToolTip(QString("h: %1\ns: %2\nv: %3")
                       .arg(widget->led().currentColour().hue())
                       .arg(widget->led().currentColour().saturation())
                       .arg(widget->led().currentColour().value()));
}

// events --------------

void LedGridWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton &&
       (QApplication::keyboardModifiers() & Qt::ControlModifier) != 0) {

        QColor colour = QColorDialog::getColor(Qt::green, this, "Select Color", QColorDialog::DontUseNativeDialog);
        for(int i = 0; i < iLedGridLayout->count(); i++) {
            LedWidget* widget = static_cast<LedWidget*>(iLedGridLayout->itemAt(i)->widget());
            if(widget->led().selected()) {
                widget->led().setCurrentColour(colour);
            }
        }

        update();
    }
}

void LedGridWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton &&
       (QApplication::keyboardModifiers() & Qt::ControlModifier) == 0) {
        iDragStartPosition = event->pos();
        iDragArea.setRect(0, 0, 0, 0);

        clearLedSelection();
    }
}

void LedGridWidget::mouseMoveEvent(QMouseEvent* event) {
    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }

    iDragArea.setTopLeft(iDragStartPosition);
    iDragArea.setBottomRight(event->pos());

    for(int i = 0; i < iLedGridLayout->rowCount(); i++) {
        for(int j = 0; j < iLedGridLayout->columnCount(); j++) {
            selectLed(i, j, iDragArea.contains(iLedGridLayout->cellRect(i, j)));

            update();
        }
    }
}

void LedGridWidget::mouseReleaseEvent(QMouseEvent *) {
    iDragArea.setRect(0,0,0,0);

    update();
}

void LedGridWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    painter.setPen(Qt::DashLine);
    painter.drawRect(iDragArea);
}

// -------------------

LedGridContainerWidget::LedGridContainerWidget(QWidget* parent, MainWindow &mainWindow) :
    QWidget(parent),
    iMainWindow(mainWindow),
    iLedGridWidget(NULL),
    iBorder(LED_GRID_BORDER){

    iLedGridWidget = new LedGridWidget(this);
    iLedGridWidget->move(iBorder, iBorder);

    QSizePolicy policy = sizePolicy();
    policy.setHorizontalStretch(0);
    policy.setVerticalStretch(0);
    setSizePolicy(policy);
}

void LedGridContainerWidget::newLed(Led &led) {
    LedWidget* widget = new LedWidget(*iLedGridWidget, led);

    int row = led.position().y();
    int column = led.position().x();

    iLedGridWidget->addWidget(*widget, row, column);

    int gridWidth = iLedGridWidget->gridWidth();
    int gridHeight = iLedGridWidget->gridHeight();

    resize(gridWidth + iBorder*2, gridHeight + iBorder*2);

    setMaximumHeight(gridHeight + iBorder*2);
    setMaximumWidth(gridWidth + iBorder*2);

    setMinimumHeight(gridHeight + iBorder*2);
    setMinimumWidth(gridWidth + iBorder*2);
}

// slots --------------------

void LedGridContainerWidget::currentFrameChanged(int currentFrame) {
    iLedGridWidget->currentFrameChanged(currentFrame);
}

void LedGridContainerWidget::ledColourChanged(int row, int column, int frame) {
    iLedGridWidget->ledColourChanged(row, column, frame);
}

// -------------------------

// events -------------------

void LedGridContainerWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    painter.setPen(Qt::black);

    painter.drawRect(iBorder, iBorder, width() - iBorder*2, height() - iBorder*2);
}

void LedGridContainerWidget::mousePressEvent(QMouseEvent* event) {

}

// ---------------------------
