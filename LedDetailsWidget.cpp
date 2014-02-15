#include "LedDetailsWidget.h"
#include "ui_leddetailswidget.h"

using namespace AnimatorUi;

LedDetailsWidget::LedDetailsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LedDetailsWidget) {
    ui->setupUi(this);

    setWindowTitle(QString("Led Details"));
}

LedDetailsWidget::~LedDetailsWidget() {
    delete ui;
}

void LedDetailsWidget::handleCurrentLedChanged(int number, QColor colour) {
    ui->ledNumber->setText(QString("Number : %1").arg(number));
    int hue = colour.hue();
    if(hue == -1) {
        hue = 0;
    }
    ui->ledHue->setText(QString("%1").arg(hue));
    ui->ledSaturation->setText(QString("%1").arg(colour.saturation()));
    ui->ledValue->setText(QString("%1").arg(colour.value()));
}

void LedDetailsWidget::handleCurrentPositionChanged(int row, int column) {
    ui->ledNumber->setText(QString(""));
    ui->ledRow->setText(QString("%1").arg(row));
    ui->ledColumn->setText(QString("%1").arg(column));
    ui->ledHue->setText(QString(""));
    ui->ledSaturation->setText(QString(""));
    ui->ledValue->setText(QString(""));
}
