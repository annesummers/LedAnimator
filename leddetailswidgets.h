/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef LEDDETAILSWIDGETS_H
#define LEDDETAILSWIDGETS_H

#include <QWidget>
#include <QLabel>

#include "ledwidget.h"
#include "selectablegroupwidget.h"

class Led;

namespace Ui {

class LedPositionWidget;
class FrameListWidget;

class LedDetailsListWidget : public SelectableGroupWidget {
    Q_OBJECT
public:
    explicit LedDetailsListWidget(QWidget *parent, const Animation &animation);

    void addLed(int row, int column);

protected:
    SelectableWidget& widgetAt(int row, int column);
    void getWidgetPosition(SelectableWidget &widget, int *row, int *column);

    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);
    Led& ledAt(int index);

    int count();


private:
    const Animation&    iAnimation;

    QList<Led*>         iShownLeds;
    QVBoxLayout*        iLedDetailsList;
};

class LedDetailsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LedDetailsWidget(QWidget *parent, const Animation &animation, Led &led, SelectableGroupWidget& groupWidget);

    Led& led();

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
private slots:
    void framesListResized(int x, int frame);

private:
    QLabel*             iPositionLabel;
    LedWidget*          iCurrentFrameWidget;
    FrameListWidget*    iFramesListWidget;

    int iFramesWidgetWidth;
};
}

#endif // LEDDETAILSWIDGETS_H
