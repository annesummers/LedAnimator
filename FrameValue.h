#ifndef FRAMEVALUE_H
#define FRAMEVALUE_H

#include <QObject>

namespace AnimatorModel {

class FrameValue : public QObject {
    Q_OBJECT

public:
    explicit inline FrameValue(QObject *parent) : QObject(parent) {}

    virtual FrameValue& operator=(const FrameValue& copyValue) = 0;
    virtual int type() const = 0;

signals:

public slots:

};

}

#endif // FRAMEVALUE_H
