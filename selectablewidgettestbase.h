#ifndef SELECTABLEWIDGETTESTBASE_H
#define SELECTABLEWIDGETTESTBASE_H

#include <QObject>
#include <QVector>
#include <QList>
#include <QtTest/QtTest>

#include "selectable.h"
#include "selectablewidget.h"
#include "selectablegroupwidget.h"

using namespace AnimatorUi;
using namespace AnimatorModel;

typedef QList<Position> PositionList;
Q_DECLARE_METATYPE ( PositionList )

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
    void compareAreaPoints(SelectableGroupWidget* groupWidget, QList<Position> selectedPoints);
    void calculateAreaPoints(QList<Position>& selectedPoints, Position firstSelected, Position secondSelected);
    
};
}

#endif // SELECTABLEWIDGETTESTBASE_H
