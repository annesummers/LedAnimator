#ifndef LEDDETAILS_H
#define LEDDETAILS_H

//#include <QtWidgets>
#include <QWidget>

namespace Ui {
class LedDetailsWidget;
}

namespace AnimatorUi {

class LedDetailsWidget : public QWidget {
    Q_OBJECT
    
public:
    explicit LedDetailsWidget(QWidget *parent = 0);
    ~LedDetailsWidget();

private slots:
    void handleCurrentLedChanged(int number, QColor colour);
    void handleCurrentPositionChanged(int row, int column);
    
private:
    Ui::LedDetailsWidget *ui;
};

}
#endif // LEDDETAILS_H
