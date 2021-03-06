#ifndef FUNCTION_H
#define FUNCTION_H

#include <QObject>

namespace AnimatorModel {

class Function {
public:
    Function(float redIncrement,
             float greenIncrement,
             float blueIncrement);

    Function();
    Function(const Function& function);

    Function& operator=(const Function& function);
    Function& operator+=(const Function& function);

    bool operator==(const Function& function);

    float redIncrement() const { return iRedIncrement; }
    float greenIncrement() const { return iGreenIncrement; }
    float blueIncrement() const { return iBlueIncrement; }

    inline bool isNull() const { return iRedIncrement == 0 &&
                                        iGreenIncrement == 0 &&
                                        iBlueIncrement == 0; }

signals:

public slots:

private:
    float iRedIncrement;
    float iGreenIncrement;
    float iBlueIncrement;

};

inline Function operator+(Function lhs, const Function& rhs) {
    lhs += rhs;
    return lhs;
}

}

#endif // FUNCTION_H
