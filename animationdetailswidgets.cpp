/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "animationdetailswidgets.h"
#include "ui_animationdetailswidget.h"

#include "mainwindow.h"
#include "leddetailswidgets.h"
#include "animation.h"
#include "framelistwidget.h"

#include "defaults.h"

using namespace AnimatorUi;

AnimationDetailsWidget::AnimationDetailsWidget(QWidget* parent, Animation &animation) :
    ColourGroupGroupWidget(parent),
    iAnimation(animation),
    iFramesListX(0),
    iFramesListWidth(0),
    iFrameSlider(NULL),
    iGridLayout(NULL),
    iSignalMapper(NULL),
    iClosed(false){

    QHBoxLayout* horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

    QScrollArea* iScrollArea = new QScrollArea(this);
    iScrollArea->setObjectName(QString::fromUtf8("iScrollArea"));
    iScrollArea->setWidgetResizable(true);
    iScrollArea->setFrameStyle(QFrame::NoFrame);

    QWidget* iScrollAreaWidgetContents = new QWidget();
    iScrollAreaWidgetContents->setObjectName(QString::fromUtf8("iScrollAreaWidgetContents"));
    iScrollAreaWidgetContents->setGeometry(QRect(0, 0, 531, 305));

    QHBoxLayout* horizontalLayout_2 = new QHBoxLayout(iScrollAreaWidgetContents);
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));

    QVBoxLayout* verticalLayout = new QVBoxLayout();
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

    iGridLayout = new QGridLayout();
    iGridLayout->setObjectName(QString::fromUtf8("iGridLayout"));

    QWidget* widget = new QWidget(iScrollAreaWidgetContents);
    widget->setObjectName(QString::fromUtf8("widget"));

    iGridLayout->addWidget(widget, 0, 0, 1, 1);

    iFrameSlider = new QSlider(iScrollAreaWidgetContents);
    iFrameSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
    iFrameSlider->setOrientation(Qt::Horizontal);
    iFrameSlider->setMinimum(INITIAL_FRAME);  // frames are indexed from 1
    iFrameSlider->setTickPosition(QSlider::TicksBelow);

    iGridLayout->addWidget(iFrameSlider, 0, 1, 1, 1);

    QWidget* widget_2 = new QWidget(iScrollAreaWidgetContents);
    widget_2->setObjectName(QString::fromUtf8("widget_2"));

    iGridLayout->addWidget(widget_2, 0, 2, 1, 1);

    verticalLayout->addLayout(iGridLayout);

    QWidget* widget_6 = new QWidget(iScrollAreaWidgetContents);
    widget_6->setObjectName(QString::fromUtf8("widget_6"));

    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(widget_6->sizePolicy().hasHeightForWidth());
    widget_6->setSizePolicy(sizePolicy1);

    verticalLayout->addWidget(widget_6);

    horizontalLayout_2->addLayout(verticalLayout);

    iScrollArea->setWidget(iScrollAreaWidgetContents);

    horizontalLayout->addWidget(iScrollArea);

    iSignalMapper = new QSignalMapper(this);

    setAcceptDrops(true);

    connect(&animation, SIGNAL(numFramesChanged(int)), this, SLOT(numFramesChanged(int)));
    connect(&animation, SIGNAL(currentFrameChanged(int)), this, SLOT(currentFrameChanged(int)));

    connect(iFrameSlider, SIGNAL(valueChanged(int)), &animation, SLOT(setCurrentFrame(int)));
}

// slots -----------

void AnimationDetailsWidget::currentFrameChanged(int currentFrame) {
    iFrameSlider->setValue(currentFrame);
    update();
}

void AnimationDetailsWidget::numFramesChanged(int numFrames) {
    iFrameSlider->setMaximum(numFrames);
    iFrameSlider->setTickInterval(numFrames);
}

// TODO this is bollocks
void AnimationDetailsWidget::frameListPosition(int x, int width) {
    iFramesListX = x;
    iFramesListWidth = width;
    int frameIncrements = iFramesListWidth/(iAnimation.numFrames());

   // iFrameSlider->move(x + frameIncrements/2, iFrameSlider->y());
  //  iFrameSlider->resize(width-frameIncrements+frameIncrements/4, iFrameSlider->height());
}

// events ---------

