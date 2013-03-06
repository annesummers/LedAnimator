#include "ledanimatoractionbase.h"

#include "animation.h"
#include "frame.h"
#include "led.h"

using namespace AnimatorModel;

LedAnimatorCommandBase::LedAnimatorCommandBase(Animation &animation, QUndoCommand* parent) :
    QUndoCommand(parent),
    iAnimation(animation)
{
}

AddLedCommand::AddLedCommand(Animation &animation, int row, int column) :
    LedAnimatorCommandBase(animation),
    iRow(row),
    iColumn(column) {
}

void AddLedCommand::redo() {
    iAnimation.addNewLed(iRow, iColumn);

    setText(QObject::tr("Add led %1,%2").arg(iRow).arg(iColumn));
}

void AddLedCommand::undo() {
    iAnimation.deleteLed(iRow, iColumn);

    setText(QObject::tr("Add led %1,%2").arg(iRow).arg(iColumn));
}

//bool AddLedCommand::mergeWith(const QUndoCommand *command) {

//}

DeleteLedCommand::DeleteLedCommand(Animation &animation, int row, int column, int number)  :
    LedAnimatorCommandBase(animation),
    iRow(row),
    iColumn(column),
    iNumber(number) {
}

void DeleteLedCommand::redo() {
    Led* led = iAnimation.ledAt(iNumber);

    for(int i = INITIAL_FRAME; i <= iAnimation.numFrames(); i++) {
        iFrames.append(led->frameAt(INITIAL_FRAME).colour());
    }

    iAnimation.deleteLed(iRow, iColumn);

    setText(QObject::tr("Delete led %1,%2").arg(iRow).arg(iColumn));
}

void DeleteLedCommand::undo() {
    iAnimation.addNewLed(iRow, iColumn, iNumber);
    Led* led = iAnimation.ledAt(iNumber);

    for(int i = INITIAL_FRAME; i <= iAnimation.numFrames(); i++) {
        led->frameAt(i).setColour(iFrames.at(i-INITIAL_FRAME));
    }

    setText(QObject::tr("Delete led %1,%2").arg(iRow).arg(iColumn));
}

//bool DeleteLedCommand::mergeWith(const QUndoCommand *command) {

//}

CloneLedCommand::CloneLedCommand(Animation &animation, int oldRow, int oldColumn, int newRow, int newColumn) :
    LedAnimatorCommandBase(animation),
    iOldRow(oldRow),
    iOldColumn(oldColumn),
    iNewRow(newRow),
    iNewColumn(newColumn) {
}

void CloneLedCommand::redo() {
    iAnimation.cloneLed(iOldRow, iOldColumn, iNewRow, iNewColumn);

    setText(QObject::tr("Copy led %1,%2 to %3, %4").arg(iOldRow).arg(iOldColumn).arg(iNewRow).arg(iNewColumn));
}

void CloneLedCommand::undo() {
    iAnimation.deleteLed(iNewRow, iNewColumn);

    setText(QObject::tr("Copy led %1,%2 to %3, %4").arg(iOldRow).arg(iOldColumn).arg(iNewRow).arg(iNewColumn));
}

//bool CloneLedCommand::mergeWith(const QUndoCommand *command) {

//}

MoveLedCommand::MoveLedCommand(Animation &animation, int oldRow, int oldColumn, int newRow, int newColumn) :
    LedAnimatorCommandBase(animation),
    iOldRow(oldRow),
    iOldColumn(oldColumn),
    iNewRow(newRow),
    iNewColumn(newColumn) {
}

void MoveLedCommand::redo() {
    iAnimation.moveLed(iOldRow, iOldColumn, iNewRow, iNewColumn);

    setText(QObject::tr("Move led %1,%2 to %3, %4").arg(iOldRow).arg(iOldColumn).arg(iNewRow).arg(iNewColumn));
}

void MoveLedCommand::undo() {
    iAnimation.moveLed(iNewRow, iNewColumn, iOldRow, iOldColumn);

    setText(QObject::tr("Move led %1,%2 to %3, %4").arg(iOldRow).arg(iOldColumn).arg(iNewRow).arg(iNewColumn));
}

//bool MoveLedCommand::mergeWith(const QUndoCommand *command) {

//}

CutLedCommand::CutLedCommand(Animation &animation, int row, int column) :
    LedAnimatorCommandBase(animation),
    iRow(row),
    iColumn(column){
}

void CutLedCommand::redo() {
    iAnimation.moveLedToClipboard(iRow, iColumn);

    setText(QObject::tr("Cut led %1,%2").arg(iRow).arg(iColumn));
}

void CutLedCommand::undo() {
   // iAnimation.moveLedToClipboard(iNewRow, iNewColumn);

    setText(QObject::tr("Cut led %1,%2").arg(iRow).arg(iColumn));
}

//bool CutLedCommand::mergeWith(const QUndoCommand *command) {

//}

CopyLedCommand::CopyLedCommand(Animation &animation, int row, int column) :
    LedAnimatorCommandBase(animation),
    iRow(row),
    iColumn(column){
}

void CopyLedCommand::redo() {
    iAnimation.moveLedToClipboard(iRow, iColumn);

    setText(QObject::tr("Copy led %1,%2").arg(iRow).arg(iColumn));
}

void CopyLedCommand::undo() {
   // iAnimation.moveLedToClipboard(iNewRow, iNewColumn);

    setText(QObject::tr("Copy led %1,%2").arg(iRow).arg(iColumn));
}

//bool CopyLedCommand::mergeWith(const QUndoCommand *command) {

//}

RenumberLedCommand::RenumberLedCommand(Animation &animation) :
    LedAnimatorCommandBase(animation) {
}

SetFrameColourCommand::SetFrameColourCommand(Animation &animation) :
    LedAnimatorCommandBase(animation) {
}

FadeCommand::FadeCommand(Animation &animation) :
    LedAnimatorCommandBase(animation) {
}

FadeToCommand::FadeToCommand(Animation &animation) :
    LedAnimatorCommandBase(animation) {
}
