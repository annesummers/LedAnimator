/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "ledgridwidget.h"

#include "ledwidgets.h"
#include "mainwindow.h"

#include "defaults.h"
#include "exceptions.h"

using namespace Exception;
using namespace Ui;

LedGridWidget::LedGridWidget(QWidget* parent, const Animation &animation) :
    SelectableGroupWidget(parent, 0, 0),
    iAnimation(animation),
    iLedGridLayout(NULL){

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

    if(maxRow() < row + 1) {
        setMaxRow(row + 1);
    }

    if(maxColumn() < column + 1) {
        setMaxColumn(column + 1);
    }
}

void LedGridWidget::getWidgetPosition(SelectableWidget& widget, int* row, int* column) {
    int rowSpan;
    int columnSpan;

    iLedGridLayout->getItemPosition(iLedGridLayout->indexOf(&widget), row, column, &rowSpan, &columnSpan);
}

SelectableWidget& LedGridWidget::widgetAt(int row, int column) {
    if(row < 0) {
        throw IllegalArgumentException("Row is negative");
    }

    if(row >= maxRow()) {
        throw IllegalArgumentException("Row is greater than maximum rows");
    }

    if(column < 0) {
        throw IllegalArgumentException("Column is negative");
    }

    if(column >= maxColumn()) {
        throw IllegalArgumentException("Column is greater than maximum columns");
    }

    return static_cast<SelectableWidget&>(*(iLedGridLayout->itemAtPosition(row, column)->widget()));
}


// events --------------

void LedGridWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    qDebug("gridWidget mouseDoubleClick");
    if (event->button() == Qt::LeftButton &&
       (QApplication::keyboardModifiers() & Qt::ControlModifier) != 0) {

        QColor colour = QColorDialog::getColor(Qt::green, this, "Select Color", QColorDialog::DontUseNativeDialog);
        if(colour.isValid()) {
            SelectableWidget* item = NULL;
            foreach(item, selectedItems() ){
                (static_cast<Led&>(item->item())).setCurrentColour(colour);
            }
        }
    }
}

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

    if(positionEndColumn > maxColumn() - 1) {
        positionEndColumn = maxColumn() - 1;
    }

    if(positionEndRow > maxRow() - 1) {
        positionEndRow = maxRow() - 1;
    }

    if(positionStartColumn < 0) {
        positionStartColumn = 0;
    }

    if(positionStartRow < 0) {
        positionStartRow = 0;
    }

    if(positionStartColumn > maxColumn() - 1) {
        positionStartColumn = maxColumn() - 1;
    }

    if(positionStartRow > maxRow() - 1) {
        positionStartRow = maxRow() - 1;
    }

    //selectArea(positionStartRow, positionEndRow, positionStartColumn, positionEndColumn);*/

    selectOne(static_cast<SelectableWidget&>(*iLedGridLayout->itemAtPosition(positionStartRow, positionStartColumn)->widget()));
    selectArea(static_cast<SelectableWidget&>(*iLedGridLayout->itemAtPosition(positionEndRow, positionEndColumn)->widget()));
}

void LedGridWidget::mouseReleaseEvent(QMouseEvent *) {
    iDragArea.setRect(0,0,0,0);

    update();
}

void LedGridWidget::keyPressEvent(QKeyEvent *event) {
    if((QApplication::keyboardModifiers() & Qt::ShiftModifier) != 0) {

        QList<SelectableWidget*> selectedWidgets = selectedItems();

        if(selectedWidgets.count() > 0) {
            int row;
            int column;

            if((event->key() & Qt::RightArrow) != 0 ||
               (event->key() & Qt::LeftArrow) != 0 ||
               (event->key() & Qt::UpArrow) != 0 ||
               (event->key() & Qt::DownArrow) != 0 ) {

                getWidgetPosition(static_cast<LedWidget&>(*selectedWidgets.at(selectedWidgets.count() - 1)),
                                                            &row,
                                                            &column);

                if(event->key() == Qt::Key_Right) {
                    selectRight(column);
                } else if(event->key() == Qt::Key_Left) {
                    selectLeft(column);
                } else if(event->key() == Qt::Key_Up) {
                    selectUp(row);
                } else if(event->key() == Qt::Key_Down) {
                    selectDown(row);
                }
            }
        }
    }
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