void AnimationDetailsWidget::paintEvent(QPaintEvent *) {
    if(iFramesListWidth > 0) {
        int currentFrameIncrements = iFramesListWidth/(iAnimation.numFrames());
        int currentFramePosition = (iAnimation.currentFrame() - 1)*currentFrameIncrements;

        QPainter painter(this);
        painter.setPen(Qt::black);
        painter.drawLine(QPoint(currentFramePosition + (iFramesListX + currentFrameIncrements/2), iFrameSlider->height() - 8), QPoint(currentFramePosition + iFramesListX + currentFrameIncrements/2 + 2, height() - iFrameSlider->height() - 8));
    }
}

void AnimationDetailsWidget::resizeEvent(QResizeEvent*) {
    resize(parentWidget()->width() - BORDER*2, parentWidget()->height());

    // slider must start from the start of the frames
    //iFrameSlider->resize(width(), SLIDER_HEIGHT);
    //iLedDetailsList->resize(width(), height() - SLIDER_HEIGHT);
}

void AnimationDetailsWidget::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->hasFormat(LED_MIME_TYPE)) {
         if (event->source() != 0) {
             event->setDropAction(Qt::LinkAction);
             event->accept();
         }
     } else {
         event->ignore();
     }
}

void AnimationDetailsWidget::dragMoveEvent(QDragMoveEvent* event) {
    if (event->mimeData()->hasFormat(LED_MIME_TYPE)) {
        if (event->source() != 0) {
            event->setDropAction(Qt::LinkAction);
            event->accept();
        }
    } else {
        event->ignore();
    }
}

void AnimationDetailsWidget::dropEvent(QDropEvent *event) {
     if (event->mimeData()->hasFormat(LED_MIME_TYPE)) {
         QByteArray itemData = event->mimeData()->data(LED_MIME_TYPE);
         QDataStream dataStream(&itemData, QIODevice::ReadOnly);

         if (event->source() != 0) {
             event->setDropAction(Qt::LinkAction);
             event->accept();
         }

         QColor colour;
         int row;
         int column;

         int numRows, numColumns;
         dataStream >> numRows >> numColumns;

         for(int i = 0; i < numRows * numColumns; i++) {
            dataStream >> colour >> row >> column;
            addLed(row, column);
         }

         update();
     } else {
         event->ignore();
     }
 }

void AnimationDetailsWidget::addLed(int row, int column) {
    Led& led = iAnimation.ledAt(row, column);

    if(!iShownLeds.contains(&led)) {
        iShownLeds.append(&led);

        int count = iShownLeds.count();

        QLabel* positionLabel = new QLabel(this);
        positionLabel->setText(QString("%1").arg(count));

        FrameListWidget* framesListWidget = new FrameListWidget(this, iAnimation, led, *this);
        framesListWidget->resize(40, framesListWidget->height());

        QPushButton* detailsCloseWidget = new QPushButton("X", this);

        iGridLayout->addWidget(positionLabel, count, 0);
        iGridLayout->addWidget(framesListWidget, count, 1);
        iGridLayout->addWidget(detailsCloseWidget, count, 2);

        iSignalMapper->setMapping(detailsCloseWidget, count);
        connect(detailsCloseWidget, SIGNAL(clicked()), iSignalMapper, SLOT(map()));
        connect(iSignalMapper, SIGNAL(mapped(int)), this, SLOT(closeClicked(int)));

        connect(detailsCloseWidget, SIGNAL(clicked()), this, SLOT(closeClicked()));

        addGroup(*framesListWidget);

        led.setDetailsShown(count);

        qDebug("add new led, %d, %d", row, column);
    }
}

void AnimationDetailsWidget::closeClicked() {
    qDebug("closeClicked");
    iClosed = false;
}

void AnimationDetailsWidget::closeClicked(int whichLed) {
    if(!iClosed) {
        qDebug("closeClicked %d", whichLed);

        Led* led = iShownLeds.at(whichLed - 1);
        led->setDetailsShown(INVALID);

        int numRows = iGridLayout->rowCount();
        int numColumns = iGridLayout->columnCount();

        qDebug("rows %d, columns %d", numRows, numColumns);

        for(int i = 0; i < 3; i++) {
            QWidget* widget =  iGridLayout->itemAtPosition(whichLed, i)->widget();
            widget->close();
            iGridLayout->removeWidget(widget);
        }

        iShownLeds.removeOne(led);

        update();

        iClosed = true;
    }

    //TODO this doesn't work
}



