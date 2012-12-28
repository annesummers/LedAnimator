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

    iPlayButton = new QPushButton("Play", this);
    iPlayButton->move(iFirstButton->width() + 1, 40);

    iFrameNumberLabel = new QLabel(this);
    iFrameNumberLabel->move(20, 80);
    iFrameNumberLabel->resize(40, 20);

    QObject::connect(iPlayButton, SIGNAL(clicked()), this, SLOT(playClicked()));
    QObject::connect(iFirstButton, SIGNAL(clicked()), this, SLOT(firstClicked()));

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

void PlayInfoWidget::currentFrameChanged(int currentFrame) {
    iFrameNumberLabel->setText(QString("%1").arg(currentFrame));
}

// ----------------------------------------
