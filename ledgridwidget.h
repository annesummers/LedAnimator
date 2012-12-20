#ifndef LEDGRID_H
#define LEDGRID_H

#include <QWidget>
#include <QGridLayout>
#include <QPoint>
#include <QRect>

#include "ledwidgets.h"

class MainWindow;
class LedWidget;
class Led;
class LedGridGroup;

class LedGridWidget : public LedContainerWidget {
    Q_OBJECT
public:
    explicit LedGridWidget(QWidget* parent, Animation &animation);

signals:

public slots:
    void addLed(Led& led);

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

class LedGridContainerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LedGridContainerWidget(QWidget* parent);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);

private:

};

#endif // LEDGRID_H
