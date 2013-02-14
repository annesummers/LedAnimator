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

#include "ledwidget.h"
#include "colourgroupwidget.h"

class Led;

namespace AnimatorUi {

class LedWidget;
class LedGridGroup;

class LedGridWidget : public ColourGroupWidget {
    Q_OBJECT

public:
    explicit LedGridWidget(QWidget* parent, const Animation &animation);

private slots:
    void addLed(int row, int column);

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent*);

    //void keyPressEvent(QKeyEvent *);

    void paintEvent(QPaintEvent*);

    void getWidgetPosition(ColourWidget& widget, int* row, int* column);
    ColourWidget& widgetAt(int row, int column);

    bool validKeyPress(Qt::Key key);

private:
    int gridWidth();
    int gridHeight();

    const Animation&      iAnimation;

    QGridLayout*    iLedGridLayout;
    QWidget*        iContainerWidget;

    QPoint          iDragStartPosition;
    QRect           iDragArea;
};
}

#endif // LEDGRID_H
