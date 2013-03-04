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
    explicit TestLedGridWidget(QWidget* parent, Animation &animation, ColourGroupGroupWidget& colourGroupGroup) :
        LedGridWidget(parent, animation, colourGroupGroup) {}

    SelectableWidget& widgetAt(int row, int column) { return LedGridWidget::widgetAt(row, column); }
};

class LedSocketInteractionTests : public QObject {
    Q_OBJECT

public:
    explicit LedSocketInteractionTests(QObject *parent = 0);
    
private slots:
    void copyPasteOneLedToOneSocket_data();
    void copyPasteOneLedToOneSocket();

    void copyPasteOneLedToOneLed_data();
    void copyPasteOneLedToOneLed();

    void copyPasteOneLedToManySockets_data();
    void copyPasteOneLedToManySockets();

    void copyPasteOneLedToManyLeds_data();
    void copyPasteOneLedToManyLeds();

    void copyPasteManyLedToSocket_data();
    void copyPasteManyLedToSocket();

    void copyPasteManyLedToLed_data();
    void copyPasteManyLedToLed();

    void copyPasteManyLedToMixed_data();
    void copyPasteManyLedToMixed();

private:
    void setupAnimation(Animation& animation, QList<QPoint> gridPositions);
};

#endif // LEDSOCKETINTERACTIONTESTS_H
