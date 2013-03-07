/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef LEDANIMATORACTIONBASE_H
#define LEDANIMATORACTIONBASE_H

#include <QUndoCommand>
#include <QColor>

namespace AnimatorModel {

class Animation;
class Frame;

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
    explicit AddLedCommand(Animation& animation, int row, int column);

    void undo();
    void redo();
    //bool mergeWith(const QUndoCommand *command);

private:
    int iRow;
    int iColumn;
};

class DeleteLedCommand : public LedAnimatorCommandBase {

public:
    explicit DeleteLedCommand(Animation& animation, int row, int column, int number);

    void undo();
    void redo();
    //bool mergeWith(const QUndoCommand *command);

private:
    int iRow;
    int iColumn;

    int iNumber;
    QList<QColor> iFrames;
};

class CloneLedCommand : public LedAnimatorCommandBase {

public:
    explicit CloneLedCommand(Animation& animation, int oldRow, int oldColumn, int newRow, int newColumn);

    void undo();
    void redo();
    //bool mergeWith(const QUndoCommand *command);

private:
    int iOldRow;
    int iOldColumn;

    int iNewRow;
    int iNewColumn;
};

class MoveLedCommand : public LedAnimatorCommandBase {

public:
    explicit MoveLedCommand(Animation& animation, int oldRow, int oldColumn, int newRow, int newColumn);

    void undo();
    void redo();
    //bool mergeWith(const QUndoCommand *command);

private:
    int iOldRow;
    int iOldColumn;

    int iNewRow;
    int iNewColumn;
};

class CutLedCommand : public LedAnimatorCommandBase {

public:
    explicit CutLedCommand(Animation& animation, int row, int column);

    void undo();
    void redo();
    //bool mergeWith(const QUndoCommand *command);

private:
    int iRow;
    int iColumn;
};

class CopyLedCommand : public LedAnimatorCommandBase {

public:
    explicit CopyLedCommand(Animation& animation, int row, int column);

    void undo();
    void redo();
    //bool mergeWith(const QUndoCommand *command);

private:
    int iRow;
    int iColumn;
};

class RenumberLedCommand : public LedAnimatorCommandBase {

public:
    explicit RenumberLedCommand(Animation& animation);

   // void undo();
   // void redo();
};

class SetFrameColourCommand : public LedAnimatorCommandBase {

public:
    explicit SetFrameColourCommand(Animation& animation);

   // void undo();
   // void redo();
   // bool mergeWith(const QUndoCommand *command);
};

class FadeCommand : public LedAnimatorCommandBase {

public:
    explicit FadeCommand(Animation& animation);

  //  void undo();
  //  void redo();
};

class FadeToCommand : public LedAnimatorCommandBase {

public:
    explicit FadeToCommand(Animation& animation);

  //  void undo();
  //  void redo();
};

}

#endif // LEDANIMATORACTIONBASE_H
