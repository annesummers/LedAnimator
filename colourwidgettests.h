/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef COLOURWIDGETTESTS_H
#define COLOURWIDGETTESTS_H

#include <QObject>
#include <QtTest/QtTest>

#include "colourwidget.h"
#include "colourgroupwidget.h"
#include "selectablewidgettestbase.h"

#include "testconstants.h"

using namespace AnimatorUi;

namespace AnimatorTest {

class ColourTestObject : public Selectable {
    Q_OBJECT

public :
    explicit ColourTestObject(QObject* parent, Animation& animation, int number) :
        Selectable(parent, animation, number) { }

    QColor iColour;
};

class ColourTestWidget: public ColourWidget {
    Q_OBJECT

public:
    explicit ColourTestWidget(QWidget* parent, ColourGroupWidget& colourGroup, ColourTestObject& selectable) :
        ColourWidget(parent, colourGroup, selectable) {
    }

    inline QString mimeType() const { return QString("test"); }
    Qt::DropActions dropActions() const { return Qt::IgnoreAction; }
    Qt::DropActions dragActions() const { return Qt::IgnoreAction; }
    Qt::DropAction  defaultDropAction() const { return Qt::IgnoreAction; }
    Qt::DropAction  controlDropAction() const { return Qt::IgnoreAction; }

    //inline QMimeData* mimeData(bool cut)  { return SelectableWidget::mimeData(cut); }

    inline const QColor colour() const { return static_cast<ColourTestObject&>(iItem).iColour; }
    inline void setColour(QColor colour) { static_cast<ColourTestObject&>(iItem).iColour = colour; }
};

class ColourGroupTestWidget : public ColourGroupWidget {
    Q_OBJECT

public:
    ColourGroupTestWidget(QWidget *parent, Animation &animation, int numRows, int numColumns,
                          ColourGroupGroupWidget& groupGroupWidget, int groupNumber);

    SelectableWidget& widgetAt(Position position);
    Position widgetPosition(SelectableWidget& widget);

    bool validKeyPress(Qt::Key key);

    void moveItem(int fromGroup, Position fromPosition, Position toPosition)
        { Q_UNUSED(fromGroup); Q_UNUSED(fromPosition); Q_UNUSED(toPosition); }
    void pasteItem(int fromGroup, Position fromPosition, Position toPosition)
        { Q_UNUSED(fromGroup); Q_UNUSED(fromPosition); Q_UNUSED(toPosition); }

    void cloneItem(int fromGroup, Position fromPosition, Position toPosition);

    QVector<WidgetVector*>*  iWidgetArray;

    inline FadeCalculator* fadeCalculator() const { return iFadeCalculator; }
    inline void setupFade(QColor fadeToColour) { ColourGroupWidget::setupFade(fadeToColour); }
    inline void startFade() { ColourGroupWidget::startFade(); }
};

class ColourGroupGroupTestWidget : public ColourGroupGroupWidget {
    Q_OBJECT

public:
    ColourGroupGroupTestWidget() {}

    inline QString mimeType() const { return TEST_MIME_TYPE; }
};

struct FadeData {
    IntList     fadeSpread;
    PositionList   fadePoints;
    QList<QColor> fadeColours;
};

class ColourWidgetTests : public SelectableWidgetTestBase {
    Q_OBJECT

public:
    explicit ColourWidgetTests(QObject *parent = 0);

protected slots:
    void dismissDialog();

    void fadeColourCalculated(QColor colour);
    void fadeComplete();
    void fadeTimeout();
    
private slots:
    void fade_data();
    void fade();

    void doubleClickOne_data();
    void doubleClickOne();

    void doubleClickOneClickOne_data();
    void doubleClickOneClickOne();

    void clickOneDoubleClickOne_data();
    void clickOneDoubleClickOne();

    void doubleClickOneDoubleClickOne_data();
    void doubleClickOneDoubleClickOne();

    void copyPasteOneInternal_data();
    void copyPasteOneInternal();

    void copyPasteManyInternal_data();
    void copyPasteManyInternal();

    void copyPasteOneExternal_data();
    void copyPasteOneExternal();

    void copyPasteManyExternal_data();
    void copyPasteManyExternal();

private:
    void doubleClickWidgetAndDismissDialog(ColourGroupTestWidget &groupWidget, Position widgetPoint);

    bool waitForSignal(QObject *sender,
                       const char *signal,
                       const char* slot,
                       const char* timeoutSlot,
                       int timeout = 1000);

    Animation* iAnimation;

    QColorDialog* iColourDialog;
    ColourGroupTestWidget* iGroupWidget;

    FadeData* iFadeData;

    int iFadeCounter;

    QEventLoop iAsyncEventLoop;
    QTimer     iAsyncTimeoutTimer;
};
}

#endif // COLOURWIDGETTESTS_H
