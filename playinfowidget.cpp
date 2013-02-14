/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "playinfowidget.h"
#include "ui_playinfowidget.h"

#include "animation.h"

#include "defaults.h"

#include <QtDebug>

using namespace AnimatorUi;

PlayInfoWidget::PlayInfoWidget(QWidget* parent, Animation& animation) :
    QWidget(parent),
    iAnimation(animation),
    iFirstButton(NULL),
    iPreviousButton(NULL),
    iPlayButton(NULL),
    iNextButton(NULL),
    iLastButton(NULL),
    iFrameNumberLabel(NULL) {

    resize(254, 78);
    setMinimumSize(QSize(275, 100));
    setMaximumSize(QSize(275, 100));

    QGridLayout* gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setMargin(20);

    iFirstButton = new QPushButton("First", this);
    iFirstButton->setObjectName(QString::fromUtf8("iFirstButton"));
    iFirstButton->setMinimumSize(QSize(95, 32));
    iFirstButton->setMaximumSize(QSize(95, 32));

    gridLayout->addWidget(iFirstButton, 0, 0, 1, 1);

    iFrameNumberLabel = new QLabel(this);
    iFrameNumberLabel->setObjectName(QString::fromUtf8("iFrameNumberLabel"));
    iFrameNumberLabel->setMinimumSize(QSize(55, 20));
    iFrameNumberLabel->setMaximumSize(QSize(55, 20));
    iFrameNumberLabel->setAlignment(Qt::AlignCenter);

    gridLayout->addWidget(iFrameNumberLabel, 0, 1, 1, 1);

    iLastButton = new QPushButton("Last", this);
    iLastButton->setObjectName(QString::fromUtf8("iLastButton"));
    iLastButton->setMinimumSize(QSize(72, 32));
    iLastButton->setMaximumSize(QSize(72, 32));

    gridLayout->addWidget(iLastButton, 0, 2, 1, 1);

    iPreviousButton = new QPushButton("Previous", this);
    iPreviousButton->setObjectName(QString::fromUtf8("iPreviousButton"));
    iPreviousButton->setMinimumSize(QSize(95, 32));
    iPreviousButton->setMaximumSize(QSize(95, 32));

    gridLayout->addWidget(iPreviousButton, 1, 0, 1, 1);

    iPlayButton = new QPushButton("Play", this);
    iPlayButton->setObjectName(QString::fromUtf8("iPlayButton"));
    iPlayButton->setMinimumSize(QSize(67, 32));
    iPlayButton->setMaximumSize(QSize(67, 32));

    gridLayout->addWidget(iPlayButton, 1, 1, 1, 1);

    iNextButton = new QPushButton("Next", this);
    iNextButton->setObjectName(QString::fromUtf8("iNextButton"));
    iNextButton->setMinimumSize(QSize(72, 32));
    iNextButton->setMaximumSize(QSize(72, 32));

    gridLayout->addWidget(iNextButton, 1, 2, 1, 1);

    QObject::connect(iFirstButton, SIGNAL(clicked()), this, SLOT(firstClicked()));
    QObject::connect(iPreviousButton, SIGNAL(clicked()), this, SLOT(previousClicked()));
    QObject::connect(iPlayButton, SIGNAL(clicked()), this, SLOT(playClicked()));
    QObject::connect(iNextButton, SIGNAL(clicked()), this, SLOT(nextClicked()));
    QObject::connect(iLastButton, SIGNAL(clicked()), this, SLOT(lastClicked()));

    QObject::connect(&animation, SIGNAL(currentFrameChanged(int)), this, SLOT(currentFrameChanged(int)));
}

// slots ---------------------------------

void PlayInfoWidget::currentFrameChanged(int currentFrame) {
    iFrameNumberLabel->setText(QString("%1").arg(currentFrame));
}

void AnimatorUi::PlayInfoWidget::playClicked() {
    // TODO can we use a state machine here?
    if(!iAnimation.isPlaying()) {
        iAnimation.play();
        iPlayButton->setText("Stop");
    } else {
        iAnimation.stop();
        iPlayButton->setText("Play");
    }
}

void AnimatorUi::PlayInfoWidget::lastClicked() {
    iAnimation.setCurrentFrame(iAnimation.numFrames() - 1);
}

void AnimatorUi::PlayInfoWidget::nextClicked() {
    int frame = iAnimation.currentFrame() - 1;
    if(frame < iAnimation.numFrames()) {
        frame = iAnimation.numFrames();
    }
    iAnimation.setCurrentFrame(frame);
}

void AnimatorUi::PlayInfoWidget::previousClicked() {
    int frame = iAnimation.currentFrame() - 1;
    if(frame < INITIAL_FRAME) {
        frame = INITIAL_FRAME;
    }
    iAnimation.setCurrentFrame(frame);
}

void AnimatorUi::PlayInfoWidget::firstClicked() {
    iAnimation.setCurrentFrame(INITIAL_FRAME);
}
