/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef LEDGRID_H
#define LEDGRID_H

#include <QWidget>
#include <QGridLayout>
#include <QPoint>
#include <QRect>

//#include "ledwidget.h"
#include "colourgroupwidget.h"

namespace AnimatorModel {
class Led;
class Animation;
}

using namespace AnimatorModel;

namespace AnimatorUi {

class LedGridWidget : public ColourGroupWidget {
    Q_OBJECT

public:
    explicit LedGridWidget(QWidget* parent, Animation &animation);

    inline bool ledNumbersShown() const { return iLedNumbersShown; }

    void copyOrMoveLed(int fromRow, int fromColumn, int toRow, int toColumn);
    void moveLed(int fromRow, int fromColumn, int toRow, int toColumn);
    void deleteLed(int row, int column);

    void addSelectedLeds();
    void hideSelectedLeds();

public slots:
    Led& addLed(int row, int column, Led *led = NULL);

private slots:
    void addSocket(int row, int column);

    void toggleLedNumbers();

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent*);

    void paintEvent(QPaintEvent*);

    void getWidgetPosition(SelectableWidget& widget, int* row, int* column);
    SelectableWidget& widgetAt(int row, int column);

    bool validKeyPress(Qt::Key key);

private:
    void copyLed(Led &led, int toRow, int toColumn);
    void moveLed(Led& led, int toRow, int toColumn);

    void hideLed(int row, int column);

    int gridWidth();
    int gridHeight();

    void deleteIfNeeded(int row, int column);

    void addWidget(SelectableWidget *widget, int row, int column);

    Led& getLed(int row, int column);

    Animation&      iAnimation;

    QGridLayout*    iLedGridLayout;
    QWidget*        iContainerWidget;

    QPoint          iDragStartPosition;
    QRect           iDragArea;

    bool            iLedNumbersShown;

    Led*            iCopyLed;
};
}

#endif // LEDGRID_H
