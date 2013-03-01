/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "animationdetailswidget.h"

#include "mainwindow.h"
#include "led.h"
#include "animation.h"
#include "framelistwidget.h"

#include "constants.h"
#include "exceptions.h"

using namespace AnimatorUi;
using namespace Exception;

AnimationDetailsWidget::AnimationDetailsWidget(QWidget* parent, Animation &animation) :
    SelectableGroupGroupWidget(parent),
    iAnimation(animation),
    iFramesListX(0),
    iFramesListWidth(0),
    iFrameSlider(NULL),
    iGridLayout(NULL),
    iSignalMapper(NULL),
    iClosed(false){

    QHBoxLayout* horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameStyle(QFrame::NoFrame);

    QWidget* scrollAreaWidgetContents = new QWidget();
    scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
    scrollAreaWidgetContents->setGeometry(QRect(0, 0, 531, 305));

    QHBoxLayout* horizontalLayout_2 = new QHBoxLayout(scrollAreaWidgetContents);
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));

    QVBoxLayout* verticalLayout = new QVBoxLayout();
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

    iGridLayout = new QGridLayout();
    iGridLayout->setObjectName(QString::fromUtf8("iGridLayout"));

    QWidget* widget = new QWidget(scrollAreaWidgetContents);
    widget->setObjectName(QString::fromUtf8("widget"));

    iGridLayout->addWidget(widget, 0, 0, 1, 1);

    iFrameSlider = new QSlider(scrollAreaWidgetContents);
    iFrameSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
    iFrameSlider->setOrientation(Qt::Horizontal);
    iFrameSlider->setMinimum(INITIAL_FRAME);  // frames are indexed from 1
    iFrameSlider->setTickPosition(QSlider::TicksBelow);

    iGridLayout->addWidget(iFrameSlider, 0, 1, 1, 1);

    QWidget* widget_2 = new QWidget(scrollAreaWidgetContents);
    widget_2->setObjectName(QString::fromUtf8("widget_2"));

    iGridLayout->addWidget(widget_2, 0, 2, 1, 1);

    verticalLayout->addLayout(iGridLayout);

    QWidget* widget_6 = new QWidget(scrollAreaWidgetContents);
    widget_6->setObjectName(QString::fromUtf8("widget_6"));

    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(widget_6->sizePolicy().hasHeightForWidth());
    widget_6->setSizePolicy(sizePolicy1);

    verticalLayout->addWidget(widget_6);

    horizontalLayout_2->addLayout(verticalLayout);

    scrollArea->setWidget(scrollAreaWidgetContents);

    horizontalLayout->addWidget(scrollArea);

    iSignalMapper = new QSignalMapper(this);

    setAcceptDrops(true);

    connect(&animation, SIGNAL(numFramesChanged(int)), this, SLOT(numFramesChanged(int)));
    connect(&animation, SIGNAL(currentFrameChanged(int)), this, SLOT(currentFrameChanged(int)));

    connect(iFrameSlider, SIGNAL(valueChanged(int)), &animation, SLOT(setCurrentFrame(int)));
}

void AnimationDetailsWidget::closeClicked(LedDetails &details) {
   // int key = iLedDetails.indexOf(&details);

    int numRows = iGridLayout->rowCount();
    int numColumns = iGridLayout->columnCount();

    qDebug("rows %d, columns %d", numRows, numColumns);
    int index = iGridLayout->indexOf(&details.label());
    int row;
    int column;
    int rowSpan;
    int columnSpan;
    iGridLayout->getItemPosition(index, &row, &column, &rowSpan, &columnSpan);

    for(int i = 0; i < 3; i++) {
        QWidget* widget =  iGridLayout->itemAtPosition(row, i)->widget();
        widget->close();
        iGridLayout->removeWidget(widget);
    }

    iLedDetails.removeOne(&details);
    iShownLeds.remove(details.ledNumber());

    update();
}

bool AnimationDetailsWidget::handleDragDropEvent(QDropEvent* event) {
    if (event->mimeData()->hasFormat(LED_MIME_TYPE)) {
        if (event->source() != 0 &&
            (event->possibleActions() & Qt::LinkAction) == Qt::LinkAction) {
            event->setDropAction(Qt::LinkAction);
            event->accept();

            return true;
         } else {
            event->ignore();

            return false;
        }
     } else {
         event->ignore();

         return false;
     }
}

