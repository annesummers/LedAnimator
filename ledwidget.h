/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef LEDWIDGETS_H
#define LEDWIDGETS_H

#include <QtGui>

#include "Led.h"
#include "colourwidget.h"
#include "ledgridwidget.h"

namespace AnimatorUi {

class LedWidget : public ColourWidget {
    Q_OBJECT

public:
    explicit LedWidget(QWidget* parent, ColourGroupWidget& ledGroup, Led& led);

    inline Led &led() const { return static_cast<Led&>(iItem); }

    void setValue(FrameValue &value) { led().setTimeAxisCurrentValue(value); }
    const QColor colour() const {return led().currentColour(); }

private slots:
    void renumber();
    void setGroup();

    void deleteLed();
    void update();

    void setAsBackgroundColour();

    void copyTimeAxis();
    void copyFrame();
    void copyValueAxis();

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

    inline Qt::DropActions dropActions() const { return Qt::CopyAction | Qt::MoveAction; }
    inline Qt::DropActions dragActions() const { return Qt::CopyAction | Qt::MoveAction | Qt::LinkAction; }
    inline Qt::DropAction  defaultDropAction() const { return Qt::CopyAction; }
    inline Qt::DropAction  controlDropAction() const { return Qt::MoveAction; }

    void addExtraActions(QMenu* menu);

    inline bool canCut() { return true; }

private:
    LedGridWidget& gridWidget() { return static_cast<LedGridWidget&>(iSelectableGroup); }

    QAction* iRenumberAction;
    QAction* iSetAsBackgroundColourAction;
    QAction* iDeleteLedAction;

    QMenu* iCopyMenu;

    QAction* iCopyTimeAxisAction;
    QAction* iCopyFrameAction;

};
}

#endif // LEDWIDGETS_H
