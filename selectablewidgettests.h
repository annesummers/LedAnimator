#ifndef SELECTABLEWIDGETTESTS_H
#define SELECTABLEWIDGETTESTS_H

#include <QObject>
#include <QtTest/QtTest>

#include "selectablewidgettestbase.h"

using namespace AnimatorUi;

namespace AnimatorTest {

class SelectableTestWidget : public SelectableWidget {
    Q_OBJECT

public:
    explicit SelectableTestWidget(QWidget* parent, SelectableGroupWidget& selectableGroup, Selectable& selectable) :
        SelectableWidget(parent, selectableGroup, selectable) {
        setObjectName("SelectableTestWidget");
    }

    virtual void handleMimeData(QDataStream& dataStream, bool move) {}
    virtual void writeMimeData(QDataStream& dataStream) {}

    inline QString mimeType() const { return QString("test"); }
    Qt::DropActions dropActions() const { return Qt::IgnoreAction; }
    Qt::DropActions dragActions() const { return Qt::IgnoreAction; }
    Qt::DropAction  defaultDropAction() const { return Qt::IgnoreAction; }
    Qt::DropAction  controlDropAction() const { return Qt::IgnoreAction; }

    inline QMimeData* mimeData()  { return SelectableWidget::mimeData(); }
};

class SelectableTestWidget1 : public SelectableTestWidget {
    Q_OBJECT

public:
    explicit SelectableTestWidget1(QWidget* parent, SelectableGroupWidget& selectableGroup, Selectable& selectable) :
        SelectableTestWidget(parent, selectableGroup, selectable) {
        setObjectName("SelectableTestWidget1");
    }
};

class SelectableTestWidget2 : public SelectableTestWidget {
    Q_OBJECT

public:
    explicit SelectableTestWidget2(QWidget* parent, SelectableGroupWidget& selectableGroup, Selectable& selectable) :
        SelectableTestWidget(parent, selectableGroup, selectable) {
        setObjectName("SelectableTestWidget2");
    }
};

class SelectableGroupTestWidget : public SelectableGroupWidget {
    Q_OBJECT

public:
    SelectableGroupTestWidget(QWidget *parent, int numRows, int numColumns, ColourGroupGroupWidget* groupGroupWidget = NULL);

    SelectableWidget& widgetAt(int row, int column);
    virtual void getWidgetPosition(SelectableWidget& widget, int* row, int* column);
    bool validKeyPress(Qt::Key key);

    QVector<WidgetVector*>*  iWidgetArray;

    inline void selectDirection(Qt::Key direction) { SelectableGroupWidget::selectDirection(direction); }
    inline const QList<SelectableWidget*> selectedItems() const { return SelectableGroupWidget::selectedItems(); }
};

class SelectableWidgetTests : public SelectableWidgetTestBase {
    Q_OBJECT

public:
    explicit SelectableWidgetTests(QObject *parent = 0);

private slots:
    void select_data();
    void select();

    void selectOne_data();
    void selectOne();

    void selectDirection_data();
    void selectDirection();

    void selectArea_data();
    void selectArea();

    void selectOneSelectExternal_data();
    void selectOneSelectExternal();

    void clickOne_data();
    void clickOne();

    void clickOneClickOne_data();
    void clickOneClickOne();

    void ctrlClickMany_data();
    void ctrlClickMany();

    void ctrlClickManyDifferentType_data();
    void ctrlClickManyDifferentType();

    void clickOneCtrlClickMany_data();
    void clickOneCtrlClickMany();

    void clickOneCtrlClickManyDifferentType_data();
    void clickOneCtrlClickManyDifferentType();

    void ctrlClickManyClickOne_data();
    void ctrlClickManyClickOne();

    void ctrlClickManyClickOneDifferentType_data();
    void ctrlClickManyClickOneDifferentType();

    void clickShiftClick_data();
    void clickShiftClick();

    void clickShiftClickMixedType_data();
    void clickShiftClickMixedType();

    void clickShiftDirection_data();
    void clickShiftDirection();

    void clickShiftDirectionClick_data();
    void clickShiftDirectionClick();

    void clickShiftDirectionMixedType_data();
    void clickShiftDirectionMixedType();

    void clickShiftClickRightClick_data();
    void clickShiftClickRightClick();

private:
    void areaData();
    void directionData();
};
}

#endif // SELECTABLEWIDGETTESTS_H
