/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef FRAME_H
#define FRAME_H

#include <QObject>
#include <QColor>
#include <QUndoStack>

#include "selectable.h"
#include "FrameValue.h"
#include "Function.h"
#include "constants.h"


namespace AnimatorModel {

class Frame : public Selectable {
    Q_OBJECT

public:
    explicit Frame(QObject *parent,
                   Animation &animation,
                   int number,
                   FrameValue &value,
                   Frame *previous,
                   QUndoStack& undoStack);
    explicit Frame(QObject *parent,
                   Animation &animation,
                   int number,
                   Frame *previous,
                   QUndoStack& undoStack);

    const QColor colour() const;
    const inline Frame* previous() const { return iPrevious; }

    const inline Frame* next() const { return iNext; }
    inline void setNext(Frame& next) { iNext = &next; }

    inline const FrameValue& value() const { return *iValue; }
    void setValue(const FrameValue &value);

    const Function function() const;

    inline void doSetValue(const FrameValue& value) { iValue = (FrameValue*)(&value);
                                                      if(value.type() == kLinked) {
                                                          emit updateAll();
                                                      } else {
                                                          emit valueChanged();
                                                      }}

protected:
    QUndoStack& iUndoStack;

    FrameValue* iValue;
    const Frame* iPrevious;
    const Frame* iNext;

signals:
    void valueChanged();
    void updateAll();
};
}

#endif // FRAME_H
