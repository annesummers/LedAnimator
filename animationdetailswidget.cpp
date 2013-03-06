/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

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
    ColourGroupGroupWidget(parent),
    iAnimation(animation),
    iFramesListX(0),
    iFramesListWidth(0),
    iFrameSlider(NULL),
    iGridLayout(NULL),
    iClosed(false){

    QHBoxLayout* horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameStyle(QFrame::Box);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    iScrollAreaWidgetContents = new ScrollContentsWidget(this, animation);
    iScrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
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
    iFrameSlider->setPageStep(1);
    iFrameSlider->setSingleStep(1);

    iGridLayout->addWidget(iFrameSlider, 0, 1, 1, 1);

    iCloseAll = new QToolButton(iScrollAreaWidgetContents);
    iCloseAll->setObjectName(QString::fromUtf8("closeAll"));
    iCloseAll->setIcon(QIcon(":/images/delete.png"));
    iCloseAll->setEnabled(false);

    connect(iCloseAll, SIGNAL(clicked()), this, SLOT(closeAllClicked()));

    iGridLayout->addWidget(iCloseAll, 0, 2, 1, 1);

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

    scrollArea->setWidget(iScrollAreaWidgetContents);

    horizontalLayout->addWidget(scrollArea);

    setAcceptDrops(true);

    connect(&animation, SIGNAL(numFramesChanged(int)), this, SLOT(numFramesChanged(int)));
    connect(&animation, SIGNAL(currentFrameChanged(int)), this, SLOT(currentFrameChanged(int)));

    connect(iFrameSlider, SIGNAL(valueChanged(int)), &animation, SLOT(setCurrentFrame(int)));
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

void AnimationDetailsWidget::deleteLed(LedDetails &details) {
    int index = iGridLayout->indexOf(&details.frameList());
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

    iLedDetails.remove(details.ledNumber());

    removeGroup(details.frameList());

    update();

    if(iLedDetails.count() == 0) {
        iCloseAll->setEnabled(false);
    }
}

void AnimationDetailsWidget::doResize() {
    int numFrames = iAnimation.numFrames();
    int width = iFrameSlider->width();
    int extra = width%numFrames;
    int frameWidth = (width-extra)/numFrames;

    iFrameSlider->resize(width-extra+(20 - frameWidth), iFrameSlider->height());

    iScrollAreaWidgetContents->setFramesSize(QSize(width-extra, iFrameSlider->height()));
    iScrollAreaWidgetContents->setFramesPos(QPoint(iFrameSlider->pos().x() + (10 - frameWidth/2), iFrameSlider->pos().y()));
    iScrollAreaWidgetContents->update();
}

// slots ---------------------------

void AnimationDetailsWidget::currentFrameChanged(int currentFrame) {
    iFrameSlider->setValue(currentFrame);

    iScrollAreaWidgetContents->update();
   // update();
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

    if(!iLedDetails.contains(led->number())) {
        int count = iLedDetails.count() + 1;
        //qDebug("add new led, %d, %d", row, column);

        QLabel* ledNumberLabel = new QLabel(this);

        FrameListWidget* framesListWidget = new FrameListWidget(this, iAnimation, *led, *this, count);
        QToolButton* closeButton = new QToolButton(this);
        closeButton->setObjectName(QString::fromUtf8("detailsClose"));
        closeButton->setIcon(QIcon(":/images/delete.png"));

        iGridLayout->addWidget(ledNumberLabel, count, 0);
        iGridLayout->addWidget(framesListWidget, count, 1);
        iGridLayout->addWidget(closeButton, count, 2);

        connect(framesListWidget, SIGNAL(resized()), this, SLOT(framesResized()));

        iLedDetails.insert(led->number(), new LedDetails(*this,
                                                         *led,
                                                         *ledNumberLabel,
                                                         *framesListWidget,
                                                         *closeButton));

        iCloseAll->setEnabled(true);
    }
}

void AnimationDetailsWidget::hideLed(int ledNumber) {
    //qDebug("AnimationDetailsWidget::deleteLed %d", ledNumber);
    if(iLedDetails.contains(ledNumber)) {
        deleteLed(*iLedDetails.value(ledNumber));
    }
}

void AnimationDetailsWidget::renumberLed(int row, int column, int oldNumber) {
    if(iLedDetails.contains(oldNumber)) {
        Led* led = iAnimation.ledAt(row, column);

        if(led == NULL) {
            throw IllegalArgumentException("AnimationDetailsWidget::addLed : NULL led");
        }

        deleteLed(*iLedDetails.value(oldNumber));
        addLed(row, column);
    }
}

void AnimationDetailsWidget::framesResized() {
    doResize();
}

void AnimationDetailsWidget::closeAllClicked() {
    LedDetails* details;
    foreach(details, iLedDetails) {
        deleteLed(*details);
    }
}

// events -------------------------------------

void AnimationDetailsWidget::resizeEvent(QResizeEvent*) {
    doResize();
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

        bool cut;
        int numGroups;
        int groupNumber;

        dataStream >> cut >> numGroups >> groupNumber;

        int numLeds;

        dataStream >> numLeds;

        int row;
        int column;

        for(int i = 0; i < numLeds; i++) {
           dataStream >> row >> column;
           addLed(row, column);
        }

        update();
    }
}

// ScrollContentsWidget -------------------

ScrollContentsWidget::ScrollContentsWidget(QWidget* parent, Animation& animation) :
    QWidget(parent),
    iAnimation(animation),
    iFramesSize(QSize()),
    iFramesPos(QPoint()){}

void ScrollContentsWidget::setFramesSize(QSize size) {
    iFramesSize = size;
}

void ScrollContentsWidget::setFramesPos(QPoint pos) {
    iFramesPos = pos;
}

void ScrollContentsWidget::paintEvent(QPaintEvent *) {
    int frameWidth = iFramesSize.width()/(iAnimation.numFrames());
    int currentFramePosition = (iAnimation.currentFrame() - 1)*frameWidth;

    QPainter painter(this);
    painter.setPen(Qt::black);
    QPoint topPoint(currentFramePosition + iFramesPos.x() + frameWidth/2,
                    iFramesSize.height() - 8);
    QPoint bottomPoint(currentFramePosition + iFramesPos.x() + frameWidth/2,
                       height() - iFramesSize.height() - 8);
    painter.drawLine(topPoint, bottomPoint);
}

// LedDetails ------------------------------

LedDetails::LedDetails(AnimationDetailsWidget& parent, Led &led, QLabel &label, FrameListWidget& framesListWidget, QToolButton &closeButton) :
    QObject(&parent),
    iLed(led),
    iLabel(label),
    iFramesListWidget(framesListWidget),
    iCloseButton(closeButton),
    iDetailsWidget(parent) {

    connect(&closeButton, SIGNAL(clicked()), this, SLOT(closeClicked()));
    connect(&led, SIGNAL(ledUpdated()), this, SLOT(ledUpdated()));

    ledUpdated();
}

void LedDetails::closeClicked() {
    qDebug("closeClicked %d", iLed.number());

    iDetailsWidget.deleteLed(*this);
}

void LedDetails::ledUpdated() {
    iLabel.setText(QString("%1").arg(iLed.number()));
}



