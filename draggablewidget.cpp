#include "draggablewidget.h"

using namespace Ui;

DragDropColourWidget::DragDropColourWidget(QWidget* parent, SelectableGroupWidget& groupWidget, Selectable& item) :
    SelectableWidget(parent, groupWidget, item) {

    setAcceptDrops(true);

    copyAct = new QAction(tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy this colour"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the copied colour"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));
}

void DragDropColourWidget::copy() {
    QClipboard *clipboard = QApplication::clipboard();
    QMimeData *mimeData = new QMimeData;
    mimeData->setData(mimeType(), mimeData());

    clipboard->setMimeData(mimeData);
}

void DragDropColourWidget::paste() {
    const QClipboard *clipboard = QApplication::clipboard();
    handleMimeData(clipboard->mimeData()->data(mimeType()));
}

// events ----------------------------------

void DragDropColourWidget::mousePressEvent(QMouseEvent* event) {
    qDebug("singleWidget mousePress");
    if (event->button() != Qt::LeftButton) {
        return;
    }

    iDragStartPosition = event->pos();

    SelectableWidget::mousePressEvent(event);
}

void DragDropColourWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() != Qt::LeftButton) {
        return;
    }

    if ((event->pos() - iDragStartPosition).manhattanLength()
             < QApplication::startDragDistance()) {
       return;
    }

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(mimeType(), mimeData());

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(pos());

    if (drag->exec(Qt::CopyAction | dropAction()) == Qt::CopyAction | dropAction()) {
     //qDebug(QString("Successfully dragged and dropped led %1,%2").arg(iLed.row()).arg(iLed.column()));
    } else {
     //qDebug(QString("Wrong! led %1,%2").arg(iLed.row()).arg(iLed.column()));
    }
}

void DragDropColourWidget::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->hasFormat(mimeType())) {
        if (event->source() != 0 && event->source() != this) {
            event->setDropAction(Qt::CopyAction);
            event->accept();
         }
     } else {
         event->ignore();
     }
}

void DragDropColourWidget::dragMoveEvent(QDragMoveEvent* event) {
    if (event->mimeData()->hasFormat(mimeType())) {
        if (event->source() != 0 && event->source() != this) {
            event->setDropAction(Qt::CopyAction);
            event->accept();
        }
    } else {
        event->ignore();
    }
}

void DragDropColourWidget::dropEvent(QDropEvent *event) {
     if (event->mimeData()->hasFormat(mimeType())) {

         handleMimeData(event->mimeData()->data(mimeType()));

         if (event->source() != 0 && event->source() != this) {
             event->setDropAction(Qt::CopyAction);
             event->accept();
         }

         update();
     } else {
         event->ignore();
     }
 }

