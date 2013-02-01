#ifndef SELECTABLETESTS_H
#define SELECTABLETESTS_H

#include <QObject>
#include <QtTest/QtTest>
#include <QVector>
#include <QList>

#include "selectable.h"
#include "colourwidget.h"
#include "colourgroupwidget.h"

using namespace Ui;

typedef QList<QPoint> PointList;
Q_DECLARE_METATYPE ( PointList )

typedef QList<int> IntList;
Q_DECLARE_METATYPE ( IntList )

namespace Test {

const int MAX_TEST_WIDGETS = 4;

template <class type>
class Q2DVector : public QVector< QVector<type> > {
    public:
        Q2DVector() : QVector< QVector<type> >(){}
        Q2DVector(int rows, int columns) : QVector< QVector<type> >(rows) {
            for(int r=0; r<rows; r++) {
                this[r].resize(columns);
            }
        }
        virtual ~Q2DVector() {}
};

typedef QVector<ColourWidget*> WidgetVector;

class SelectableTestObject : public Selectable {
    Q_OBJECT

public :
    explicit SelectableTestObject(QObject* parent) :
        Selectable(parent) { }

    QColor iColour;
};

class ColourTestWidget: public ColourWidget {
    Q_OBJECT

public:
    explicit ColourTestWidget(QWidget* parent, ColourGroupWidget& colourGroup, SelectableTestObject& selectable) :
        ColourWidget(parent, colourGroup, selectable) {
    }

    inline const QColor colour() const { return static_cast<SelectableTestObject&>(iItem).iColour; }
    inline void setColour(QColor colour) { static_cast<SelectableTestObject&>(iItem).iColour = colour; }

    inline void handleExtraData(QDataStream& dataStream) { Q_UNUSED(dataStream); }
    inline void addExtraData(QDataStream& dataStream) { Q_UNUSED(dataStream); }

    inline QString mimeType() const { return QString("test"); }
    inline Qt::DropAction dropAction() const { return Qt::LinkAction; }

    inline QColorDialog& colourDialog() { return *iColourDialog; }
};

class ColourGroupTestWidget : public ColourGroupWidget {
    Q_OBJECT

public:
    ColourGroupTestWidget(QWidget *parent, int numRows, int numColumns);

    ColourWidget& widgetAt(int row, int column);
    virtual void getWidgetPosition(ColourWidget& widget, int* row, int* column);
    bool validKeyPress(Qt::Key key);

    QVector<WidgetVector*>*  iWidgetArray;

    inline void selectDirection(Qt::Key direction) { ColourGroupWidget::selectDirection(direction); }
    inline QList<ColourWidget*> selectedItems() { return iSelected; }
};

class ColourWidgetTests : public QObject {
    Q_OBJECT

public:
    explicit ColourWidgetTests(QObject *parent = 0);
    
private slots:
    void select_data();
    void select();

    void selectOne_data();
    void selectOne();

    void selectDirection_data();
    void selectDirection();

    void selectArea_data();
    void selectArea();

    void fade_data();
    void fade();

    void clickOne_data();
    void clickOne();

    void clickOneClickOne_data();
    void clickOneClickOne();

    void doubleClickOne_data();
    void doubleClickOne();

    void doubleClickOneClickOne_data();
    void doubleClickOneClickOne();

    void clickOneDoubleClickOne_data();
    void clickOneDoubleClickOne();

    void doubleClickOneDoubleClickOne_data();
    void doubleClickOneDoubleClickOne();

    void ctrlClickMany_data();
    void ctrlClickMany();

    void clickOneCtrlClickMany_data();
    void clickOneCtrlClickMany();

    void ctrlClickManyClickOne_data();
    void ctrlClickManyClickOne();

    void clickShiftClick_data();
    void clickShiftClick();

    void clickShiftDirection_data();
    void clickShiftDirection();

    void clickShiftDirectionClick_data();
    void clickShiftDirectionClick();

    void dragDropOneInternal_data();
    void dragDropOneInternal();

    void dragDropManyInternal_data();
    void dragDropManyInternal();

    void dragDropOneExternal_data();
    void dragDropOneExternal();

    void dragDropManyExternal_data();
    void dragDropManyExternal();

    void copyPasteOneInternal_data();
    void copyPasteOneInternal();

    void copyPasteManyInternal_data();
    void copyPasteManyInternal();

    void copyPasteOneExternal_data();
    void copyPasteOneExternal();

    void copyPasteManyExternal_data();
    void copyPasteManyExternal();

    void dismissDialog();

private:
    void compareSelected(ColourGroupTestWidget* groupWidget/*, QList<ColourWidget*> selected*/, QList<QPoint> selectedPoints);
    void doubleClickWidgetAndDismissDialog(ColourGroupTestWidget &groupWidget, QPoint widgetPoint);
    void calculateSelectedPoints(QList<QPoint>& selectedPoints, QPoint firstSelected, QPoint secondSelected);

    void areaData();
    void directionData();

    QColorDialog* iColourDialog;
};
}

#endif // SELECTABLETESTS_H
