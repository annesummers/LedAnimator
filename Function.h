#ifndef FUNCTION_H
#define FUNCTION_H

#include <QObject>

namespace AnimatorModel {

class Function : public QObject {
public:
    Function(float redIncrement,
             float greenIncrement,
              float blueIncrement);
    Function();
    Function(const Function& function);

    Function& operator=(const Function& position);

    const float redIncrement() const { return iRedIncrement; }
    const float greenIncrement() const { return iGreenIncrement; }
    const float blueIncrement() const { return iBlueIncrement; }

signals:

public slots:

private:
    float iRedIncrement;
    float iGreenIncrement;
    float iBlueIncrement;

};
}

#endif // FUNCTION_H
