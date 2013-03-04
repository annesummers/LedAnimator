#include "griditem.h"

using namespace AnimatorModel;

GridItem::GridItem(QObject *parent, int number, int row, int column) :
    Selectable(parent, number),
    iRow(row),
    iColumn(column) {
}
