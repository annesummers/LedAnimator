/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef ANIMATORWIDGET_H
#define ANIMATORWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QMimeData>

#include "selectable.h"

using namespace AnimatorModel;

namespace AnimatorUi {

class SelectableGroupWidget;

class SelectableWidget : public QWidget {
    Q_OBJECT

public:
    explicit SelectableWidget(QWidget *parent, SelectableGroupWidget& selectableGroup, Selectable &item);
    virtual ~SelectableWidget();

    inline void select(bool selected) { iItem.select(selected); }
    inline bool isSelected() { return iItem.isSelected(); }

private slots:
    void cut();
    void copy();

    inline void pasteWrap() { paste(true); }
    inline void pasteTruncate() { paste(false); }
    
protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

    void dragEnterEvent(QDragEnterEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dropEvent(QDropEvent *);

    void keyPressEvent(QKeyEvent *event);

    void contextMenuEvent(QContextMenuEvent *event);

    virtual Qt::DropActions dropActions() const = 0;
    virtual Qt::DropActions dragActions() const = 0;
    virtual Qt::DropAction  defaultDropAction() const = 0;
    virtual Qt::DropAction  controlDropAction() const = 0;

    virtual void addExtraActions(QMenu* menu) { Q_UNUSED(menu); }
    virtual void addDefaultAction(QMenu* menu) { Q_UNUSED(menu); }

    virtual inline bool canCut() { return false; }
    virtual inline bool canCopy() { return true; }

    QMimeData* mimeData(bool cut);
    const QByteArray writeMimeData(bool cut);

    void addCutAction(QMenu* menu);
    void addPasteActions(QMenu* menu);

    void paste(bool wrap);

    QPoint   iDragStartPosition;

    SelectableGroupWidget&  iSelectableGroup;
    Selectable&             iItem;

    bool iDoubleClick;

private:
    Qt::DropAction handleDragDropEvent(QDropEvent *event);

    QAction* iCutAction;
    QAction* iCopyAction;

    QAction* iPasteWrapAction;
    QAction* iPasteTruncateAction;
};

}

//Q_DECLARE_METATYPE ( SelectableWidget )
#endif // ANIMATORWIDGET_H
