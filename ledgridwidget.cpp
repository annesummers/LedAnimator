/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "ledgridwidget.h"

#include "ledwidget.h"
#include "mainwindow.h"
#include "socketwidget.h"
#include "colourgroupgroupwidget.h"
#include "ledanimatoractionbase.h"

#include "constants.h"
#include "exceptions.h"

using namespace Exception;
using namespace AnimatorUi;

LedGridWidget::LedGridWidget(QWidget* parent, Animation &animation, QUndoStack &undoStack, ColourGroupGroupWidget &groupGroup) :
    ColourGroupWidget(parent, 0, 0, groupGroup, 0),
    iAnimation(animation),
    iUndoStack(undoStack),
    iLedGridLayout(NULL),
    iCurrentLed(NULL),
    iLedNumbersShown(true) {

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
    connect(&animation, SIGNAL(ledDeleted(int, int, int)), this, SLOT(ledDeleted(int, int, int)));
    connect(&animation, SIGNAL(ledMoved(int, int, int, int)), this, SLOT(ledMoved(int, int, int, int)));
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

        iLedGridLayout->removeWidget(oldWidget);
        oldWidget->hide();

      //  if(selectedItems().count() != 0) {
         //   delete oldWidget;
       //xs }

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
}

void LedGridWidget::moveItem(int fromGroup, int fromRow, int fromColumn, int toRow, int toColumn) {
    Q_UNUSED(fromGroup);// there is only one group

    //qDebug("LedGridWidget::moveItem : from %d,%d to %d,%d", fromRow, fromColumn, toRow, toColumn);

  //  iUndoStack.push(new MoveLedCommand(iAnimation, fromRow, fromColumn, toRow, toColumn));
    iAnimation.moveLed(fromRow, fromColumn, toRow, toColumn);
}

void LedGridWidget::cloneItem(int fromGroup, int fromRow, int fromColumn, int toRow, int toColumn) {
    Q_UNUSED(fromGroup);// there is only one group
    qDebug("LedGridWidget::copyItem : from %d,%d to %d,%d", fromRow, fromColumn, toRow, toColumn);

    //iUndoStack.push(new CloneLedCommand(iAnimation, fromRow, fromColumn, toRow, toColumn));
    iAnimation.cloneLed(fromRow, fromColumn, toRow, toColumn);
}

void LedGridWidget::pasteItem(int fromGroup, int fromRow, int fromColumn, int toRow, int toColumn) {
    Q_UNUSED(fromGroup);// there is only one group
    qDebug("LedGridWidget::pasteItem : from %d,%d to %d,%d", fromRow, fromColumn, toRow, toColumn);

    iAnimation.pasteLed(fromRow, fromColumn, toRow, toColumn);
}

void LedGridWidget::moveToClipboard(int group, int row, int column) {
    Q_UNUSED(group);

    iAnimation.moveLedToClipboard(row, column);
}

void LedGridWidget::addSelectedLeds() {
    SelectableWidget* widget;
    int row;
    int column;

    QList<int> newRows;
    QList<int> newColumns;

    foreach(widget, selectedItems()) {
        toggle(*widget);

        getWidgetPosition(*widget, &row, &column);
        //iAnimation.addNewLed(row, column);
        iUndoStack.push(new AddLedCommand(iAnimation, row, column));

        newRows.append(row);
        newColumns.append(column);
    }

    for(int i = 0; i < newRows.count(); i++) {
        toggle(widgetAt(newRows.at(i), newColumns.at(i)));
    }

    iAnimation.setSaved(false);
}

void LedGridWidget::setSelectedLedsGroupNumber() {
    bool ok;
    int i = QInputDialog::getInt(this, tr("Set led group"),
                                 tr("Group number:"), 1, 1, iAnimation.numGroups() + 1, 1, &ok);
    if (ok) {
        SelectableWidget* widget;
        foreach(widget, selectedItems()) {
            static_cast<LedWidget*>(widget)->led().setGroupNumber(i);
        }
    }
}

void LedGridWidget::deleteSelectedLeds() {
    SelectableWidget* widget;
    foreach(widget, selectedItems()) {
        LedWidget& ledWidget = static_cast<LedWidget&>(*widget);
        Led& led = ledWidget.led();

        iUndoStack.push(new DeleteLedCommand(iAnimation, led.row(), led.column(), led.number()));
        //iAnimation.deleteLed(ledWidget.led().row(), ledWidget.led().column());
    }
}

Led& LedGridWidget::getLed(int row, int column) {
    Led* led = iAnimation.ledAt(row, column);
    if(led == NULL) {
        throw IllegalArgumentException(QString("LedGridWidget::getLed : led at %1, %2 is NULL").arg(row).arg(column));
    }

    return *led;
}

void LedGridWidget::renumberLed(Led& led) {
    bool ok = true;
    int oldNumber = led.number();
    int i = led.number();
    while (ok && iAnimation.ledAt(i) != NULL) {
        i = QInputDialog::getInt(this, tr("Renumber led"),
                                     tr("New number:"), iAnimation.numLeds() + 1, INITIAL_LED, 999, 1, &ok);


        if(ok) {
            if(iAnimation.ledAt(i) != NULL) {
                QMessageBox::critical(this, "Led already exists", QString("There is already an led with number %1, please choose another number").arg(i));
            } else {
                iAnimation.renumberLed(led, i);
                emit renumberLed(led.row(), led.column(), oldNumber);
                break;
            }
        }
    }
}

void LedGridWidget::setCurrentLed(Led& led) {
    if(iCurrentLed != &led) {
        iCurrentLed == &led;
        emit currentLedDetails(led.number(), led.row(), led.column(), led.currentColour());
    }
}

// slots --------------------

void LedGridWidget::addLed(int row, int column, Led* led) {
    if(led == NULL) {
        led = iAnimation.ledAt(row, column);
    }

    LedWidget* widget = new LedWidget(this, iAnimation, *this, *led);

    addWidget(widget, row, column);
}

void LedGridWidget::addSocket(int row, int column) {
    GridItem* item = new GridItem(this, INVALID, row, column);
    addWidget(new SocketWidget(this, *this, *item), row, column);
}

void LedGridWidget::ledDeleted(int row, int column, int ledNumber) {
    qDebug("LedGridWidget::ledDeleted : %d,%d #%d", row, column, ledNumber);
    addSocket(row, column);

    update();

    emit hideLed(ledNumber);
}

void LedGridWidget::ledMoved(int oldRow, int oldColumn, int newRow, int newColumn) {
    qDebug("LedGridWidget::ledMoved : from %d,%d to %d,%d", oldRow, oldColumn, newRow, newColumn);

    addSocket(oldRow, oldColumn);
    addLed(newRow, newColumn, iAnimation.ledAt(newRow, newColumn));
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

// events ---------------------------

void LedGridWidget::mousePressEvent(QMouseEvent* event) {
    qDebug("gridWidget mousePress");
    if (event->button() == Qt::LeftButton &&
       (QApplication::keyboardModifiers() & Qt::ControlModifier) == Qt::ControlModifier) {

        iDragStartPosition = event->pos();
        iDragArea.setRect(0, 0, 0, 0);

        clearSelection();
    }
}

void LedGridWidget::mouseMoveEvent(QMouseEvent* event) {
    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }

   /* iDragArea.setTopLeft(iDragStartPosition);
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
    selectArea(static_cast<ColourWidget&>(*iLedGridLayout->itemAtPosition(positionEndRow, positionEndColumn)->widget()));*/
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
