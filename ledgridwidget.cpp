/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "ledgridwidget.h"

#include "ledwidget.h"
#include "mainwindow.h"
#include "socketwidget.h"

#include "constants.h"
#include "exceptions.h"

using namespace Exception;
using namespace AnimatorUi;

LedGridWidget::LedGridWidget(QWidget* parent,  Animation &animation) :
    ColourGroupWidget(parent, 0, 0),
    iAnimation(animation),
    iLedGridLayout(NULL),
    iCopyLed(NULL) {

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

    setFocusPolicy(Qt::ClickFocus);

    connect(&animation, SIGNAL(newLed(int, int)), this, SLOT(addLed(int, int)));
    connect(&animation, SIGNAL(newSocket(int, int)), this, SLOT(addSocket(int, int)));
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

void LedGridWidget::addWidget(SelectableWidget *widget, int row, int column) {
    QLayoutItem* oldItem = iLedGridLayout->itemAtPosition(row, column);

    QWidget* oldWidget = NULL;
    if(oldItem == NULL) {
        iLedGridLayout->addWidget(widget, row, column);
    } else {
        oldWidget = oldItem->widget();
        iLedGridLayout->removeWidget(oldItem->widget());
        iLedGridLayout->addWidget(widget, row, column);
    }

    widget->resize(LED_RADIUS*2, LED_RADIUS*2);

    iLedGridLayout->setColumnMinimumWidth(column, widget->width() + 4);
    iLedGridLayout->setRowMinimumHeight(row, widget->height() + 4);

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

    if(numRows() < row + 1) {
        setMaxRow(row + 1);
    }

    if(numColumns() < column + 1) {
        setMaxColumn(column + 1);
    }

    widget->update();

    if(oldWidget != NULL) {
        oldWidget->update();
    }
}

void LedGridWidget::copyOrMoveLed(int fromRow, int fromColumn, int toRow, int toColumn) {
    Led& led = getLed(fromRow, fromColumn);

    if(led.isHidden()) {
        moveLed(led, toRow, toColumn);
    } else {
        copyLed(led, toRow, toColumn);
    }
}

void LedGridWidget::moveLed(int fromRow, int fromColumn, int toRow, int toColumn) {
    hideLed(fromRow, fromColumn);

    Led& led = getLed(fromRow, fromColumn);

    moveLed(led, toRow, toColumn);
}

void LedGridWidget::copyLed(Led& led, int toRow, int toColumn) {
    Led& newLed = iAnimation.addLed(toRow, toColumn);

    newLed.copyFrames(led);
    newLed.move(toRow, toColumn);
}

void LedGridWidget::moveLed(Led &led, int toRow, int toColumn) {
    iAnimation.moveLed(led, toRow, toColumn);

    addLed(toRow, toColumn, &led);

    led.move(toRow, toColumn);
    led.setHidden(false);
}

void LedGridWidget::hideLed(int row, int column) {
    Led& led = getLed(row, column);
    led.setHidden(true);

    addSocket(row, column);
}

void LedGridWidget::deleteLed(int row, int column) {
    Led& led = getLed(row, column);
    led.setHidden(true);

    addSocket(row, column);

    iAnimation.deleteLed(led);
}

void LedGridWidget::addSelectedLeds() {
    SelectableWidget* widget;
    int row;
    int column;

    foreach(widget, selectedItems()) {
        getWidgetPosition(*widget, &row, &column);
        Led& led = iAnimation.addLed(row, column);

        toggle(*widget);
    }
}

void LedGridWidget::hideSelectedLeds() {
    SelectableWidget* widget;
    int row;
    int column;

    foreach(widget, selectedItems()) {
        getWidgetPosition(*widget, &row, &column);
        hideLed(row, column);
    }
}

Led& LedGridWidget::getLed(int row, int column) {
    Led* led = iAnimation.ledAt(row, column);
    if(led == NULL) {
        throw IllegalArgumentException("LedGridWidget::getLed : led is NULL");
    }

    return *led;
}

// slots --------------------

Led& LedGridWidget::addLed(int row, int column, Led* led) {
    if(led == NULL) {
        led = iAnimation.ledAt(row, column);
    }

    addWidget(new LedWidget(this, iAnimation, *this, *led), row, column);

    return *led;
}

void LedGridWidget::addSocket(int row, int column) {
    GridItem* item = new GridItem(this, row, column);
    addWidget(new SocketWidget(this, *this, *item), row, column);
}

void LedGridWidget::toggleLedNumbers() {
    if(iLedNumbersShown) {
        iLedNumbersShown = false;
    } else {
        iLedNumbersShown = true;
    }
}

// from ColourGroupWidget ----------------------

void LedGridWidget::getWidgetPosition(SelectableWidget &widget, int* row, int* column) {
    int rowSpan;
    int columnSpan;

    int index = iLedGridLayout->indexOf(&widget);

    iLedGridLayout->getItemPosition(index, row, column, &rowSpan, &columnSpan);

    SelectableGroupWidget::getWidgetPosition(widget, row, column);
}

SelectableWidget &LedGridWidget::widgetAt(int row, int column) {
    SelectableGroupWidget::widgetAt(row, column);

    return static_cast<SelectableWidget&>(*(iLedGridLayout->itemAtPosition(row, column)->widget()));
}

bool LedGridWidget::validKeyPress(Qt::Key key) {
    return (key & Qt::Key_Right) == Qt::Key_Right ||
            (key & Qt::Key_Left) == Qt::Key_Left ||
            (key & Qt::Key_Up) == Qt::Key_Up ||
            (key & Qt::Key_Down) == Qt::Key_Down;
}

void LedGridWidget::deleteIfNeeded(int row, int column) {
    Led& led = getLed(row, column);

    if(led.isHidden()) {
        iAnimation.deleteLed(led);
    }
}

// events ---------------------------

void LedGridWidget::mousePressEvent(QMouseEvent* event) {
    qDebug("gridWidget mousePress");
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

    int positionStartColumn = iDragStartPosition.x()/iLedGridLayout->columnMinimumWidth(0);
    int positionStartRow = iDragStartPosition.y()/iLedGridLayout->rowMinimumHeight(0);

    int positionEndColumn = event->pos().x()/iLedGridLayout->columnMinimumWidth(0) - 1;
    int positionEndRow = event->pos().y()/iLedGridLayout->rowMinimumHeight(0) - 1;

    if(positionEndColumn < 0) {
        positionEndColumn = 0;
    }

    if(positionEndRow < 0) {
        positionEndRow = 0;
    }

    if(positionEndColumn > numColumns() - 1) {
        positionEndColumn = numColumns() - 1;
    }

    if(positionEndRow > numRows() - 1) {
        positionEndRow = numRows() - 1;
    }

    if(positionStartColumn < 0) {
        positionStartColumn = 0;
    }

    if(positionStartRow < 0) {
        positionStartRow = 0;
    }

    if(positionStartColumn > numColumns() - 1) {
        positionStartColumn = numColumns() - 1;
    }

    if(positionStartRow > numRows() - 1) {
        positionStartRow = numRows() - 1;
    }

    selectOne(static_cast<ColourWidget&>(*iLedGridLayout->itemAtPosition(positionStartRow, positionStartColumn)->widget()));
    selectArea(static_cast<ColourWidget&>(*iLedGridLayout->itemAtPosition(positionEndRow, positionEndColumn)->widget()));
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
