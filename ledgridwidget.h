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

//class MainWindow;
class Led;

namespace Ui {

class LedWidget;
class LedGridGroup;

class LedGridWidget : public LedContainerWidget {
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

    void paintEvent(QPaintEvent*);

    int count();

    Led& ledAt(int index);

private:
    int gridWidth();
    int gridHeight();

    Led& ledAtPosition(int x, int y);

    QGridLayout*    iLedGridLayout;

    QPoint          iDragStartPosition;
    QRect           iDragArea;
};

class LedGridContainerWidget : public QWidget {
    Q_OBJECT

public:
    explicit LedGridContainerWidget(QWidget* parent);

protected:
    void paintEvent(QPaintEvent *event);
};
}

#endif // LEDGRID_H
