#ifndef LEDSET_H
#define LEDSET_H

#include <QHash>

#include "constants.h"
#include "LedSetIterator.h"
#include "Led.h"

namespace AnimatorModel {

class Animation;

class LedSet : public QObject {
    Q_OBJECT

public :
    explicit LedSet(QObject *parent);

    void addLed(Led& led);
    void removeLed(Led& led);

    void clearPositions();
    inline void clear() { iLeds.clear(); }
    void clearAll();

    Led* findLed(Position position) const;
    Led* findLed(int ledNumber) const;

    void moveLed(Led &led, Position fromPosition, Position toPosition);

    void setPositions(QList<int> positions);

    Position ledPosition (int ledNumber) const;

    inline bool ledsMissing() const { return iFreeNumbers.count() != 0; }
    inline bool isMissing(int ledNumber) const { return iFreeNumbers.contains(ledNumber); }
    inline const QList<int> missingLedNumbers() const { return iFreeNumbers; }

    void clearMissing();
    void addMissing(int ledNumber);

    inline void setHighestNumber(int highestNumber) { iHighestNumber = highestNumber; }
    inline int highestNumber() const { return iHighestNumber; }

    inline int count() const { return iLeds.count(); }

    LedSetIterator& iterator();

    inline void setNumRows(int numRows) { iNumRows = numRows; }
    inline void setNumColumns(int numColumns) { iNumColumns = numColumns; }

private:
    LedSet(const LedSet &set);

    int positionNumber(Position position) const;
    void setPositionNumber(Position position, int number);
    LedSetIterator* iIterator;

    int iHighestNumber;
    QList<int> iFreeNumbers;

    QList<int> iPositions;

    QHash<int, Led*> iLeds;

    int iNumRows;
    int iNumColumns;

    friend class LedSetIterator;
};
}

#endif // LEDSET_H
