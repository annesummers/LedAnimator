/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

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
    virtual ~SocketWidget();

private slots:
    void addLed();
    void selected();

protected:
   void paintEvent(QPaintEvent *);
   void keyPressEvent(QKeyEvent *event);
   void mouseMoveEvent(QMouseEvent *event);

   inline Qt::DropActions dropActions() const { return Qt::CopyAction | Qt::MoveAction; }
   inline Qt::DropActions dragActions() const { return Qt::IgnoreAction; }
   inline Qt::DropAction  defaultDropAction() const { return Qt::MoveAction; }
   inline Qt::DropAction  controlDropAction() const { return Qt::CopyAction; }

   void addDefaultAction(QMenu *menu);

   inline bool canCopy() { return false; }
   inline bool canCut() { return false; }

private:
   LedGridWidget& gridWidget() { return static_cast<LedGridWidget&>(iSelectableGroup); }
   GridItem& gridItem() { return static_cast<GridItem&>(iItem); }

   QAction* iAddLedAction;
};
}

#endif // SOCKETWIDGET_H
