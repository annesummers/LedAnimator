#include "FrameDetailsWidget.h"
#include "ui_framedetails.h"

using namespace AnimatorUi;

FrameDetailsWidget::FrameDetailsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameDetailsWidget)
{
    ui->setupUi(this);

    handleCurrentFrameChanged(0);
}

FrameDetailsWidget::~FrameDetailsWidget()
{
    delete ui;
}

void FrameDetailsWidget::handleCurrentFrameChanged(int currentFrame) {
    ui->currentFrame->setText(QString("%1").arg(currentFrame));
}

void FrameDetailsWidget::handleHighValueChanged(int oldHighValue, int highValue) {
    ui->numFrames->setText(QString("%1").arg(highValue));
}

//void FrameDetailsWidget::framesInserted(int numFrames, int numFramesAdded) {
  //  numFramesChanged(numFrames);
//}
