/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "playinfowidget.h"
#include "ui_playinfowidget.h"

#include "animation.h"

#include "constants.h"

#include <QtDebug>

using namespace AnimatorUi;

PlayInfoWidget::PlayInfoWidget(QWidget* parent, Animation& animation) :
    QWidget(parent),
    iAnimation(animation),
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

    QObject::connect(&animation, SIGNAL(currentFrameChanged(int)), this, SLOT(currentFrameChanged(int)));
    QObject::connect(&animation, SIGNAL(stopped()), this, SLOT(stopped()));
}

// slots ---------------------------------

void PlayInfoWidget::currentFrameChanged(int currentFrame) {
    //iFrameNumberLabel->setText(QString("%1").arg(currentFrame));
    if(currentFrame == INITIAL_FRAME) {
        iFirstButton->setEnabled(false);
    } else if(!iFirstButton->isEnabled()){
        iFirstButton->setEnabled(true);
    }

    if(currentFrame == iAnimation.numFrames()) {
        iLastButton->setEnabled(false);
    } else if (!iLastButton->isEnabled()) {
        iLastButton->setEnabled(true);
    }
}

void PlayInfoWidget::playClicked() {
    // TODO can we use a state machine here?
    if(!iAnimation.isPlaying()) {
        iAnimation.play(iRepeatButton->isChecked());
        iPlayButton->setIcon(QIcon(":/images/pause.png"));
    } else {
        iAnimation.stop();
    }
}

void PlayInfoWidget::lastClicked() {
    iAnimation.setCurrentFrame(iAnimation.numFrames());
}

void PlayInfoWidget::nextClicked() {
    int frame = iAnimation.currentFrame() + 1;
    if(frame > iAnimation.numFrames()) {
        frame = iAnimation.numFrames();
    }

    iAnimation.setCurrentFrame(frame);
}

void PlayInfoWidget::previousClicked() {
    int frame = iAnimation.currentFrame() - 1;
    if(frame < INITIAL_FRAME) {
        frame = INITIAL_FRAME;
    }

    iAnimation.setCurrentFrame(frame);
}

void PlayInfoWidget::firstClicked() {
    iAnimation.setCurrentFrame(INITIAL_FRAME);
}

void PlayInfoWidget::repeatClicked() {

}

void PlayInfoWidget::stopped() {
    iPlayButton->setIcon(QIcon(":/images/play.png"));
}
