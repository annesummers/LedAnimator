#ifndef ANIMATORWIDGET_H
#define ANIMATORWIDGET_H

#include <QWidget>
#include <QMimeData>
#include <QMenu>

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
    inline void selected() { update(); }

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

    void contextMenuEvent(QContextMenuEvent *);

    virtual QString mimeType() const = 0;

    virtual Qt::DropActions dropActions() const = 0;
    virtual Qt::DropActions dragActions() const = 0;
    virtual Qt::DropAction  defaultDropAction() const = 0;
    virtual Qt::DropAction  controlDropAction() const = 0;

    virtual bool shouldMove() { return false; }

    QMimeData* mimeData();

    virtual void addCutAction(QMenu* menu) { Q_UNUSED(menu); }
    void addPasteActions(QMenu* menu);

    void paste(bool wrap);

    QPoint   iDragStartPosition;

    SelectableGroupWidget&  iSelectableGroup;
    Selectable&             iItem;

    bool iDoubleClick;

private:
    Qt::DropAction handleDragDropEvent(QDropEvent *event);

    QAction* iPasteWrapAction;
    QAction* iPasteTruncateAction;
};

}

//Q_DECLARE_METATYPE ( SelectableWidget )
#endif // ANIMATORWIDGET_H
