#ifndef FUNCTIONFRAME_H
#define FUNCTIONFRAME_H

#include <QObject>

#include "Frame.h"
#include "FunctionValue.h"

namespace AnimatorModel {

class FunctionFrame : public Frame {
    Q_OBJECT

public:
    explicit FunctionFrame(QObject *parent,
                           Animation &animation,
                           int number,
                           Frame& previousFrame,
                           QUndoStack &undoStack);
    explicit FunctionFrame(QObject* parent,
                           Animation &animation,
                           Function function,
                           int number,
                           Frame& previousFrame,
                           QUndoStack &undoStack);

    const QColor colour() const;
    const int type() const { return kColour; }

    void setValue(FrameValue& value);

signals:

public slots:

private:
    Frame& iPreviousFrame;

};
}

#endif // FUNCTIONFRAME_H