// slots ---------------------------

void AnimationDetailsWidget::currentFrameChanged(int currentFrame) {
    iFrameSlider->setValue(currentFrame);
    update();
}

void AnimationDetailsWidget::numFramesChanged(int numFrames) {
    iFrameSlider->setMaximum(numFrames);
    iFrameSlider->setTickInterval(numFrames);
}

void AnimationDetailsWidget::addLed(int row, int column) {
    Led* led = iAnimation.ledAt(row, column);

    if(led == NULL) {
        throw IllegalArgumentException("AnimationDetailsWidget::addLed : NULL led");
    }

    if(!iShownLeds.contains(led->number())) {
        int count = iShownLeds.count() + 1;
        qDebug("add new led, %d, %d", row, column);

        QLabel* ledNumberLabel = new QLabel(this);

        FrameListWidget* framesListWidget = new FrameListWidget(this, iAnimation, *led, *this);
       // framesListWidget->resize(40, framesListWidget->height());
        QPushButton* detailsCloseWidget = new QPushButton("X", this);

        iGridLayout->addWidget(ledNumberLabel, count, 0);
        iGridLayout->addWidget(framesListWidget, count, 1);
        iGridLayout->addWidget(detailsCloseWidget, count, 2);

        addGroup(*framesListWidget);

        iShownLeds.insert(led->number(), led);
        iLedDetails.append(new LedDetails(*this, *led, *ledNumberLabel, *detailsCloseWidget));
    }
}

// events -------------------------------------

void AnimationDetailsWidget::paintEvent(QPaintEvent *) {
    int currentFrameIncrements = iFrameSlider->width()/(iAnimation.numFrames());
    int currentFramePosition = (iAnimation.currentFrame() - 1)*currentFrameIncrements;

    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.drawLine(QPoint(currentFramePosition + /*(iFrameSlider->pos().x() + */currentFrameIncrements/2,
                            iFrameSlider->height() - 8),
                     QPoint(currentFramePosition/* + iFrameSlider->pos().x()*/ + currentFrameIncrements/2,
                            height() - iFrameSlider->height() - 8));
}

void AnimationDetailsWidget::resizeEvent(QResizeEvent*) {
    resize(parentWidget()->width() - BORDER*2, parentWidget()->height());
    int numFrames = iAnimation.numFrames();
    int width = iFrameSlider->width();

    if(numFrames > 0) {
        int extra = width%numFrames;

        iFrameSlider->resize(width-extra, iFrameSlider->height());
    }
}

void AnimationDetailsWidget::dragEnterEvent(QDragEnterEvent* event) {
    handleDragDropEvent(event);
}

void AnimationDetailsWidget::dragMoveEvent(QDragMoveEvent* event) {
    handleDragDropEvent(event);
}

void AnimationDetailsWidget::dropEvent(QDropEvent *event) {
    if(handleDragDropEvent(event)) {
        QByteArray itemData = event->mimeData()->data(LED_MIME_TYPE);
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        int row;
        int column;

        int numLeds;

        dataStream >> numLeds;

        int groupNumber;

        dataStream >> groupNumber;

        for(int i = 0; i < numLeds; i++) {
           dataStream >> row >> column;
           addLed(row, column);
        }

        update();
    }
}

// LedDetails ------------------------------

LedDetails::LedDetails(AnimationDetailsWidget& parent, Led &led, QLabel &label, QPushButton &closeButton) :
    QObject(&parent),
    iLed(led),
    iLabel(label),
    iCloseButton(closeButton),
    iDetailsWidget(parent) {

    connect(&closeButton, SIGNAL(clicked()), this, SLOT(closeClicked()));
    connect(&led, SIGNAL(ledUpdated()), this, SLOT(ledUpdated()));

    ledUpdated();
}

void LedDetails::closeClicked() {
    qDebug("closeClicked %d", iLed.number());

    iDetailsWidget.closeClicked(*this);
}

void LedDetails::ledUpdated() {
    iLabel.setText(QString("%1").arg(iLed.number()));
}



