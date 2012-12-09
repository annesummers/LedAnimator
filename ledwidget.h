#ifndef LEDWIDGET_H
#define LEDWIDGET_H

#include <QtGui>

#include "led.h"
#include "ledgridwidget.h"

class LedWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LedWidget(LedGridWidget& parent, Led& led);

    Led& led();
    
signals:
    
public slots:

protected:
    void paintEvent(QPaintEvent* event);
    void mouseDoubleClickEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent* event);

private:
    LedGridWidget& iLedGridWidget;
    Led& iLed;
};

#endif // LEDWIDGET_H
