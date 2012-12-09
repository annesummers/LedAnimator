#include "framedetailswidget.h"

#include "mainwindow.h"

FrameDetailsWidget::FrameDetailsWidget(QWidget* parent, MainWindow& mainWindow) :
    QWidget(parent),
    iMainWindow(mainWindow),
    iFrameSlider(NULL)
{
    iFrameSlider = new QSlider(Qt::Horizontal, this);
    iFrameSlider->move(20, 20);

    QObject::connect(iFrameSlider, SIGNAL(valueChanged(int)), &iMainWindow, SLOT(setCurrentFrame(int)));
}

// slots -----------

void FrameDetailsWidget::currentFrameChanged(int currentFrame) {
    iFrameSlider->setValue(currentFrame);
}

void FrameDetailsWidget::numFramesChanged(int numFrames) {
    iFrameSlider->setMaximum(numFrames);
}

void FrameDetailsWidget::newLed(Led& led) {

}

void FrameDetailsWidget::ledColourChanged(int row, int column, int frame) {

}

// ----------------

// events ---------

void FrameDetailsWidget::resizeEvent(QResizeEvent*) {
    iFrameSlider->resize(width() - 20, iFrameSlider->height());
}

// ----------------
