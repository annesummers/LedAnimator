/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "AxisDetailsWidget.h"

#include "engine.h"
#include "Led.h"
#include "Animation.h"
#include "framelistwidget.h"

#include "constants.h"
#include "exceptions.h"

#include <QScrollArea>
#include <QObject>
#include <QToolButton>


using namespace AnimatorUi;
using namespace Exception;

const int LED_LABEL_WIDTH = 25;

const int SCROLL_AREA_MARGIN = 3;
const int SLIDER_TICK_OFFSET = 8;

AxisDetailsWidget::AxisDetailsWidget(QWidget* parent,
                                     Animation &animation,
                                     Axis &axis,
                                     Engine& engine) :
    ColourGroupGroupWidget(parent, engine),
    iAxis(axis),
    iAnimation(animation),
    iFramesListX(0),
    iFramesListWidth(0),
    iFrameSlider(NULL),
    iGridLayout(NULL) {

    iGridLayout = new QGridLayout();
    iGridLayout->setObjectName(QString::fromUtf8("gridLayout"));

    QSpacerItem* verticalSpacer = new QSpacerItem(20, 115, QSizePolicy::Minimum, QSizePolicy::Expanding);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameStyle(QFrame::Box);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    iScrollAreaWidgetContents = new ScrollContentsWidget(this, animation, axis);
    iScrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
    iScrollAreaWidgetContents->setGeometry(QRect(0, 0, 531, 305));

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(iScrollAreaWidgetContents->sizePolicy().hasHeightForWidth());

    iScrollAreaWidgetContents->setSizePolicy(sizePolicy);
    scrollArea->setWidget(iScrollAreaWidgetContents);

    QVBoxLayout* scrollAreaLayout = new QVBoxLayout(iScrollAreaWidgetContents);
    scrollAreaLayout->setObjectName(QString::fromUtf8("scrollAreaLayout"));

    scrollAreaLayout->addLayout(iGridLayout);
    scrollAreaLayout->addItem(verticalSpacer);

    iGridLayout->setVerticalSpacing(FRAME_SPACING);

    iFrameSlider = new QSlider(this);
    iFrameSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
    iFrameSlider->setOrientation(Qt::Horizontal);
    iFrameSlider->setMinimum(iAxis.lowValue());
    iFrameSlider->setMaximum(iAxis.highValue());
    iFrameSlider->setTickPosition(QSlider::TicksBelow);
    iFrameSlider->setPageStep(1);
    iFrameSlider->setSingleStep(1);

    //iGridLayout->addWidget(iFrameSlider, 0, 1, 1, 1);

    iCloseAll = new QToolButton(this);
    iCloseAll->setObjectName(QString::fromUtf8("closeAll"));
    iCloseAll->setIcon(QIcon(":/images/delete.png"));
    iCloseAll->setEnabled(false);

    connect(iCloseAll, SIGNAL(clicked()), this, SLOT(closeAllClicked()));

   // iGridLayout->addWidget(iCloseAll, 0, 2, 1, 1);

    QHBoxLayout* topHorizontalLayout = new QHBoxLayout();
    topHorizontalLayout->setObjectName(QString::fromUtf8("topHorizontalLayout"));

    iSliderSpacer = new QSpacerItem(LED_LABEL_WIDTH + SCROLL_AREA_MARGIN + SLIDER_TICK_OFFSET, iFrameSlider->height(), QSizePolicy::Fixed, QSizePolicy::Fixed);

    topHorizontalLayout->addWidget(iCloseAll);
    topHorizontalLayout->addItem(iSliderSpacer);

    topHorizontalLayout->addWidget(iFrameSlider);
    topHorizontalLayout->addItem(new QSpacerItem(SCROLL_AREA_MARGIN + SLIDER_TICK_OFFSET, iFrameSlider->height(), QSizePolicy::Fixed, QSizePolicy::Fixed));

    QVBoxLayout* contentsLayout = new QVBoxLayout();
    contentsLayout->setObjectName(QString::fromUtf8("contentsLayout"));

    contentsLayout->addLayout(topHorizontalLayout);
    contentsLayout->addWidget(scrollArea);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QString::fromUtf8("mainLayout"));

    mainLayout->addLayout(contentsLayout);

    setAcceptDrops(true);

    connect(iFrameSlider, SIGNAL(valueChanged(int)), &axis, SLOT(setCurrentFrame(int)));
}

