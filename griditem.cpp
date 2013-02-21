#include "griditem.h"

using namespace AnimatorModel;

GridItem::GridItem(QObject *parent, int row, int column) :
    Selectable(parent),
    iRow(row),
    iColumn(column) {
}
