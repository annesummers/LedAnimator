/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef LEDGRID_H
#define LEDGRID_H

#include <QWidget>
#include <QGridLayout>
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
    explicit LedGridWidget(QWidget* parent, Animation &animation, ColourGroupGroupWidget& groupGroup);

    inline bool ledNumbersShown() const { return iLedNumbersShown; }

    void renumberLed(Led& led);

    void addSelectedLeds();
    void hideSelectedLeds();
    void setSelectedLedsGroupNumber();

    void deleteSelectedLeds();
    void moveSelectedLedsToClipboard();

    void copySelectedLedsValueAxis(int axisNum);
    void copySelectedLedsTimeAxis();
    void copySelectedLedsCurrentFrames();

    bool shouldMoveLeds();

    void setCurrentLed(Led& led);

signals:
    void hideLed(int number);
    void renumberLed(int row, int column, int oldNumber);
    void currentLedDetails(int number, int row, int column, QColor colour);

public slots:
    void addLed(int row, int column, Led *led = NULL);
    void copySelected();

private slots:
    void addSocket(int row, int column);
    void ledDeleted(int row, int column, int ledNumber);
    void ledMoved(int oldRow, int oldColumn, int newRow, int newColumn);


protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent*);

    void paintEvent(QPaintEvent*);

    Position widgetPosition(SelectableWidget& widget);
    SelectableWidget& widgetAt(Position position);

    bool validKeyPress(Qt::Key key);

private:
    enum CopyType { EClone, ETimeAxis, EValueAxis, EFrame };

    void moveItem(int fromGroup, Position fromPosition, Position toPosition);
    void cloneItem(int fromGroup, Position fromPosition, Position toPosition);
    void pasteItem(int fromGroup, Position fromPosition, Position toPosition);

    void moveToClipboard(int group, Position position);

    int gridWidth();
    int gridHeight();

    void addWidget(SelectableWidget *widget, Position position);

    Led& getLed(Position position);

    Animation&      iAnimation;

    QGridLayout*    iLedGridLayout;
    QWidget*        iContainerWidget;

    QPoint          iDragStartPosition;
    QRect           iDragArea;

    Led*            iCurrentLed;

    CopyType        iCopyType;
    int             iCopySubType;

    bool            iLedNumbersShown;
};
}

#endif // LEDGRID_H
