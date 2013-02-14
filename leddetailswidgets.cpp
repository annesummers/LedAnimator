/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "leddetailswidgets.h"

#include "ledwidget.h"
#include "framelistwidget.h"
#include "animationdetailswidgets.h"

#include "exceptions.h"

using namespace AnimatorUi;
using namespace Exception;
/*
LedDetailsWidget::LedDetailsWidget(QWidget *parent,
                                   const Animation &animation,
                                   Led& led,
                                   ColourGroupWidget& ledGroup,
                                   ColourGroupGroupWidget &framesListGroup) :
    QWidget(parent),
    iPositionLabel(NULL),
    iCurrentFrameWidget(NULL),
    iFramesListWidget(NULL),
    iLed(led){

   // iLayout = new QHBoxLayout(this);
    iLayout = new QGridLayout(this);

    iFrameSlider = new QSlider(iScrollAreaWidgetContents);
    iFrameSlider->setObjectName(QString::fromUtf8("iFrameSlider"));
    iFrameSlider->setOrientation(Qt::Horizontal);
    iFrameSlider->setMinimum(INITIAL_FRAME);  // frames are indexed from 1
  //  iFrameSlider->resize(parent->width() - BORDER*2, SLIDER_HEIGHT);
    iFrameSlider->setTickPosition(QSlider::TicksBelow);
    iLayout->addWidget(iPositionLabel, 0, 1);

    iPositionLabel = new QLabel(this);
    iPositionLabel->setText(QString("(%1, %2)").arg(led.row()).arg(led.column()));
    iLayout->addWidget(iPositionLabel, 1, 0);
    //iPositionLabel->move(5, 10);

   // iCurrentFrameWidget = new LedWidget(this, animation, ledGroup, led);
   // iCurrentFrameWidget->move(50, 10);
   // iCurrentFrameWidget->resize(40, 40);
  //  iLayout->addWidget(iCurrentFrameWidget, 0);

    iFramesListWidget = new FrameListWidget(this, animation, led, framesListGroup);
    iFramesListWidget->resize(40, iFramesListWidget->height());
    iLayout->addWidget(iFramesListWidget, 1, 1);
   // iFramesListWidget->move(iCurrentFrameWidget->width() + 60, 5);

    iDetailsCloseWidget = new QPushButton("X", this);
    iLayout->addWidget(iDetailsCloseWidget, 0, Qt::AlignRight);
  //  iDetailsCloseWidget->move(iFramesListWidget->rect().bottomRight().x() + 10, iDetailsCloseWidget->pos().y());

    setLayout(iLayout);
    connect(iFramesListWidget, SIGNAL(resized(int,int)), this, SLOT(framesListResized(int, int)));
    connect(iDetailsCloseWidget, SIGNAL(clicked()), this, SLOT(closeClicked()));
}

Led& LedDetailsWidget::led(){
    return iLed;
}

void LedDetailsWidget::framesListResized(int x, int width) {
    //static_cast<AnimationDetailsWidget*>(parentWidget()->parentWidget())->frameListPosition(x, width);

}

void LedDetailsWidget::closeClicked() {
    static_cast<LedDetailsListWidget*>(parentWidget())->closeDetails(*this);
    close();
}

void LedDetailsWidget::paintEvent(QPaintEvent*) {

    QPainter painter(this);
    painter.setBrush(Qt::magenta);
    painter.drawRect(0, 0, width(), height());
}

void LedDetailsWidget::resizeEvent(QResizeEvent *) {
    //iFramesListWidget->resize(width() - iPositionLabel->width() - iCurrentFrameWidget->width(), iFramesListWidget->height());
}*/

// ------------------------------------

/*LedDetailsSelectorGroupWidget::LedDetailsSelectorGroupWidget(LedDetailsListWidget& parent) :
    ColourGroupWidget(&parent, 0, 0),
    iDetailsList(parent){
}

ColourWidget& LedDetailsSelectorGroupWidget::widgetAt(int row, int column) {
    if(column != 0) {
        throw new IllegalArgumentException("Column should be zero");
    }

    return iDetailsList.selectorWidgetAt(row);
    //return static_cast<SelectableWidget&>(*iDetailsList.itemAt(row)->widget()->childAt(1, 0));
}

void LedDetailsSelectorGroupWidget::getWidgetPosition(ColourWidget &widget, int *row, int *column) {
    Q_UNUSED(widget)
    column = 0;
    row = 0;
*/
   /* for(int i = 0; i < iDetailsList.count(); i++) {
        if(widgetAt(i, 0). == widget) {
            *row = i;
            return;
        }
    }*/
