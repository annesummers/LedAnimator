#ifndef DRAGGABLEWIDGET_H
#define DRAGGABLEWIDGET_H

#include <QtGui>

#include "selectablewidget.h"

namespace Ui {

class DragDropWidget : public SelectableWidget {
    Q_OBJECT

public:
    DragDropWidget(QWidget* parent, SelectableGroupWidget& groupWidget, Selectable& item);

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent *event);

    void dragEnterEvent(QDragEnterEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dropEvent(QDropEvent *);

    virtual Qt::DropAction dropAction() const = 0;
    virtual const QByteArray dragData() const = 0;
    virtual QString mimeType() const = 0;
    virtual void handleDragData(QByteArray dragData) = 0;

    QPoint    iDragStartPosition;
};

}
#endif // DRAGGABLEWIDGET_H
