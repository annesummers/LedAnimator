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

    inline const FrameValue& value() const { return *iValue; }
    void setValue(const FrameValue &value);

    inline void doSetValue(const FrameValue& value) { iValue = (FrameValue*)&value; emit valueChanged(); }

protected:
    QUndoStack& iUndoStack;

    FrameValue* iValue;
    Frame* iPrevious;
signals:
    void valueChanged();

};
}

#endif // FRAME_H
