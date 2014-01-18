#ifndef RANGE_H
#define RANGE_H

#include <QPair>

namespace AnimatorModel {

class Range {

public:
    inline Range(int anchorValue,
                 int startValue,
                 int endValue,
                 int function) :
        iRange(startValue, endValue),
        iFunction(function),
        iAnchorValue(anchorValue){}

    inline Range(const Range& range) :
        iRange(range.iRange),
        iFunction(range.iFunction),
        iAnchorValue (range.iAnchorValue) { }

    inline Range& operator=(const Range& range) {
        iRange = range.iRange;
        iFunction = range.iFunction;
        iAnchorValue = range.iAnchorValue;

        return *this;
    }

    inline int lowValue() const { return iRange.first; }
    inline int highValue() const { return iRange.second; }

    inline void setFunction(int function) { iFunction = function; }
    inline int function() const { return iFunction; }

    inline void setAnchor(int anchor) { iAnchorValue = anchor; }
    inline int anchor() const { return iAnchorValue; }

private:
    QPair<int, int>  iRange;
    int iFunction;
    int iAnchorValue;
};

}

#endif // RANGE_H
