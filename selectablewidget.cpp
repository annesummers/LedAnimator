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

QMimeData* SelectableWidget::mimeData(bool cut) {
    QMimeData *data = new QMimeData;
    data->setData(mimeType(), iSelectableGroup.writeMimeData(cut));

    return data;
}

void SelectableWidget::cut() {
    clearClipboard();

    QApplication::clipboard()->setMimeData(mimeData(true));
}

void SelectableWidget::copy() {
    clearClipboard();

    QApplication::clipboard()->setMimeData(mimeData(false));
}

void SelectableWidget::paste(bool wrap) {
    const QClipboard *clipboard = QApplication::clipboard();

    if(clipboard->mimeData()->hasFormat(mimeType())) {
        bool wasCut = iSelectableGroup.handleMimeData(clipboard->mimeData()->data(mimeType()), *this, wrap);//, move);

        if(wasCut) {
            QApplication::clipboard()->setMimeData(mimeData(false));
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

void SelectableWidget::addCutAction(QMenu* menu) {
    menu->addAction(iCutAction);
}

void SelectableWidget::addPasteActions(QMenu* menu) {
    const QClipboard *clipboard = QApplication::clipboard();
    if(clipboard->mimeData()->hasFormat(mimeType())) {
        menu->addAction(iPasteWrapAction);
        menu->addAction(iPasteTruncateAction);
    }
}

void SelectableWidget::clearClipboard() {
    const QClipboard *clipboard = QApplication::clipboard();

    if(clipboard->mimeData()->hasFormat(mimeType())) {
        QApplication::clipboard()->clear();
    }
}

// events ----------------------------------------

void SelectableWidget::mousePressEvent(QMouseEvent* event) {
    qDebug("singleWidget mousePress");
    if (event->button() != Qt::LeftButton) {
        return;
    }

    iDragStartPosition = event->pos();

    if((QApplication::keyboardModifiers() & Qt::ControlModifier) == Qt::ControlModifier) {
        qDebug("mousepress : control modifier");
    }

    if((QApplication::keyboardModifiers() & Qt::ShiftModifier) == Qt::ShiftModifier) {
        qDebug("mousepress : shift modifier");
    }

    if((QApplication::keyboardModifiers() & Qt::MetaModifier) == Qt::MetaModifier) {
        qDebug("mousepress : meta modifier");
    }

   /* if((QApplication::keyboardModifiers() & Qt::ControlModifier) == Qt::ControlModifier &&
        (QApplication::keyboardModifiers() & Qt::ShiftModifier) != Qt::ShiftModifier) {
         if(iSelectableGroup.isAnySelected()) {
             iSelectableGroup.toggle(*this);
         } else {
             iSelectableGroup.toggleOne(*this, false);
         }

         return;
    }*/

  /*  if((QApplication::keyboardModifiers() & Qt::ShiftModifier) == Qt::ShiftModifier) {
        iSelectableGroup.selectArea(*this, (QApplication::keyboardModifiers() & Qt::ControlModifier) != Qt::ControlModifier);
        return;
    }

    if((QApplication::keyboardModifiers() & Qt::ControlModifier) == Qt::ControlModifier &&
        (QApplication::keyboardModifiers() & Qt::ShiftModifier) != Qt::ShiftModifier) {
         if(iSelectableGroup.isAnySelected()) {
             iSelectableGroup.toggle(*this);
         } else {
             iSelectableGroup.toggleOne(*this, false);
         }

         return;
    }

    if(!iDoubleClick) {
        iSelectableGroup.toggleOne(*this);
    } else {
        iDoubleClick = false;
    }*/
}

void SelectableWidget::mouseReleaseEvent(QMouseEvent* event){
    qDebug("singleWidget mouseRelease");
    if (event->button() == Qt::RightButton) {
        return;
    }

    if(event->button() == Qt::LeftButton) {
        if((QApplication::keyboardModifiers() & Qt::ControlModifier) == Qt::ControlModifier) {
            qDebug("mouseRelease : control modifier");
        }

        if((QApplication::keyboardModifiers() & Qt::ShiftModifier) == Qt::ShiftModifier) {
            qDebug("mouseRelease : shift modifier");
        }

        if((QApplication::keyboardModifiers() & Qt::MetaModifier) == Qt::MetaModifier) {
            qDebug("mouseRelease : meta modifier");
        }

        if((QApplication::keyboardModifiers() & Qt::ShiftModifier) == Qt::ShiftModifier) {
            iSelectableGroup.selectArea(*this, (QApplication::keyboardModifiers() & Qt::ControlModifier) == Qt::ControlModifier);
            return;
        }

        if((QApplication::keyboardModifiers() & Qt::ControlModifier) == Qt::ControlModifier &&
            (QApplication::keyboardModifiers() & Qt::ShiftModifier) != Qt::ShiftModifier) {
             if(iSelectableGroup.isAnySelected()) {
                 iSelectableGroup.toggle(*this);
             } else {
                 iSelectableGroup.toggleOne(*this, false);
             }

             return;
        }

        if(!iDoubleClick) {
            iSelectableGroup.toggleOne(*this);
        } else {
            iDoubleClick = false;
        }
    }
}

void SelectableWidget::mouseMoveEvent(QMouseEvent *event) {
    //qDebug("singelWidge mouse move");
    if(event->buttons() != Qt::LeftButton ||
       (event->pos() - iDragStartPosition).manhattanLength() < QApplication::startDragDistance()*2) {
        return;
    }

    bool group = iSelectableGroup.isAreaSelected();
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
    drag->setMimeData(mimeData(false));
    drag->setHotSpot(pos());

    drag->exec(dragActions());
}

void SelectableWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    qDebug("singleWidget mouseDoubleClick");
    if (event->buttons() != Qt::LeftButton) {
        return;
    }

  //  iDoubleClick = true;
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

void SelectableWidget::contextMenuEvent(QContextMenuEvent *event) {
    qDebug("contextMenuevent");

    if(!isSelected()) {
        iSelectableGroup.selectOne(*this);
    }

    QMenu menu(this);

    addDefaultAction(&menu);

    if(canCut()) {
        menu.addAction(iCutAction);
    }

    if(canCopy()) {
        menu.addAction(iCopyAction);

        if(QApplication::clipboard()->mimeData()->hasFormat(mimeType())) {
            menu.addAction(iPasteTruncateAction);
            menu.addAction(iPasteWrapAction);
        }
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
