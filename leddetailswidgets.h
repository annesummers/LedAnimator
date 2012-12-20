#ifndef LEDDETAILSWIDGETS_H
#define LEDDETAILSWIDGETS_H

#include <QWidget>
#include <QLabel>

#include "ledwidgets.h"

class LedPositionWidget;
class FrameListWidget;
class Led;
class LedWidget;

class LedDetailsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LedDetailsWidget(QWidget *parent, Animation &animation, Led &led, LedContainerWidget& groupWidget);
    
    Led& led();

signals:
    
public slots:
  //  void numFramesChanged(int numFrames);

protected:
    void resizeEvent(QResizeEvent *);

private:
    QLabel*             iPositionLabel;
    LedWidget*          iCurrentFrameWidget;
    FrameListWidget*    iFramesListWidget;

    int iFramesWidgetWidth;
};

class LedDetailsListWidget : public LedContainerWidget
{
    Q_OBJECT
public:
    explicit LedDetailsListWidget(QWidget *parent, Animation &animation);

    void addLed(Led& led);

signals:

public slots:

protected:
    void resizeEvent(QResizeEvent *);
    Led& ledAt(int index);

    int count();

private:
    QList<Led*>         iShownLeds;
    QVBoxLayout*        iLedDetailsList;
};

#endif // LEDDETAILSWIDGETS_H
