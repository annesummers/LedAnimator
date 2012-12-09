#include "playinfowidget.h"

#include "mainwindow.h"

#include <QtDebug>

PlayInfoWidget::PlayInfoWidget(QWidget* parent, MainWindow& mainWindow) :
    QWidget(parent),
    iMainWindow(mainWindow),
    iPlayButton(NULL),
    iFrameNumberLabel(NULL) {

    iFirstButton = new QPushButton("First", this);
    iFirstButton->move(10, 40);

    iPlayButton = new QPushButton("Play", this);
    iPlayButton->move(iFirstButton->width() + 1, 40);

    iFrameNumberLabel = new QLabel(this);
    iFrameNumberLabel->move(20, 80);
    iFrameNumberLabel->resize(40, 20);

    QObject::connect(iPlayButton, SIGNAL(clicked()), &iMainWindow, SLOT(playClicked()));
    QObject::connect(iPlayButton, SIGNAL(clicked()), this, SLOT(playClicked()));

    QObject::connect(iFirstButton, SIGNAL(clicked()), this, SLOT(firstClicked()));

    QSizePolicy policy = sizePolicy();
    policy.setHorizontalStretch(0);
    policy.setVerticalStretch(1);
    setSizePolicy(policy);
}

// slots ---------------------------------

void PlayInfoWidget::playClicked() {
    if(iPlayButton->text() == "Play") {
        iPlayButton->setText("Stop");
    } else {
        iPlayButton->setText("Play");
    }
}

void PlayInfoWidget::firstClicked() {
    iMainWindow.setCurrentFrame(0);
}

void PlayInfoWidget::currentFrameChanged(int currentFrame) {
    iFrameNumberLabel->setText(QString("%1").arg(currentFrame));
}

// ----------------------------------------
