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
    explicit ColourTestObject(QObject* parent, int number) :
        Selectable(parent, number) { }

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

    inline QMimeData* mimeData(bool cut)  { return SelectableWidget::mimeData(cut); }

    inline const QColor colour() const { return static_cast<ColourTestObject&>(iItem).iColour; }
    inline void setColour(QColor colour) { static_cast<ColourTestObject&>(iItem).iColour = colour; }
};

class ColourGroupTestWidget : public ColourGroupWidget {
    Q_OBJECT

public:
    ColourGroupTestWidget(QWidget *parent, int numRows, int numColumns, ColourGroupGroupWidget& groupGroupWidget, int groupNumber);

    SelectableWidget& widgetAt(int row, int column);
    void getWidgetPosition(SelectableWidget& widget, int* row, int* column);
    bool validKeyPress(Qt::Key key);

    void moveItem(int fromGroup, int fromRow, int fromColumn, int toRow, int toColumn)
        { Q_UNUSED(fromGroup); Q_UNUSED(fromRow); Q_UNUSED(fromColumn); Q_UNUSED(toRow); Q_UNUSED(toColumn); }
    void pasteItem(int fromGroup, int fromRow, int fromColumn, int toRow, int toColumn)
        { Q_UNUSED(fromGroup); Q_UNUSED(fromRow); Q_UNUSED(fromColumn); Q_UNUSED(toRow); Q_UNUSED(toColumn); }

    void cloneItem(int fromGroup, int fromRow, int fromColumn, int toRow, int toColumn);

    QVector<WidgetVector*>*  iWidgetArray;

    inline FadeCalculator* fadeCalculator() const { return iFadeCalculator; }
    inline void setupFade(QColor fadeToColour) { ColourGroupWidget::setupFade(fadeToColour); }
    inline void startFade() { ColourGroupWidget::startFade(); }
};

struct FadeData {
    IntList     fadeSpread;
    PointList   fadePoints;
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
    void doubleClickWidgetAndDismissDialog(ColourGroupTestWidget &groupWidget, QPoint widgetPoint);

    bool waitForSignal(QObject *sender,
                       const char *signal,
                       const char* slot,
                       const char* timeoutSlot,
                       int timeout = 1000);

    QColorDialog* iColourDialog;
    ColourGroupTestWidget* iGroupWidget;

    FadeData* iFadeData;

    int iFadeCounter;

    QEventLoop iAsyncEventLoop;
    QTimer     iAsyncTimeoutTimer;
};
}

#endif // COLOURWIDGETTESTS_H
