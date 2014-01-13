/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "TimeAxisPlayWidget.h"
//#include "ui_playinfowidget.h"

#include "Animation.h"
#include "TimeAxis.h"

#include "constants.h"

#include <QtDebug>
#include <QGridLayout>

using namespace AnimatorUi;

TimeAxisPlayWidget::TimeAxisPlayWidget(QWidget* parent, Animation& animation, TimeAxis& timeAxis) :
    QWidget(parent),
    iAnimation(animation),
    iTimeAxis(timeAxis),
    iFirstButton(NULL),
    iPreviousButton(NULL),
    iPlayButton(NULL),
    iNextButton(NULL),
    iLastButton(NULL) {//,
    //iFrameNumberLabel(NULL) {

    resize(254, 78);
    setMinimumSize(QSize(275, 100));
    setMaximumSize(QSize(275, 100));

    QGridLayout* gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setMargin(20);

    iFirstButton = new QToolButton(this);
    iFirstButton->setObjectName(QString::fromUtf8("firstButton"));
    iFirstButton->setIcon(QIcon(":/images/previous.png"));
   //// iFirstButton->setMinimumSize(QSize(95, 32));
   // iFirstButton->setMaximumSize(QSize(95, 32));

    gridLayout->addWidget(iFirstButton, 0, 0, 1, 1);

   /* iFrameNumberLabel = new QLabel(this);
    iFrameNumberLabel->setObjectName(QString::fromUtf8("iFrameNumberLabel"));
    iFrameNumberLabel->setMinimumSize(QSize(55, 20));
    iFrameNumberLabel->setMaximumSize(QSize(55, 20));
    iFrameNumberLabel->setAlignment(Qt::AlignCenter);

    gridLayout->addWidget(iFrameNumberLabel, 0, 1, 1, 1);
*/

    iPreviousButton = new QToolButton(this);
    iPreviousButton->setObjectName(QString::fromUtf8("previousButton"));
    iPreviousButton->setIcon(QIcon(":/images/rewind.png"));

    gridLayout->addWidget(iPreviousButton, 0, 1, 1, 1);

    iPlayButton = new QToolButton(this);
    iPlayButton->setObjectName(QString::fromUtf8("playButton"));
    iPlayButton->setIcon(QIcon(":/images/play.png"));

    gridLayout->addWidget(iPlayButton, 0, 2, 1, 1);

    iNextButton = new QToolButton(this);
    iNextButton->setObjectName(QString::fromUtf8("nextButton"));
    iNextButton->setIcon(QIcon(":/images/fast-forward.png"));

    gridLayout->addWidget(iNextButton, 0, 3, 1, 1);

    iLastButton = new QToolButton(this);
    iLastButton->setObjectName(QString::fromUtf8("lastButton"));
    iLastButton->setIcon(QIcon(":/images/next.png"));

    gridLayout->addWidget(iLastButton, 0, 4, 1, 1);

    iRepeatButton = new QToolButton(this);
    iRepeatButton->setObjectName("repeatButton");
    iRepeatButton->setIcon(QIcon(":/images/repeat.png"));
    iRepeatButton->setCheckable(true);

    gridLayout->addWidget(iRepeatButton, 0, 5, 1, 1);

    QObject::connect(iFirstButton, SIGNAL(clicked()), this, SLOT(firstClicked()));
    QObject::connect(iPreviousButton, SIGNAL(clicked()), this, SLOT(previousClicked()));
    QObject::connect(iPlayButton, SIGNAL(clicked()), this, SLOT(playClicked()));
    QObject::connect(iNextButton, SIGNAL(clicked()), this, SLOT(nextClicked()));
    QObject::connect(iLastButton, SIGNAL(clicked()), this, SLOT(lastClicked()));
    QObject::connect(iRepeatButton, SIGNAL(clicked()), this, SLOT(repeatClicked()));

    QObject::connect(&iTimeAxis, SIGNAL(currentFrameChanged(int)), this, SLOT(currentFrameChanged(int)));
    QObject::connect(&iTimeAxis, SIGNAL(stopped()), this, SLOT(stopped()));
}

// slots ---------------------------------

void TimeAxisPlayWidget::currentFrameChanged(int currentFrame) {
    //iFrameNumberLabel->setText(QString("%1").arg(currentFrame));
    if(currentFrame == iTimeAxis.lowValue()) {
        iFirstButton->setEnabled(false);
    } else if(!iFirstButton->isEnabled()){
        iFirstButton->setEnabled(true);
    }

    if(currentFrame == iTimeAxis.highValue()) {
        iLastButton->setEnabled(false);
    } else if (!iLastButton->isEnabled()) {
        iLastButton->setEnabled(true);
    }
}

void TimeAxisPlayWidget::playClicked() {
    // TODO can we use a state machine here?
    if(!iTimeAxis.isPlaying()) {
        iTimeAxis.play(iRepeatButton->isChecked());
        iPlayButton->setIcon(QIcon(":/images/pause.png"));
    } else {
        iTimeAxis.stop();
    }
}

void TimeAxisPlayWidget::lastClicked() {
    iTimeAxis.setCurrentFrame(iTimeAxis.highValue());
}

void TimeAxisPlayWidget::nextClicked() {
    int frame = iTimeAxis.currentFrameNum() + 1;
    if(frame > iTimeAxis.highValue()) {
        frame = iTimeAxis.highValue();
    }

    iTimeAxis.setCurrentFrame(frame);
}

void TimeAxisPlayWidget::previousClicked() {
    int frame = iTimeAxis.currentFrameNum() - 1;
    if(frame < iTimeAxis.lowValue()) {
        frame = iTimeAxis.lowValue();
    }

    iTimeAxis.setCurrentFrame(frame);
}

void TimeAxisPlayWidget::firstClicked() {
    iTimeAxis.setCurrentFrame(iTimeAxis.lowValue());
}

void TimeAxisPlayWidget::repeatClicked() {
    iTimeAxis.setRepeating(iTimeAxis.isRepeating());
}

void TimeAxisPlayWidget::stopped() {
    iPlayButton->setIcon(QIcon(":/images/play.png"));
}
