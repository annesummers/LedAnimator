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
#include "renumberleddialog.h"

#include "constants.h"
#include "exceptions.h"

using namespace Exception;
using namespace AnimatorUi;

LedGridWidget::LedGridWidget(QWidget* parent, Animation &animation, ColourGroupGroupWidget &groupGroup) :
    ColourGroupWidget(parent, 0, 0, groupGroup, 0),
    iAnimation(animation),
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

void LedGridWidget::addWidget(SelectableWidget *widget, Position position) {
    QLayoutItem* oldItem = iLedGridLayout->itemAtPosition(position.row(), position.column());

    QWidget* oldWidget = NULL;
    if(oldItem == NULL) {
        iLedGridLayout->addWidget(widget, position.row(), position.column());
    } else {
        oldWidget = oldItem->widget();

        iLedGridLayout->removeWidget(oldWidget);
        oldWidget->hide();

      //  if(selectedItems().count() != 0) {
         //   delete oldWidget;
       //xs }

        iLedGridLayout->addWidget(widget, position.row(), position.column());
    }

    widget->resize(LED_RADIUS*2, LED_RADIUS*2);

    iLedGridLayout->setColumnMinimumWidth(position.column(), widget->width() + 4);
    iLedGridLayout->setRowMinimumHeight(position.row(), widget->height() + 4);

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

    if(numRows() < position.row() + 1) {
        setMaxRow(position.row() + 1);
    }

    if(numColumns() < position.column() + 1) {
        setMaxColumn(position.column() + 1);
    }
}

void LedGridWidget::moveItem(int fromGroup, Position fromPosition, Position toPosition) {
    Q_UNUSED(fromGroup);// there is only one group

    //qDebug("LedGridWidget::moveItem : from %d,%d to %d,%d", fromRow, fromColumn, toRow, toColumn);

    iAnimation.moveLed(fromPosition, toPosition);
}

void LedGridWidget::cloneItem(int fromGroup, Position fromPosition, Position toPosition) {
    Q_UNUSED(fromGroup);// there is only one group
    //qDebug("LedGridWidget::copyItem : from %d,%d to %d,%d", fromPosition.row(), fromPosition.column(),
    //                                                        toPosition.row(), toPosition.column());

    iAnimation.cloneLed(fromPosition, toPosition);
}

void LedGridWidget::pasteItem(int fromGroup, Position fromPosition, Position toPosition) {
    Q_UNUSED(fromGroup);// there is only one group
    //qDebug("LedGridWidget::pasteItem : from %d,%d to %d,%d", fromPosition.row(), fromPosition.column(),
    //                                                        toPosition.row(), toPosition.column());

    iAnimation.pasteLed(fromPosition, toPosition);
}

void LedGridWidget::moveToClipboard(int group, Position position) {
    Q_UNUSED(group);

    iAnimation.moveLedToClipboard(position);
}

void LedGridWidget::addSelectedLeds() {
    SelectableWidget* widget;

    QList<Position> newPositions;

    foreach(widget, selectedItems()) {
        toggle(*widget);

        Position position = widgetPosition(*widget);
        iAnimation.addNewLed(position);

        newPositions.append(position);
    }

    for(int i = 0; i < newPositions.count(); i++) {
        toggle(widgetAt(newPositions.at(i)));
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

        iAnimation.deleteLed(ledWidget.led(), true);
    }
}

Led& LedGridWidget::getLed(Position position) {
    Led* led = iAnimation.ledAt(position);
    if(led == NULL) {
        throw IllegalArgumentException(QString("LedGridWidget::getLed : led at %1, %2 is NULL").arg(position.row()).arg(position.column()));
    }

    return *led;
}

void LedGridWidget::renumberLed(Led& led) {
    int oldNumber = led.number();

    RenumberLedDialog renumberDialog(this, iAnimation);
    if(renumberDialog.exec() == QDialog::Accepted) {
        iAnimation.renumberLed(led.position(), oldNumber, renumberDialog.newNumber());
    }
}

void LedGridWidget::setCurrentLed(Led& led) {
    iCurrentLed = &led;

    emit currentLedDetails(led.number(), led.position().row(), led.position().column(), led.currentColour());
}

// slots --------------------

void LedGridWidget::addLed(int row, int column, Led* led) {
    if(led == NULL) {
        led = iAnimation.ledAt(Position(row, column));
    }

    LedWidget* widget = new LedWidget(this, iAnimation, *this, *led);

    addWidget(widget, Position(row, column));
}

void LedGridWidget::addSocket(int row, int column) {
    GridItem* item = new GridItem(this, iAnimation, INVALID, Position(row, column));
    addWidget(new SocketWidget(this, *this, *item), Position(row, column));
}

void LedGridWidget::ledDeleted(int row, int column, int ledNumber) {
    qDebug("LedGridWidget::ledDeleted : %d,%d #%d", row, column, ledNumber);
    addSocket(row, column);

    update();
}

void LedGridWidget::ledMoved(int oldRow, int oldColumn, int newRow, int newColumn) {
    qDebug("LedGridWidget::ledMoved : from %d,%d to %d,%d", oldRow, oldColumn, newRow, newColumn);

    addSocket(oldRow, oldColumn);
    addLed(newRow, newColumn, iAnimation.ledAt(Position(newRow, newColumn)));
}
/*
void LedGridWidget::toggleLedNumbers() {
    if(iLedNumbersShown) {
        iLedNumbersShown = false;
    } else {
        iLedNumbersShown = true;
    }
}*/


// from ColourGroupWidget ----------------------

Position LedGridWidget::widgetPosition(SelectableWidget &widget) {
    int row;
    int column;
    int rowSpan;
    int columnSpan;

    int index = iLedGridLayout->indexOf(&widget);

    iLedGridLayout->getItemPosition(index, &row, &column, &rowSpan, &columnSpan);

    return Position(row, column);
}

SelectableWidget &LedGridWidget::widgetAt(Position position) {
    SelectableGroupWidget::widgetAt(position);

    return static_cast<SelectableWidget&>(*(iLedGridLayout->itemAtPosition(position.row(), position.column())->widget()));
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

        clearAllSelection();
    }
}

void LedGridWidget::mouseMoveEvent(QMouseEvent* event) {
    if (!(event->buttons() & Qt::LeftButton)) {
        return;
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
