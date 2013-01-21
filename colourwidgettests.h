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

    QString mimeType() const { return QString("test"); }
    Qt::DropAction dropAction() const { Qt::LinkAction; }
};

class ColourGroupTestWidget : public ColourGroupWidget {
    Q_OBJECT

public:
    ColourGroupTestWidget(QWidget *parent, int numRows, int numColumns);

    ColourWidget& widgetAt(int row, int column);
    virtual void getWidgetPosition(ColourWidget& widget, int* row, int* column);

    QVector<WidgetVector*>*  iWidgetArray;

    inline void selectDirection(int direction) { selectDirection(direction); }
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

    void copyPasteOne_data();
    void copyPasteOne();

    void copyPasteMany_data();
    void copyPasteMany();

    void compareSelected(ColourGroupTestWidget* groupWidget, QList<ColourWidget*> selected, QList<QPoint> selectedPoints);
};
}

#endif // SELECTABLETESTS_H
