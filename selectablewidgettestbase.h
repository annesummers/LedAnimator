#ifndef SELECTABLEWIDGETTESTBASE_H
#define SELECTABLEWIDGETTESTBASE_H

#include <QObject>
#include <QVector>
#include <QList>
#include <QPoint>
#include <QtTest/QtTest>

#include "selectable.h"
#include "selectablewidget.h"
#include "selectablegroupwidget.h"

using namespace AnimatorUi;
using namespace AnimatorModel;

typedef QList<QPoint> PointList;
Q_DECLARE_METATYPE ( PointList )

namespace AnimatorTest {

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

typedef QVector<SelectableWidget*> WidgetVector;

class SelectableWidgetTestBase : public QObject
{
    Q_OBJECT
public:
    explicit SelectableWidgetTestBase(QObject *parent = 0);
    
protected:
    void compareAreaPoints(SelectableGroupWidget* groupWidget, QList<QPoint> selectedPoints);
    void calculateAreaPoints(QList<QPoint>& selectedPoints, QPoint firstSelected, QPoint secondSelected);
    
};
}

#endif // SELECTABLEWIDGETTESTBASE_H
