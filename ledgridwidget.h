/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef LEDGRID_H
#define LEDGRID_H

#include <QWidget>
#include <QGridLayout>
#include <QPoint>
#include <QRect>
#include <QUndoStack>

#include "colourgroupwidget.h"

namespace AnimatorModel {
class Led;
class Animation;
}

using namespace AnimatorModel;

namespace AnimatorUi {

class ColourGroupGroupWidget;

class LedGridWidget : public ColourGroupWidget {
    Q_OBJECT

public:
    explicit LedGridWidget(QWidget* parent, Animation &animation, QUndoStack &undoStack, ColourGroupGroupWidget& groupGroup);

    inline bool ledNumbersShown() const { return iLedNumbersShown; }

    void renumberLed(Led& led);

    void addSelectedLeds();
    void hideSelectedLeds();
    void setSelectedLedsGroupNumber();
    void deleteSelectedLeds();
    void moveSelectedLedsToClipboard();

    bool shouldMoveLeds();

    void setCurrentLed(Led& led);

signals:
    void hideLed(int number);
    void renumberLed(int row, int column, int oldNumber);
    void currentLedDetails(int number, int row, int column, QColor colour);

public slots:
    void addLed(int row, int column, Led *led = NULL);

private slots:
    void addSocket(int row, int column);
    void ledDeleted(int row, int column, int ledNumber);
    void ledMoved(int oldRow, int oldColumn, int newRow, int newColumn);

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
    void moveItem(int fromGroup, int fromRow, int fromColumn, int toRow, int toColumn);
    void cloneItem(int fromGroup, int fromRow, int fromColumn, int toRow, int toColumn);
    void pasteItem(int fromGroup, int fromRow, int fromColumn, int toRow, int toColumn);

    void moveToClipboard(int group, int row, int column);

    int gridWidth();
    int gridHeight();

    void addWidget(SelectableWidget *widget, int row, int column);

    Led& getLed(int row, int column);

    Animation&      iAnimation;

    QUndoStack&     iUndoStack;

    QGridLayout*    iLedGridLayout;
    QWidget*        iContainerWidget;

    QPoint          iDragStartPosition;
    QRect           iDragArea;

    Led*            iCurrentLed;

    bool            iLedNumbersShown;
};
}

#endif // LEDGRID_H
