/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef LEDANIMATORACTIONBASE_H
#define LEDANIMATORACTIONBASE_H

#include <QUndoCommand>
#include <QColor>

#include "constants.h"
#include "Led.h"

namespace AnimatorModel {
class Animation;
class ColourFrame;

namespace UndoCommands {

class LedAnimatorCommandBase : public QUndoCommand {

public:
    explicit LedAnimatorCommandBase(Animation& animation, QUndoCommand *parent = NULL);
    
signals:
    
public slots:

protected:
    Animation& iAnimation;
    
};

class AddLedCommand : public LedAnimatorCommandBase {

public:
    explicit AddLedCommand(Animation& animation, Position position);

    void undo();
    void redo();
    //bool mergeWith(const QUndoCommand *command);

private:
    Position iPosition;
};

class DeleteLedCommand : public LedAnimatorCommandBase {

public:
    explicit DeleteLedCommand(Animation& animation, Led &led, bool deleteObject);

    void undo();
    void redo();
    //bool mergeWith(const QUndoCommand *command);

private:
    Led iOldLed;
    bool iDeleteObject;
};

class CloneLedCommand : public LedAnimatorCommandBase {

public:
    explicit CloneLedCommand(Animation& animation, Position fromPosition, Position toPosition);

    void undo();
    void redo();
    //bool mergeWith(const QUndoCommand *command);

private:
    void setText();

    Position iFromPosition;
    Position iToPosition;

    Led* iOldLed;
};

class PasteLedCommand : public LedAnimatorCommandBase {

public:
    explicit PasteLedCommand(Animation& animation, Position fromPosition, Position toPosition);

    void undo();
    void redo();
    //bool mergeWith(const QUndoCommand *command);

private:
    void setText();

    Position iFromPosition;
    Position iToPosition;

    Led* iFromLed;
    Led* iToLed;
};

class RenumberLedCommand : public LedAnimatorCommandBase {

public:
    explicit RenumberLedCommand(Animation& animation, Position position, int oldNumber, int newNumber);

    void undo();
    void redo();

private:
    void setText();

    Position iPosition;

    int iOldNumber;
    int iNewNumber;
};

class SetFrameColourCommand : public LedAnimatorCommandBase {

public:
    explicit SetFrameColourCommand(Animation &animation,
                                   Frame& frame, const FrameValue &newValue);

    void undo();
    void redo();
   // bool mergeWith(const QUndoCommand *command);

private:
    void setText();

    Frame& iFrame;
    const FrameValue& iOldValue;
    const FrameValue& iNewValue;
};

class FadeCommand : public LedAnimatorCommandBase {

public:
    explicit FadeCommand(Animation& animation);

    void undo();
    void redo();

private:
    void setText();
};

class FadeToCommand : public LedAnimatorCommandBase {

public:
    explicit FadeToCommand(Animation& animation);

    void undo();
    void redo();

private:
    void setText();
};
/*
class MoveLedCommand : public LedAnimatorCommandBase {

public:
    explicit MoveLedCommand(Animation& animation, Position fromPosition, Position toPosition);

    void undo();
    void redo();
    //bool mergeWith(const QUndoCommand *command);

private:
    void setText();

    Position iFromPosition;
    Position iToPosition;
};

class CutLedCommand : public LedAnimatorCommandBase {

public:
    explicit CutLedCommand(Animation& animation, Led &cutLed);

    void undo();
    void redo();
    //bool mergeWith(const QUndoCommand *command);

private:
    void setText();

    Led iCutLed;
};

class CopyLedCommand : public LedAnimatorCommandBase {

public:
    explicit CopyLedCommand(Animation& animation, Led &led);

    void undo();
    void redo();
    //bool mergeWith(const QUndoCommand *command);

private:
    void setText();

    Led iCopyLed;
};
*/
}

}

#endif // LEDANIMATORACTIONBASE_H
