#include "socketwidget.h"

#include "selectablegroupwidget.h"

using namespace AnimatorUi;

SocketWidget::SocketWidget(QWidget *parent, LedGridWidget& group, GridItem& item) :
    SelectableWidget(parent, group, item),
    iCutLed(NULL) {

    setObjectName("SocketWidget");

    iAddLedAction = new QAction(tr("&Add led"), this);
    iAddLedAction->setStatusTip(tr("Add a new led"));

    connect(iAddLedAction, SIGNAL(triggered()), this, SLOT(addLed()));

    connect(&iItem, SIGNAL(selected()), this, SLOT(selected()));
}

// slots --------------------------------------

void SocketWidget::addLed() {
    gridWidget().addSelectedLeds();
}

void SocketWidget::selected() {
    update();
}

// from SelectableWidget ---------------------

/*void SocketWidget::handleMimeData(QDataStream& dataStream, bool move) {
    QColor colour;

    dataStream >> colour;

    int row;
    int column;

    dataStream >> row >> column;

    if(move) {
        gridWidget().moveLed(row, column, gridItem().row(), gridItem().column());
    } else {
        gridWidget().copyOrMoveLed(row, column, gridItem().row(), gridItem().column());
    }
}

void SocketWidget::writeMimeData(QDataStream& dataStream) {
    dataStream << QColor();
    dataStream << gridItem().row() << gridItem().column();
}
*/
// events -----------------------------------

void SocketWidget::contextMenuEvent(QContextMenuEvent *event) {
    SelectableWidget::contextMenuEvent(event);

    QMenu menu(this);

    menu.addAction(iAddLedAction);
    menu.addSeparator();

    addPasteActions(&menu);

    menu.exec(event->globalPos());
}

void SocketWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    painter.setBrush(painter.background());
    QRect backgroundRect(-1, -1, width() + 1, height() + 1);
    painter.drawRect(backgroundRect);

    painter.setPen(iItem.isSelected() ? Qt::white : Qt::black);
    painter.setBrush(Qt::black);

    int w = width();
    int h = height();

    int extraX = w - 8;
    int extraY = h - 8;

    int originX = extraX/2;
    int originY = extraY/2;

    int bottomX = w - extraX;
    int bottomY = w - extraY;

    QRect rect(originX, originY, bottomX, bottomY);

    painter.drawRect(rect);

    if(iItem.isSelected()) {
        painter.setBrush(Qt::NoBrush);
        painter.setPen(Qt::DashLine);
        painter.drawRect(rect);
    }
}

