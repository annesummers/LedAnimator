#include "leddetails.h"
#include "ui_leddetails.h"

using namespace AnimatorUi;

LedDetailsWidget::LedDetailsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LedDetailsWidget)
{
    ui->setupUi(this);
}

LedDetailsWidget::~LedDetailsWidget()
{
    delete ui;
}

void LedDetailsWidget::currentLedDetails(int number, int row, int column, QColor colour) {
    ui->ledNumber->setText(QString("Number : %1").arg(number));
    ui->ledRow->setText(QString("%1").arg(row));
    ui->ledColumn->setText(QString("%1").arg(column));
    int hue = colour.hue();
    if(hue == -1) {
        hue = 0;
    }
    ui->ledHue->setText(QString("%1").arg(hue));
    ui->ledSaturation->setText(QString("%1").arg(colour.saturation()));
    ui->ledValue->setText(QString("%1").arg(colour.value()));
}
