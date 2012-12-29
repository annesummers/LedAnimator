#ifndef SELECTABLETESTS_H
#define SELECTABLETESTS_H

#include <QObject>
#include <QtTest/QtTest>
#include <QVector>
#include <QList>

#include "selectable.h"
#include "selectablewidget.h"
#include "selectablegroupwidget.h"

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

//typedef Q2DVector<SelectableWidget*> WidgetVector;

typedef QVector<SelectableWidget*> WidgetVector;

//typedef QVector<WidgetVector> 2DWidgetVector;

class SelectableTestObject : public Selectable {
    Q_OBJECT

};

class SelectableTestWidget: public SelectableWidget {
    Q_OBJECT

};

class SelectableGroupTestWidget : public SelectableGroupWidget {
    Q_OBJECT

public:
    SelectableGroupTestWidget(QWidget *parent, int maxRow, int maxColumn);

    SelectableWidget& widgetAt(int row, int column);
    virtual void getWidgetPosition(SelectableWidget& widget, int* row, int* column);

    QVector<WidgetVector*>*  iWidgetArray;

    inline void selectDirection(int direction) { selectDirection(direction); }
};

class SelectableTests : public QObject {
    Q_OBJECT

public:
    explicit SelectableTests(QObject *parent = 0);
    
private slots:
    void select_data();
    void select();

    void selectOne_data();
    void selectOne();

    void selectDirection_data();
    void selectDirection();

    void selectArea_data();
    void selectArea();

    void compareSelected(SelectableGroupTestWidget* groupWidget, QList<SelectableWidget*> selected, QList<QPoint> selectedPoints);
};
}

#endif // SELECTABLETESTS_H
