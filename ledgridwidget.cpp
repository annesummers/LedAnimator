/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "ledgridwidget.h"

#include "ledwidgets.h"
#include "mainwindow.h"

#include "defaults.h"

using namespace Ui;

LedGridWidget::LedGridWidget(QWidget* parent, const Animation &animation) :
    SelectableGroupWidget(parent),
    iAnimation(animation),
    iLedGridLayout(NULL) {

    iContainerWidget = new QWidget(this);
    iLedGridLayout = new QGridLayout(iContainerWidget);

    iContainerWidget->setLayout(iLedGridLayout);
    iContainerWidget->move(BORDER, BORDER);

    iDragStartPosition.setX(0);
    iDragStartPosition.setY(0);

    iDragArea.setRect(0, 0, 0, 0);

    QSizePolicy policy = sizePolicy();
    policy.setHorizontalStretch(0);
    policy.setVerticalStretch(0);
    setSizePolicy(policy);

    connect(&animation, SIGNAL(newLed(int, int)), this, SLOT(addLed(int, int)));
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

Led& LedGridWidget::ledAtPosition(int row, int column) {
    return static_cast<LedWidget*>(iLedGridLayout->itemAtPosition(row, column)->widget())->led();
}

// slots --------------------

void LedGridWidget::addLed(int row, int column) {
    LedWidget* widget = new LedWidget(this, iAnimation, *this, iAnimation.ledAt(row, column));

    iLedGridLayout->addWidget(widget, row, column);
    widget->resize(LED_RADIUS*2, LED_RADIUS*2);

    iLedGridLayout->setColumnMinimumWidth(column, widget->width() + 2);
    iLedGridLayout->setRowMinimumHeight(row, widget->height() + 2);

    resize(gridWidth(), gridHeight());

    iContainerWidget->setMaximumHeight(gridHeight());
    iContainerWidget->setMaximumWidth(gridWidth());

    iContainerWidget->setMinimumHeight(gridHeight());
    iContainerWidget->setMinimumWidth(gridWidth());

    resize(gridWidth() + BORDER*2, gridHeight() + BORDER*2);

    setMinimumHeight(gridHeight() + BORDER*2);
    setMinimumWidth(gridWidth() + BORDER*2);

    setMaximumHeight(gridHeight() + BORDER*2);
    setMaximumWidth(gridWidth() + BORDER*2);
}

// events --------------

void LedGridWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton &&
       (QApplication::keyboardModifiers() & Qt::ControlModifier) != 0) {

        QColor colour = QColorDialog::getColor(Qt::green, this, "Select Color", QColorDialog::DontUseNativeDialog);
        if(colour.isValid()) {
            Selectable* item = NULL;
            foreach(item, selectedItems() ){
                (static_cast<Led*>(item))->setCurrentColour(colour);
            }
        }
    }
}

void LedGridWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton &&
       (QApplication::keyboardModifiers() & Qt::ControlModifier) == 0) {
        iDragStartPosition = event->pos();
        iDragArea.setRect(0, 0, 0, 0);

        clearSelection();
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
                select(led, selected);
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

    painter.setPen(Qt::black);

    painter.drawRect(BORDER,
                     BORDER,
                     width() - BORDER*2,
                     height() - BORDER*2);
}

// -------------------
