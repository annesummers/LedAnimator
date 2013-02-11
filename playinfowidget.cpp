/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "playinfowidget.h"
#include "animation.h"

#include "defaults.h"

#include <QtDebug>

using namespace Ui;

PlayInfoWidget::PlayInfoWidget(QWidget* parent, Animation& animation) :
    QWidget(parent),
    iAnimation(animation),
    iPlayButton(NULL),
    iFrameNumberLabel(NULL) {

    iFirstButton = new QPushButton("First", this);
    iFirstButton->move(10, 40);

    iPreviousButton = new QPushButton("Previous", this);
    iPreviousButton->move(iFirstButton->width() + 1, 40);

    iPlayButton = new QPushButton("Play", this);
    iPlayButton->move(iFirstButton->width() +
                      iPreviousButton->width() + 1, 40);

    iNextButton = new QPushButton("Next", this);
    iNextButton->move(iFirstButton->width() +
                      iPreviousButton->width() +
                      iPlayButton->width() + 1, 40);

    iLastButton = new QPushButton("Last", this);
    iLastButton->move(iFirstButton->width() +
                      iPreviousButton->width() +
                      iPlayButton->width() +
                      iNextButton->width() + 1, 40);

    iFrameNumberLabel = new QLabel(this);
    iFrameNumberLabel->move(20, 80);
    iFrameNumberLabel->resize(40, 20);

    QObject::connect(iFirstButton, SIGNAL(clicked()), this, SLOT(firstClicked()));
    QObject::connect(iPreviousButton, SIGNAL(clicked()), this, SLOT(previousClicked()));
    QObject::connect(iPlayButton, SIGNAL(clicked()), this, SLOT(playClicked()));
    QObject::connect(iNextButton, SIGNAL(clicked()), this, SLOT(nextClicked()));
    QObject::connect(iLastButton, SIGNAL(clicked()), this, SLOT(lastClicked()));

    QObject::connect(&animation, SIGNAL(currentFrameChanged(int)), this, SLOT(currentFrameChanged(int)));

    QSizePolicy policy = sizePolicy();
    policy.setHorizontalStretch(0);
    policy.setVerticalStretch(1);
    setSizePolicy(policy);
}

// slots ---------------------------------

void PlayInfoWidget::playClicked() {
    // TODO can we use a state machine here?
    if(!iAnimation.isPlaying()) {
        iAnimation.play();
        iPlayButton->setText("Stop");
    } else {
        iAnimation.stop();
        iPlayButton->setText("Play");
    }
}

void PlayInfoWidget::firstClicked() {
    iAnimation.setCurrentFrame(INITIAL_FRAME);
}

void PlayInfoWidget::previousClicked() {
    iAnimation.setCurrentFrame(iAnimation.currentFrame() - 1);
}

void PlayInfoWidget::nextClicked() {
    iAnimation.setCurrentFrame(iAnimation.currentFrame() + 1);
}

void PlayInfoWidget::lastClicked() {
    iAnimation.setCurrentFrame(iAnimation.numFrames() - 1);
}

void PlayInfoWidget::currentFrameChanged(int currentFrame) {
    iFrameNumberLabel->setText(QString("%1").arg(currentFrame));
}

// ----------------------------------------
