#ifndef SOCKETWIDGET_H
#define SOCKETWIDGET_H

#include <QtGui>

#include "selectablewidget.h"
#include "constants.h"
#include "ledgridwidget.h"
#include "griditem.h"

namespace AnimatorUi {

class SocketWidget : public SelectableWidget {
    Q_OBJECT

public:
    SocketWidget(QWidget* parent, LedGridWidget &group, GridItem &item);

    void handleMimeData(QDataStream& dataStream, bool move);
    void writeMimeData(QDataStream& dataStream);

private slots:
    void addLed();
    void selected();

protected:
   void contextMenuEvent(QContextMenuEvent *event);
   void paintEvent(QPaintEvent *);

   inline QString mimeType() const { return LED_MIME_TYPE; }

   inline Qt::DropActions dropActions() const { return Qt::CopyAction | Qt::MoveAction; }
   inline Qt::DropActions dragActions() const { return Qt::IgnoreAction; }
   inline Qt::DropAction  defaultDropAction() const { return Qt::MoveAction; }
   inline Qt::DropAction  controlDropAction() const { return Qt::CopyAction; }

private:
   LedGridWidget& gridWidget() { return static_cast<LedGridWidget&>(iSelectableGroup); }
   GridItem& gridItem() { return static_cast<GridItem&>(iItem); }

   QAction* iAddLedAction;
};
}

#endif // SOCKETWIDGET_H