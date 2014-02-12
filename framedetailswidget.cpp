#include "FrameDetailsWidget.h"
#include "ui_framedetails.h"

using namespace AnimatorUi;

FrameDetailsWidget::FrameDetailsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameDetailsWidget)
{
    ui->setupUi(this);

    currentFrameChanged(0);
}

FrameDetailsWidget::~FrameDetailsWidget()
{
    delete ui;
}

void FrameDetailsWidget::currentFrameChanged(int currentFrame) {
    ui->currentFrame->setText(QString("%1").arg(currentFrame));
}

void FrameDetailsWidget::highValueChanged(int highValue) {
    ui->numFrames->setText(QString("%1").arg(highValue));
}

//void FrameDetailsWidget::framesInserted(int numFrames, int numFramesAdded) {
  //  numFramesChanged(numFrames);
//}
