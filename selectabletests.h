#ifndef SELECTABLETESTS_H
#define SELECTABLETESTS_H

#include <QObject>
#include <QtTest/QtTest>

#include "selectable.h"
#include "selectablewidget.h"
#include "selectablegroupwidget.h"

using namespace Ui;

namespace Test {

const int MAX_TEST_WIDGETS = 4;

class SelectableTestObject : public Selectable {
    Q_OBJECT

};

class SelectableTestWidget: public SelectableWidget {
    Q_OBJECT

};

class SelectableGroupTestWidget : public SelectableGroupWidget {
    Q_OBJECT

public:
    SelectableGroupTestWidget(QWidget *parent, int minRow, int maxRow, int minColumn, int maxColumn);

    SelectableWidget& widgetAt(int row, int column);
    virtual void getWidgetPosition(SelectableWidget& widget, int* row, int* column);

    QList<QList*>  iWidgetArray;

private:
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
};
}

#endif // SELECTABLETESTS_H
