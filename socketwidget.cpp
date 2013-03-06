#include "socketwidget.h"

#include "selectablegroupwidget.h"

using namespace AnimatorUi;

SocketWidget::SocketWidget(QWidget *parent, LedGridWidget& group, GridItem& item) :
    SelectableWidget(parent, group, item),
    iAddLedAction(NULL) {

    setObjectName("SocketWidget");

    iAddLedAction = new QAction(tr("&Add led"), this);
    iAddLedAction->setStatusTip(tr("Add a new led"));

    connect(iAddLedAction, SIGNAL(triggered()), this, SLOT(addLed()));

    connect(&iItem, SIGNAL(selected()), this, SLOT(selected()));
}

SocketWidget::~SocketWidget() {
   // qDebug("delete widget at %d, %d", gridItem().row(), gridItem().column());
}

void SocketWidget::addDefaultAction(QMenu *menu) {
    menu->addAction(iAddLedAction);
    menu->addSeparator();
}

// slots --------------------------------------

void SocketWidget::addLed() {
    gridWidget().addSelectedLeds();
}

void SocketWidget::selected() {
    update();
}

// events -----------------------------------

void SocketWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    painter.setBrush(painter.background());
    QRect backgroundRect(-1, -1, width() + 1, height() + 1);
    painter.drawRect(backgroundRect);

    painter.setPen(iItem.isSelected() ? Qt::white : Qt::black);
    painter.setBrush(Qt::NoBrush);

    int w = width();
    int h = height();

    int extraX = w - 12;
    int extraY = h - 12;

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

void SocketWidget::keyPressEvent(QKeyEvent *event) {
    Qt::Key key = (Qt::Key)event->key();

    if (key == Qt::Key_V && event->modifiers() == Qt::ControlModifier) {
        paste(false);
    }
}
