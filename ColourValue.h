#ifndef COLOURVALUE_H
#define COLOURVALUE_H

#include <QObject>
#include <QColor>

#include "FrameValue.h"
#include "constants.h"

namespace AnimatorModel {

class ColourValue : public FrameValue {
    Q_OBJECT

public:
    explicit ColourValue(QObject *parent, QColor colour);

    void setColour(QColor colour);
    inline const QColor colour() const { return iColour; }

    FrameValue& operator=(const FrameValue& copyValue);

    inline int type() const { return kColour; }

signals:

public slots:

private:
    QColor iColour;

};

}

#endif // COLOURVALUE_H
