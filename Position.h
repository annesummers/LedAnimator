#ifndef POSITION_H
#define POSITION_H

#include <QPair>
#include <QMetaType>

#include "constants.h"

namespace AnimatorModel {

class Position : public QPair<int, int> {
public:
    inline Position(int row, int column) : QPair<int, int> (row, column) {}
    inline Position() : QPair<int, int> (INVALID, INVALID) {}
    inline Position(const Position& position) : QPair<int, int> (position.first, position.second) {}

    inline int row() const { return first; }
    inline int column() const { return second; }
    inline bool isValid() const { return first > INVALID && second > INVALID; }
    inline bool operator==(const Position& position) { return first == position.row() && second == position.column(); }
    inline bool operator<(const Position& position) { return (first < position.row() && second == position.column()) ||
                                                        (first == position.row() && second < position.column()); }
    inline Position& operator=(const Position& position) { first = position.first; second = position.second; return *this; }
};
}

using namespace AnimatorModel;

Q_DECLARE_METATYPE ( Position )

#endif // POSITION_H
