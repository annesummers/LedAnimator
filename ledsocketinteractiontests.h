#ifndef LEDSOCKETINTERACTIONTESTS_H
#define LEDSOCKETINTERACTIONTESTS_H

#include <QObject>
#include <QtTest/QTest>

#include "ledgridwidget.h"

namespace AnimatorModel {
class Animation;
}

namespace AnimatorUi {
class SelectableWidget;
}

using namespace AnimatorModel;
using namespace AnimatorUi;

class TestLedGridWidget : public LedGridWidget {
    Q_OBJECT

public:
    explicit TestLedGridWidget(QWidget* parent, Animation &animation) :
        LedGridWidget(parent, animation) {}

    SelectableWidget& widgetAt(int row, int column) { return LedGridWidget::widgetAt(row, column); }
};

class LedSocketInteractionTests : public QObject {
    Q_OBJECT

public:
    explicit LedSocketInteractionTests(QObject *parent = 0);
    
private slots:
    void copyPasteOneLedToSocket_data();
    void copyPasteOneLedToSocket();

    void copyPasteManyLedToSocket_data();
    void copyPasteManyLedToSocket();

    void cutPasteOneLedToSocket_data();
    void cutPasteOneLedToSocket();

    void cutPasteManyLedToSocket_data();
    void cutPasteManyLedToSocket();

private:
    void setupAnimation(Animation& animation, QList<QPoint> gridPositions);
};

#endif // LEDSOCKETINTERACTIONTESTS_H
