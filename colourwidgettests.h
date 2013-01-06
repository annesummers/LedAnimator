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

};

class ColourTestWidget: public ColourWidget {
    Q_OBJECT

};

class ColourGroupTestWidget : public ColourGroupWidget {
    Q_OBJECT

public:
    ColourGroupTestWidget(QWidget *parent, int numRows, int numColumns);

    ColourWidget& widgetAt(int row, int column);
    virtual void getWidgetPosition(ColourWidget& widget, int* row, int* column);

    QVector<WidgetVector*>*  iWidgetArray;

    inline void selectDirection(int direction) { selectDirection(direction); }
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

    void selectColour_data();
    void selectColour();

    void adjustSize_data();
    void adjustSize();

    void fade_data();
    void fade();

    void copyOne_data();
    void copyOne();

    void copyMany_data();
    void copyMany();

    void pasteOne_data();
    void pasteOne();

    void pasteMany_data();
    void pasteMany();

    void compareSelected(ColourGroupTestWidget* groupWidget, QList<ColourWidget*> selected, QList<QPoint> selectedPoints);
};
}

#endif // SELECTABLETESTS_H
