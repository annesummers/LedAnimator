/*******************************/
/* Copyright Anne Summers 2012 */
/*******************************/

#include "ledgridwidget.h"

#include "ledwidgets.h"
#include "mainwindow.h"

#include "defaults.h"

LedGridWidget::LedGridWidget(QWidget* parent, Animation& animation) :
    LedContainerWidget(parent, animation),
    iLedGridLayout(NULL) {

    iLedGridLayout = new QGridLayout(this);
    setLayout(iLedGridLayout);

    iDragStartPosition.setX(0);
    iDragStartPosition.setY(0);

    iDragArea.setRect(0, 0, 0, 0);
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

int LedGridWidget::count() {
    return iLedGridLayout->count();
}

Led& LedGridWidget::ledAt(int index) {
    return static_cast<LedWidget*>(iLedGridLayout->itemAt(index)->widget())->led();
}

Led& LedGridWidget::ledAtPosition(int row, int column) {
    return static_cast<LedWidget*>(iLedGridLayout->itemAtPosition(row, column)->widget())->led();
}

// slots --------------------

void LedGridWidget::addLed(Led &led) {
    LedWidget* widget = new LedWidget(this, iAnimation, *this, led);

    iLedGridLayout->addWidget(widget, led.row(), led.column());
    widget->resize(30, 30);

    iLedGridLayout->setColumnMinimumWidth(led.column(), widget->width() + 2);
    iLedGridLayout->setRowMinimumHeight(led.row(), widget->height() + 2);

    resize(gridWidth(), gridHeight());

    setMaximumHeight(gridHeight());
    setMaximumWidth(gridWidth());

    setMinimumHeight(gridHeight());
    setMinimumWidth(gridWidth());

    parentWidget()->resize(gridWidth() + LED_GRID_BORDER*2, gridHeight() + LED_GRID_BORDER*2);

    parentWidget()->setMaximumHeight(gridHeight() + LED_GRID_BORDER*2);
    parentWidget()->setMaximumWidth(gridWidth() + LED_GRID_BORDER*2);

    parentWidget()->setMinimumHeight(gridHeight() + LED_GRID_BORDER*2);
    parentWidget()->setMinimumWidth(gridWidth() + LED_GRID_BORDER*2);
}

// events --------------

void LedGridWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton &&
       (QApplication::keyboardModifiers() & Qt::ControlModifier) != 0) {

        QColor colour = QColorDialog::getColor(Qt::green, this, "Select Color", QColorDialog::DontUseNativeDialog);
        if(colour.isValid()) {
            for(int i = 0; i < iLedGridLayout->count(); i++) {
                Led& led = ledAt(i);
                if(led.isSelected()) {
                    led.setCurrentColour(colour);
                }
            }

            update();
        }
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
            Led& led = ledAtPosition(i, j);
            bool selected = iDragArea.contains(iLedGridLayout->cellRect(i, j));
            bool wasSelected = led.isSelected();
            if((selected && !wasSelected) || (!selected && wasSelected)) {
                led.select(selected);
            }
        }

        update();
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

// TODO get rid

LedGridContainerWidget::LedGridContainerWidget(QWidget* parent) :
    QWidget(parent){

    QSizePolicy policy = sizePolicy();
    policy.setHorizontalStretch(0);
    policy.setVerticalStretch(0);
    setSizePolicy(policy);

    // TODO sort the stretch out in mainwindo
}

// events -------------------------------------

void LedGridContainerWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    painter.setPen(Qt::black);

    painter.drawRect(LED_GRID_BORDER, LED_GRID_BORDER, width() - LED_GRID_BORDER*2, height() - LED_GRID_BORDER*2);
}

// ---------------------------------------------