/*}

bool LedDetailsSelectorGroupWidget::validKeyPress(Qt::Key key) {
    Q_UNUSED(key);
    return false;
}
*/
// ------------------------------------

LedDetailsListWidget::LedDetailsListWidget(QWidget *parent) ://, const Animation &animation) :
    ColourGroupGroupWidget(parent),
    iAnimation(NULL) {

   // iLedDetailsList = new QVBoxLayout(this);
   // setLayout(iLedDetailsList);

    //iSelectorGroup = new LedDetailsSelectorGroupWidget(*this);
    iFramesListGroup = new ColourGroupGroupWidget(this);


    iGridLayout = new QGridLayout(this);

    QWidget* widget1 = new QWidget(this);
    iGridLayout->addWidget(widget1, 0, 0);

    iFrameSlider = new QSlider(this);
    iFrameSlider->setObjectName(QString::fromUtf8("iFrameSlider"));
    iFrameSlider->setOrientation(Qt::Horizontal);
    iFrameSlider->setMinimum(INITIAL_FRAME);  // frames are indexed from 1
  //  iFrameSlider->resize(parent->width() - BORDER*2, SLIDER_HEIGHT);
    iFrameSlider->setTickPosition(QSlider::TicksBelow);
    iGridLayout->addWidget(iFrameSlider, 0, 1);


    QWidget* widget2 = new QWidget(this);
    iGridLayout->addWidget(widget2, 0, 2);


    iSignalMapper = new QSignalMapper(this);
}

void LedDetailsListWidget::setAnimation(Animation* animation) {
    iAnimation = animation;

}

void LedDetailsListWidget::addLed(int row, int column) {
    Led& led = iAnimation->ledAt(row, column);

    if(!iShownLeds.contains(&led)) {
        iShownLeds.append(&led);

        //LedDetailsWidget* ledDetails = new LedDetailsWidget(this, *iAnimation, led, *iSelectorGroup, *iFramesListGroup);
        QLabel* positionLabel = new QLabel(this);
        positionLabel->setText(QString("(%1, %2)").arg(led.row()).arg(led.column()));

        FrameListWidget* framesListWidget = new FrameListWidget(this, *iAnimation, led, *iFramesListGroup);
        framesListWidget->resize(40, framesListWidget->height());

        QPushButton* detailsCloseWidget = new QPushButton("X", this);
        detailsCloseWidget->setObjectName(QString("%1").arg(iLedDetailsList.count()));


        iSignalMapper->setMapping(detailsCloseWidget, iLedDetailsList.count());
        connect(detailsCloseWidget, SIGNAL(clicked()), iSignalMapper, SLOT(map()));

        connect(iSignalMapper, SIGNAL(mapped(int)), this, SLOT(closeClicked(int)));

        LedDetails* ledDetails = new LedDetails(this, led, positionLabel, framesListWidget, detailsCloseWidget);

        iLedDetailsList.append(ledDetails);

        //ledDetails->setMaximumHeight(40);
        //ledDetails->setMinimumHeight(40);

        int count = iLedDetailsList.count();

        setMinimumHeight(count * 50);
        setMaximumHeight(count * 50);

        //iSelectorGroup->setMaxColumn(count);

        addGroup(*framesListWidget);

        iGridLayout->addWidget(positionLabel, count, 0);
        iGridLayout->addWidget(framesListWidget, count, 1);
        iGridLayout->addWidget(detailsCloseWidget, count, 2);

        qDebug("add new led, %d, %d", row, column);
    }
}
/*
void LedDetailsListWidget::closeClicked(int whichLed) {
    iShownLeds.removeAt(whichLed);

    for(int i = 0; i < 3; i++) {
        iGridLayout->removeWidget(iGridLayout->itemAtPosition(i, iLedDetailsList.count())->widget());
    }

    iLedDetailsList.removeAt(whichLed);


}*/

// events -------------------------------------

void LedDetailsListWidget::resizeEvent(QResizeEvent *) {
}

void LedDetailsListWidget::paintEvent(QPaintEvent *) {
   /* QPainter painter(this);
    painter.setBrush(Qt::cyan);
    painter.drawRect(0, 0, width(), height());*/
}