bool AxisDetailsWidget::handleDragDropEvent(QDropEvent* event) {
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

void AxisDetailsWidget::deleteLed(LedDetails &details) {
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
        iScrollAreaWidgetContents->setShowCurrentFrameLine(false);
    }
}

void AxisDetailsWidget::doResize() {
   // qDebug("animationdetails doresize");

    int numFrames = iAxis.numFrames();
    int width = iFrameSlider->width();
    int extra = width%numFrames;
    int frameWidth = (width-extra)/numFrames;

    width = width - extra;

    iSliderSpacer->changeSize(LED_LABEL_WIDTH + SCROLL_AREA_MARGIN + SLIDER_TICK_OFFSET + frameWidth/2, iFrameSlider->height());

//    layout()->invalidate();

    iScrollAreaWidgetContents->setFramesSize(QSize(width, iLedDetails.count()*(FRAME_HEIGHT+FRAME_SPACING)));
    iScrollAreaWidgetContents->setFramesPos(QPoint(iFrameSlider->pos().x() - SLIDER_TICK_OFFSET + SCROLL_AREA_MARGIN, iFrameSlider->pos().y()));
    iScrollAreaWidgetContents->update();

    for(int i = 0; i < iLedDetails.count(); i++) {
        if(iLedDetails.value(i) != NULL) {
            iLedDetails.value(i)->setWidth(width);
        }
    }

    iFrameSlider->resize(width + (SLIDER_TICK_OFFSET*2 - frameWidth), iFrameSlider->height());

}

// slots ---------------------------

void AxisDetailsWidget::currentFrameChanged(int currentFrame) {
    iFrameSlider->setValue(currentFrame);

    iScrollAreaWidgetContents->update();
}

void AxisDetailsWidget::numFramesChanged(int numFrames) {
    iFrameSlider->setMaximum(numFrames);
    iFrameSlider->setTickInterval(numFrames);
}

//void AxisDetailsWidget::framesInserted(int numFrames, int framesAdded) {
 //   numFramesChanged(numFrames);
//}

void AxisDetailsWidget::addLed(int row, int column) {
    Led* led = iAnimation.ledAt(Position(row, column));

    if(led == NULL) {
        throw IllegalArgumentException("AnimationDetailsWidget::addLed : NULL led");
    }

    if(!iLedDetails.contains(led->number())) {
        int count = iLedDetails.count();
        qDebug("add new led, %d, %d", row, column);

        QLabel* ledNumberLabel = new QLabel(this);
        ledNumberLabel->setMaximumWidth(LED_LABEL_WIDTH);
        ledNumberLabel->setMinimumWidth(LED_LABEL_WIDTH);

        FrameListWidget* framesListWidget = new FrameListWidget(this, axisData(*led), *this);//, count);
        QToolButton* closeButton = new QToolButton(this);
        closeButton->setObjectName(QString::fromUtf8("detailsClose"));
        closeButton->setIcon(QIcon(":/images/delete.png"));

        iGridLayout->addWidget(closeButton, count, 0);
        iGridLayout->addWidget(ledNumberLabel, count, 1);
        iGridLayout->addWidget(framesListWidget, count, 2);
       // iGridLayout->setColumnStretch(1, 1);
       // iGridLayout->addWidget(new QWidget(), count, 3);
       // iGridLayout->setColumnStretch(3, 2);

        iLedDetails.insert(led->number(), new LedDetails(*this,
                                                         *led,
                                                         *ledNumberLabel,
                                                         *framesListWidget,
                                                         *closeButton));

        iCloseAll->setEnabled(true);
        iScrollAreaWidgetContents->setShowCurrentFrameLine(true);

        doResize();
        layout()->invalidate();
    }
}

