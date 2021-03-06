/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef LEDDETAILSWIDGETS_H
#define LEDDETAILSWIDGETS_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>

#include "ledwidget.h"
#include "colourgroupwidget.h"
#include "colourgroupgroupwidget.h"

namespace AnimatorUi {

class LedPositionWidget;
class FrameListWidget;
class LedDetailsSelectorGroupWidget;

//class LedDetailsWidget;



class LedDetails :  public QObject {
    Q_OBJECT

public:
    explicit LedDetails(QObject* parent,
                        Led& led,
                        QLabel* positionLabel,
                        FrameListWidget* frameList,
                        QPushButton* closeButton) :
        QObject(parent),
        iLed(led),
        iPositionLabel(positionLabel),
        iFrameList(frameList),
        iCloseButton(closeButton) {}

private:
    Led& iLed;
    QLabel* iPositionLabel;
    FrameListWidget* iFrameList;
    QPushButton* iCloseButton;

};

class LedDetailsListWidget : public SelectableGroupGroupWidget {
    Q_OBJECT

public:
    explicit LedDetailsListWidget(QWidget *parent);//, const Animation &animation);
    void setAnimation(Animation* animation);
    void addLed(int row, int column);
   // LedWidget& selectorWidgetAt(int index);

    int count();

    void closeClicked(int whichLed);

protected:
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);

private:
    const Animation*    iAnimation;

    QList<Led*>                     iShownLeds;
    QGridLayout*                    iGridLayout;
   // LedDetailsSelectorGroupWidget*  iSelectorGroup;

    QList<LedDetails*> iLedDetailsList;

    QSlider* iFrameSlider;

    QSignalMapper* iSignalMapper;

    SelectableGroupGroupWidget*         iFramesListGroup;
};

/*class LedDetailsWidget : public QWidget {
    Q_OBJECT

public:
    explicit LedDetailsWidget(QWidget *parent,
                              const Animation &animation,
                              Led &led,
                              ColourGroupWidget& groupWidget,
                              ColourGroupGroupWidget& framesListGroup);

    Led &led();

    LedWidget& selectorWidget() { return *iCurrentFrameWidget; }
    FrameListWidget& framesListWidget() { return *iFramesListWidget; }

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

private slots:
    void framesListResized(int x, int frame);
    void closeClicked();

private:
    QGridLayout*        iLayout;

    QLabel*             iPositionLabel;
    LedWidget*          iCurrentFrameWidget;
    FrameListWidget*    iFramesListWidget;
    QPushButton*        iDetailsCloseWidget;

    int iFramesWidgetWidth;

    Led& iLed;

};

class LedDetailsSelectorGroupWidget : public ColourGroupWidget {
    Q_OBJECT

public:
    explicit LedDetailsSelectorGroupWidget(LedDetailsListWidget &parent);

protected:
    ColourWidget& widgetAt(int row, int column);
    void getWidgetPosition(ColourWidget &widget, int *row, int *column);
    bool validKeyPress(Qt::Key key);

private:
    LedDetailsListWidget& iDetailsList;
};*/
}

#endif // LEDDETAILSWIDGETS_H
