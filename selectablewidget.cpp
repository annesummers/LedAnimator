#include "selectablewidget.h"

#include <QtGui>
#include <QApplication>

#include "selectablegroupwidget.h"
#include "selectable.h"

using namespace AnimatorUi;

SelectableWidget::SelectableWidget(QWidget *parent, SelectableGroupWidget &selectableGroup, Selectable& item) :
    QWidget(parent),
    iSelectableGroup(selectableGroup),
    iItem(item),
    iDoubleClick(false) {

    setAcceptDrops(true);

    iPasteWrapAction = new QAction(tr("&Paste wrap"), this);
    iPasteWrapAction->setStatusTip(tr("Paste the copied items with wrapping"));

    connect(iPasteWrapAction, SIGNAL(triggered()), this, SLOT(pasteWrap()));

    iPasteTruncateAction = new QAction(tr("&Paste"), this);
    iPasteTruncateAction->setStatusTip(tr("Paste the copied items without wrapping"));

    connect(iPasteTruncateAction, SIGNAL(triggered()), this, SLOT(pasteTruncate()));
}

SelectableWidget::~SelectableWidget() {
   // qDebug("delete widget");
}

QMimeData* SelectableWidget::mimeData() {
    QMimeData *data = new QMimeData;
    data->setData(mimeType(), iSelectableGroup.writeMimeData());

    return data;
}

void SelectableWidget::paste(bool wrap) {
    const QClipboard *clipboard = QApplication::clipboard();

    if(clipboard->mimeData()->hasFormat(mimeType())) {
        bool move = shouldMove();
        iSelectableGroup.handleMimeData(clipboard->mimeData()->data(mimeType()), *this, wrap, move);

        if(move) {
            QApplication::clipboard()->setMimeData(mimeData());
        }
    }
}

Qt::DropAction SelectableWidget::handleDragDropEvent(QDropEvent* event) {
    Qt::DropAction action = Qt::IgnoreAction;
    if (event->mimeData()->hasFormat(mimeType())) {
        if (event->source() !=  NULL &&
            event->source() != this) {
            Qt::KeyboardModifiers modifiers = QApplication::keyboardModifiers();

            if((modifiers & Qt::ControlModifier) == Qt::ControlModifier) {
                action = controlDropAction();
                event->setDropAction(action);
            } else {
                action = defaultDropAction();
                event->setDropAction(action);
            }

            event->accept();
        } else {
            event->ignore();
        }
     } else {
         event->ignore();
     }

     return action;
}

void SelectableWidget::addPasteActions(QMenu* menu) {
    const QClipboard *clipboard = QApplication::clipboard();
    if(clipboard->mimeData()->hasFormat(mimeType())) {
        menu->addAction(iPasteWrapAction);
        menu->addAction(iPasteTruncateAction);
    }
}

// events ----------------------------------------

void SelectableWidget::mousePressEvent(QMouseEvent* event) {
   // qDebug("singleWidget mousePress");
    if (event->button() != Qt::LeftButton) {
        return;
    }

    iDragStartPosition = event->pos();

    if((QApplication::keyboardModifiers() & Qt::ControlModifier) == Qt::ControlModifier &&
        (QApplication::keyboardModifiers() & Qt::ShiftModifier) != Qt::ShiftModifier) {
         if(iSelectableGroup.isAnySelected()) {
             iSelectableGroup.toggle(*this);
         } else {
             iSelectableGroup.toggleOne(*this, false);
         }

         return;
    }
}

void SelectableWidget::mouseReleaseEvent(QMouseEvent* event){
   // qDebug("singleWidget mouseRelease");
    if (event->button() != Qt::LeftButton) {
        return;
    }

    if((QApplication::keyboardModifiers() & Qt::ShiftModifier) == Qt::ShiftModifier) {
        iSelectableGroup.selectArea(*this, (QApplication::keyboardModifiers() & Qt::ControlModifier) != Qt::ControlModifier);
        return;
    }

    if((QApplication::keyboardModifiers() & Qt::ControlModifier) == Qt::ControlModifier) {
        return;
    }

    if(!iDoubleClick) {
        iSelectableGroup.toggleOne(*this);
    } else {
        iDoubleClick = false;
    }
}

void SelectableWidget::mouseMoveEvent(QMouseEvent *event) {
    //qDebug("singelWidge mouse move");
    if(event->buttons() != Qt::LeftButton ||
       (event->pos() - iDragStartPosition).manhattanLength() < QApplication::startDragDistance()) {
        return;
    }

    bool group = iSelectableGroup.isGroupSelected();
    bool multiple = iSelectableGroup.isMultipleSelected();
    if(!group &&
       !multiple) {
        iSelectableGroup.selectOne(*this);
        qDebug("select one on drag");
    } else {
        if(group) {
            qDebug("group selected on drag");
        } else if(multiple) {
            qDebug("multiple selected on drag");
        }
    }

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData());
    drag->setHotSpot(pos());

    if(drag->exec(dragActions()) == Qt::MoveAction) {
        qDebug("move event");
    }
}

void SelectableWidget::mouseDoubleClickEvent(QMouseEvent* event) {
   // qDebug("singleWidget mouseDoubleClick");
    if (event->buttons() != Qt::LeftButton) {
        return;
    }

    iDoubleClick = true;
    iSelectableGroup.selectOne(*this);
}

void SelectableWidget::dragEnterEvent(QDragEnterEvent* event) {
   // qDebug("singleWidget drag enter");
    handleDragDropEvent(event);
}

void SelectableWidget::dragMoveEvent(QDragMoveEvent* event) {
   // qDebug("singleWidget drag move");
    handleDragDropEvent(event);
}

void SelectableWidget::dropEvent(QDropEvent *event) {
   // qDebug("singleWidget drop");
    Qt::DropAction action = handleDragDropEvent(event);
    if(action != Qt::IgnoreAction) {
        iSelectableGroup.handleMimeData(event->mimeData()->data(mimeType()), *this, false, action == Qt::MoveAction);

        update();
    }
}

void SelectableWidget::contextMenuEvent(QContextMenuEvent *) {
    if(!isSelected()) {
        iSelectableGroup.selectOne(*this);
    }
}
