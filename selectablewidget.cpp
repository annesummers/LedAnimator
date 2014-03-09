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

    iCutAction = new QAction(tr("&Cut"), this);
    iCutAction->setStatusTip(tr("Cut selected"));

    connect(iCutAction, SIGNAL(triggered()), this, SLOT(cut()));

    iCopyAction = new QAction(tr("&Copy"), this);
    iCopyAction->setStatusTip(tr("Copy this colour"));

    connect(iCopyAction, SIGNAL(triggered()), this, SLOT(copy()));

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

void SelectableWidget::cut() {   
    iSelectableGroup.cutSelected();
}

void SelectableWidget::copy() {
    iSelectableGroup.copySelected();
}

void SelectableWidget::paste(bool wrap) {
    iSelectableGroup.paste(*this, wrap);
}

Qt::DropAction SelectableWidget::handleDragDropEvent(QDropEvent* event) {
    Qt::DropAction action = Qt::IgnoreAction;
    if (event->mimeData()->hasFormat(iSelectableGroup.mimeType())) {
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

void SelectableWidget::addCutAction(QMenu* menu) {
    menu->addAction(iCutAction);
}

void SelectableWidget::addPasteActions(QMenu* menu) {
    const QClipboard *clipboard = QApplication::clipboard();
    if(clipboard->mimeData()->hasFormat(iSelectableGroup.mimeType())) {
        menu->addAction(iPasteWrapAction);
        menu->addAction(iPasteTruncateAction);
    }
}

// events ----------------------------------------

void SelectableWidget::mousePressEvent(QMouseEvent* event) {
    //qDebug("singleWidget mousePress");
    if (event->button() != Qt::LeftButton) {
        return;
    }

    iDragStartPosition = event->pos();
}

void SelectableWidget::mouseReleaseEvent(QMouseEvent* event){
    //qDebug("singleWidget mouseRelease");
    if (event->button() != Qt::LeftButton) {
        return;
    }

    if((QApplication::keyboardModifiers() & Qt::ShiftModifier) == Qt::ShiftModifier) {
      iSelectableGroup.selectArea(*this, (QApplication::keyboardModifiers() & Qt::ControlModifier) == Qt::ControlModifier);
      return;
    }

    if((QApplication::keyboardModifiers() & Qt::ControlModifier) == Qt::ControlModifier) {
       if(iSelectableGroup.isAnySelected()) {
           iSelectableGroup.select(*this, !isSelected());
       } else {
           iSelectableGroup.selectOne(*this, false);
       }

       return;
    }

    if(!iDoubleClick) {
        iSelectableGroup.selectOne(*this, false);
    } else {
        iDoubleClick = false;
    }
}

void SelectableWidget::mouseMoveEvent(QMouseEvent *event) {
    //qDebug("singelWidge mouse move");
    if(event->buttons() != Qt::LeftButton ||
       (event->pos() - iDragStartPosition).manhattanLength() < QApplication::startDragDistance()*2) {
        return;
    }

  //  if(iSelectableGroup.isSingleSelected()) {
   //      iSelectableGroup.selectOne(*this);

   // }

   /* bool group = iSelectableGroup.isAreaSelected();
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
    }*/

    QDrag *drag = new QDrag(this);
    if(isSelected()) {
        drag->setMimeData(iSelectableGroup.mimeData(false));
    } else {
        drag->setMimeData(mimeData(false));
    }

    drag->setHotSpot(pos());

    drag->exec(dragActions());
}

QMimeData* SelectableWidget::mimeData(bool cut) {
    QMimeData *data = new QMimeData;
    data->setData(iSelectableGroup.mimeType(), writeMimeData(cut));

    return data;
}

const QByteArray SelectableWidget::writeMimeData(bool cut) {
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    dataStream << 1;  // selected group count
    dataStream << iSelectableGroup.groupNumber();
    dataStream << cut;
    dataStream << 1;  // selected widget count

    Position position = iSelectableGroup.widgetPosition(*this);

    qDebug("WriteMimeData : Adding item at %d,%d", position.row(), position.column());
    dataStream << position.row() << position.column();

    if(cut) {
        iSelectableGroup.moveToClipboard(iSelectableGroup.groupNumber(), position);
    }

    return itemData;
}

void SelectableWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    //qDebug("singleWidget mouseDoubleClick");
    if (event->buttons() != Qt::LeftButton) {
        return;
    }

    iDoubleClick = true;
    iSelectableGroup.selectOne(*this, true);
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
        iSelectableGroup.handleMimeData(event->mimeData()->data(iSelectableGroup.mimeType()), *this, false, action == Qt::MoveAction);

        update();
    }
}

void SelectableWidget::contextMenuEvent(QContextMenuEvent *event) {
   // qDebug("contextMenuevent");

    if(!isSelected()) {
        iSelectableGroup.selectOne(*this, true);
    }

    QMenu menu(this);

    addDefaultAction(&menu);

    if(canCut()) {
        menu.addAction(iCutAction);
    }

    if(canCopy()) {
        menu.addAction(iCopyAction);
    }

    if(QApplication::clipboard()->mimeData()->hasFormat(iSelectableGroup.mimeType())) {
        menu.addAction(iPasteTruncateAction);
        menu.addAction(iPasteWrapAction);
    }

    addExtraActions(&menu);

    menu.exec(event->globalPos());
}

void SelectableWidget::keyPressEvent(QKeyEvent *event) {
    Qt::Key key = (Qt::Key)event->key();

    if(key == Qt::Key_C && event->modifiers() == Qt::ControlModifier) {
        copy();
    } else if(key == Qt::Key_X && event->modifiers() == Qt::ControlModifier) {
        cut();
    } else if (key == Qt::Key_V && event->modifiers() == Qt::ControlModifier) {
        paste(false);
    }
}
