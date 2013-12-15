#ifndef LEDSETITERATOR_H
#define LEDSETITERATOR_H

#include <QObject>

namespace AnimatorModel {

class LedSet;
class Led;

class LedSetIterator : public QObject {
    Q_OBJECT

public:
    explicit LedSetIterator (QObject *parent, const LedSet &set);

    bool findNext (const int group);
    bool findPrevious (const int group);
    bool hasNext () const;
    bool hasPrevious () const;
    Led& next ();
    const Led& peekNext () const;
    const Led& peekPrevious () const;
    Led& previous ();
    void toBack ();
    void toFront ();

private:
    LedSetIterator& operator= (const LedSetIterator &set);
    const LedSet& iLedSet;
    int iNextCounter;
    int iPreviousCounter;
};

}

#endif // LEDSETITERATOR_H