void AxisDetailsWidget::ledDeleted(int row, int column, int ledNumber) {
    Q_UNUSED(row);
    Q_UNUSED(column);
    //qDebug("AnimationDetailsWidget::deleteLed %d", ledNumber);
    if(iLedDetails.contains(ledNumber)) {
        deleteLed(*iLedDetails.value(ledNumber));
    }
}

void AxisDetailsWidget::ledRenumbered(int row, int column, int oldNumber) {
    if(iLedDetails.contains(oldNumber)) {
        Led* led = iAnimation.ledAt(Position(row, column));

        if(led == NULL) {
            throw IllegalArgumentException("AnimationDetailsWidget::addLed : NULL led");
        }

        deleteLed(*iLedDetails.value(oldNumber));
        addLed(row, column);
    }
}

void AxisDetailsWidget::closeAllClicked() {
    LedDetails* details;
    foreach(details, iLedDetails) {
        if(details != NULL) {
            deleteLed(*details);
        }
    }
}

// events -------------------------------------

void AxisDetailsWidget::resizeEvent(QResizeEvent*) {
   // qDebug("animation details resize event");
    doResize();
}

void AxisDetailsWidget::dragEnterEvent(QDragEnterEvent* event) {
    handleDragDropEvent(event);
}

void AxisDetailsWidget::dragMoveEvent(QDragMoveEvent* event) {
    handleDragDropEvent(event);
}

void AxisDetailsWidget::dropEvent(QDropEvent *event) {
    if(handleDragDropEvent(event)) {
        QByteArray itemData = event->mimeData()->data(LED_MIME_TYPE);
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        bool cut;
        int numGroups;
        int groupNumber;

        dataStream >> numGroups >> groupNumber >> cut;

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

ScrollContentsWidget::ScrollContentsWidget(QWidget* parent, Animation& animation, Axis &axis) :
    QWidget(parent),
    iAnimation(animation),
    iAxis(axis),
    iFramesSize(QSize()),
    iFramesPos(QPoint()),
    iShowLine(false) {}

void ScrollContentsWidget::paintEvent(QPaintEvent *) {
    if(iShowLine) {
        int frameWidth = iFramesSize.width()/(iAxis.numFrames());
        int currentFramePosition = (iAxis.currentFrameNum() - iAxis.lowValue())*frameWidth;

        QPainter painter(this);
        painter.setPen(Qt::darkGray);
        QPoint topPoint(currentFramePosition + iFramesPos.x(),
                        iFramesSize.height() - 8);
        QPoint bottomPoint(currentFramePosition + iFramesPos.x(),
                           height() - iFramesSize.height() - 8);
        painter.drawLine(topPoint, bottomPoint);
    }
}

// LedDetails ------------------------------

LedDetails::LedDetails(AxisDetailsWidget& parent, Led &led, QLabel &label, FrameListWidget& framesListWidget, QToolButton &closeButton) :
    QObject(&parent),
    iLed(led),
    iLabel(label),
    iFramesListWidget(framesListWidget),
    iCloseButton(closeButton),
    iDetailsWidget(parent) {

    connect(&closeButton, SIGNAL(clicked()), this, SLOT(closeClicked()));
    connect(&led, SIGNAL(updated()), this, SLOT(ledUpdated()));

    ledUpdated();
}

void LedDetails::closeClicked() {
    qDebug("closeClicked %d", iLed.number());

    iDetailsWidget.deleteLed(*this);
}

void LedDetails::ledUpdated() {
    iLabel.setText(QString("%1").arg(iLed.number()));
}

void LedDetails::setWidth(int width) {
    iFramesListWidget.resize(width, iFramesListWidget.height());
}



