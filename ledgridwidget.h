#ifndef LEDGRID_H
#define LEDGRID_H

#include <QWidget>
#include <QGridLayout>
#include <QPoint>
#include <QRect>

class MainWindow;
class LedWidget;
class Led;

class LedGridWidget : public QWidget {
    Q_OBJECT
public:
    explicit LedGridWidget(QWidget* parent);

    void clearLedSelection();
    void selectLed(int index, bool select);
    void selectLed(int x, int y, bool select);

    void addWidget(LedWidget& widget, int row, int column);

    int gridWidth();
    int gridHeight();

    void currentFrameChanged(int currentFrame);
    void ledColourChanged(int row, int column, int frame);

signals:

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent*);

    void paintEvent(QPaintEvent*);

private:
    QGridLayout*    iLedGridLayout;

    QPoint  iDragStartPosition;
    QRect   iDragArea;
};

class LedGridContainerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LedGridContainerWidget(QWidget* parent, MainWindow& mainWindow);

    void addLed(Led& led);

signals:

public slots:
    void newLed(Led& led);
    void currentFrameChanged(int currentFrame);
    void ledColourChanged(int row, int column, int frame);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *);

private:
    MainWindow& iMainWindow;

    LedGridWidget*  iLedGridWidget;

    int             iBorder;
};



#endif // LEDGRID_H
