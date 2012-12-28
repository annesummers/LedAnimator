/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef LEDGRID_H
#define LEDGRID_H

#include <QWidget>
#include <QGridLayout>
#include <QPoint>
#include <QRect>

#include "ledwidgets.h"
#include "selectablegroupwidget.h"

class Led;

namespace Ui {

class LedWidget;
class LedGridGroup;

class LedGridWidget : public SelectableGroupWidget {
    Q_OBJECT

public:
    explicit LedGridWidget(QWidget* parent, const Animation &animation);

private slots:
    void addLed(int row, int column);

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent*);

    void keyPressEvent(QKeyEvent *);

    void paintEvent(QPaintEvent*);

    void getWidgetPosition(SelectableWidget& widget, int* row, int* column);
    SelectableWidget& widgetAt(int row, int column);

private:
    int gridWidth();
    int gridHeight();

    Led& ledAtPosition(int x, int y);

    const Animation&      iAnimation;

    QGridLayout*    iLedGridLayout;
    QWidget*        iContainerWidget;

    QPoint          iDragStartPosition;
    QRect           iDragArea;

};
}

#endif // LEDGRID_H
