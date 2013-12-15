#ifndef LEDDETAILS_H
#define LEDDETAILS_H

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
    void currentLedDetails(int number, int row, int column, QColor colour);
    
private:
    Ui::LedDetailsWidget *ui;
};

}
#endif // LEDDETAILS_H
