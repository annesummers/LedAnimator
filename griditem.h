#ifndef GRIDITEM_H
#define GRIDITEM_H

#include "selectable.h"

namespace AnimatorModel {

class GridItem : public Selectable {
    Q_OBJECT

public:
    explicit GridItem(QObject *parent, int row, int column);

    inline const int row() const { return iRow; }
    inline const int column() const {return iColumn; }

protected:
    inline void setRow(int row) { iRow = row; }
    inline void setColumn(int column) { iColumn = column; }

private:
    int     iRow;
    int     iColumn;   
};
}

#endif // GRIDITEM_H
