/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "ledanimatoractionbase.h"

#include "Animation.h"
#include "Led.h"
#include "FrameValue.h"

using namespace AnimatorModel;
using namespace UndoCommands;

LedAnimatorCommandBase::LedAnimatorCommandBase(Animation &animation, QUndoCommand* parent) :
    QUndoCommand(parent),
    iAnimation(animation)
{
}

AddLedCommand::AddLedCommand(Animation &animation, Position position) :
    LedAnimatorCommandBase(animation),
    iPosition(position) {
}

void AddLedCommand::redo() {
    iAnimation.doAddNewLed(iPosition);

    setText(QObject::tr("Add led %1,%2").arg(iPosition.row()).arg(iPosition.column()));
}

void AddLedCommand::undo() {
    iAnimation.doDeleteLed(iPosition, true);

    setText(QObject::tr("Add led %1,%2").arg(iPosition.row()).arg(iPosition.column()));
}

//bool AddLedCommand::mergeWith(const QUndoCommand *command) {

//}

DeleteLedCommand::DeleteLedCommand(Animation &animation, Led& led, bool deleteObject)  :
    LedAnimatorCommandBase(animation),
    iOldLed(led),
    iDeleteObject(deleteObject){
}

void DeleteLedCommand::redo() {
    iAnimation.doDeleteLed(iOldLed.position(), iDeleteObject);

    setText(QObject::tr("Delete led %1,%2").arg(iOldLed.position().row()).arg(iOldLed.position().column()));
}

void DeleteLedCommand::undo() {
    if(!iDeleteObject) {
        iAnimation.deleteLedFromClipboard(iOldLed.number());
    }

    iAnimation.doAddNewLed(iOldLed.position(), iOldLed.number());

    Led* led = iAnimation.ledAt(iOldLed.number());

    led->copyAxes(iOldLed);

    setText(QObject::tr("Delete led %1,%2").arg(iOldLed.position().row()).arg(iOldLed.position().column()));
}

//bool DeleteLedCommand::mergeWith(const QUndoCommand *command) {

//}

CloneLedCommand::CloneLedCommand(Animation &animation, Position fromPosition, Position toPosition) :
    LedAnimatorCommandBase(animation),
    iFromPosition(fromPosition),
    iToPosition(toPosition),
    iOldLed(NULL) {
}

void CloneLedCommand::redo() {
    iOldLed = iAnimation.doCloneLed(iFromPosition, iToPosition);

    setText();
}

void CloneLedCommand::undo() {
    iAnimation.doDeleteLed(iToPosition, true);

    if(iOldLed != NULL) {
        iAnimation.doAddNewLed(iToPosition, iOldLed->number());
        Led* led = iAnimation.ledAt(iOldLed->number());

        led->copyAxes(*iOldLed);
        delete iOldLed;
    }

    setText();
}

void CloneLedCommand::setText() {
    QUndoCommand::setText(QObject::tr("Copy led %1,%2 to %3, %4").arg(iFromPosition.row()).arg(iFromPosition.column()).arg(iToPosition.row()).arg(iToPosition.column()));
}

//bool CloneLedCommand::mergeWith(const QUndoCommand *command) {

//}

PasteLedCommand::PasteLedCommand(Animation &animation, Position fromPosition, Position toPosition) :
    LedAnimatorCommandBase(animation),
    iFromPosition(fromPosition),
    iToPosition(toPosition),
    iFromLed(NULL),
    iToLed(NULL){
}

void PasteLedCommand::redo() {
    iAnimation.doPasteLed(iFromPosition, iToPosition, &iFromLed, &iToLed);

    setText();
}

void PasteLedCommand::undo() {
    iFromLed->move(iFromPosition);
    iAnimation.addLedToClipboard(iFromLed);
    iAnimation.doDeleteLed(iToPosition, true);

    if(iToLed != NULL) {
        iAnimation.doAddNewLed(iToPosition, iToLed->number());
        Led* led = iAnimation.ledAt(iToLed->number());

        led->copyAxes(*iToLed);
        delete iToLed;
    }

    setText();
}

void PasteLedCommand::setText() {
    QUndoCommand::setText(QObject::tr("Paste led %1,%2").arg(iToPosition.row()).arg(iToPosition.column()));
}

RenumberLedCommand::RenumberLedCommand(Animation &animation, Position position, int oldNumber, int newNumber) :
    LedAnimatorCommandBase(animation),
    iPosition(position),
    iOldNumber(oldNumber),
    iNewNumber(newNumber) {
}

void RenumberLedCommand::redo() {
    iAnimation.doRenumberLed(iPosition, iNewNumber);

    setText();
}

void RenumberLedCommand::undo() {
    iAnimation.doRenumberLed(iPosition, iOldNumber);

    setText();
}

void RenumberLedCommand::setText() {
    QUndoCommand::setText(QObject::tr("Renumber led %1,%2 from %3 to %4").arg(iPosition.row()).arg(iPosition.column()).arg(iOldNumber).arg(iNewNumber));
}

SetFrameColourCommand::SetFrameColourCommand(Animation& animation, Frame &frame, const FrameValue& newValue) :
    LedAnimatorCommandBase(animation),
    iFrame(frame),
    iOldValue(frame.value()),
    iNewValue(newValue) {
}

void SetFrameColourCommand::redo() {
    iFrame.doSetValue(iNewValue);

    setText();
}

void SetFrameColourCommand::undo() {
    iFrame.doSetValue(iOldValue);

    setText();
}

void SetFrameColourCommand::setText() {
    QUndoCommand::setText(QObject::tr("Set frame colour"));
}


FadeCommand::FadeCommand(Animation &animation) :
    LedAnimatorCommandBase(animation) {
}

void FadeCommand::redo() {
    setText();
}

void FadeCommand::undo() {

    setText();
}

void FadeCommand::setText() {
    QUndoCommand::setText(QObject::tr("Fade"));
}

FadeToCommand::FadeToCommand(Animation &animation) :
    LedAnimatorCommandBase(animation) {
}

void FadeToCommand::redo() {

    setText();
}

void FadeToCommand::undo() {
    setText();
}

void FadeToCommand::setText() {
    QUndoCommand::setText(QObject::tr("Fade to"));
}


/*MoveLedCommand::MoveLedCommand(Animation &animation, Position fromPosition, Position toPosition) :
    LedAnimatorCommandBase(animation),
    iFromPosition(fromPosition),
    iToPosition(toPosition) {
}

void MoveLedCommand::redo() {
    iAnimation.moveLed(iFromPosition, iToPosition);

    setText();
}

void MoveLedCommand::undo() {
    iAnimation.moveLed(iToPosition, iFromPosition);

    setText();
}

void MoveLedCommand::setText() {
    QUndoCommand::setText(QObject::tr("Move led %1,%2 to %3, %4").arg(iFromPosition.row()).arg(iFromPosition.column()).arg(iToPosition.row()).arg(iToPosition.column()));
}

//bool MoveLedCommand::mergeWith(const QUndoCommand *command) {

//}

CutLedCommand::CutLedCommand(Animation &animation, Led& cutLed) :
    LedAnimatorCommandBase(animation),
    iCutLed(cutLed){
}

void CutLedCommand::redo() {
    iAnimation.moveLedToClipboard(iCutLed.position());

    setText();
}

void CutLedCommand::undo() {
   // iAnimation.addLedToClipboard(Led& newLed);
    Led* newLed = new Led(&iAnimation, iAnimation, iCutLed.number(), iCutLed.position());
    newLed->copyFrames(iCutLed);

    iAnimation.addLed(*newLed, newLed->number());
    setText();
}

void CutLedCommand::setText() {
    QUndoCommand::setText(QObject::tr("Cut led %1,%2").arg(iCutLed.position().row()).arg(iCutLed.position().column()));
}

//bool CutLedCommand::mergeWith(const QUndoCommand *command) {

//}

CopyLedCommand::CopyLedCommand(Animation &animation, Led& led) :
    LedAnimatorCommandBase(animation),
    iCopyLed(led){
}

void CopyLedCommand::redo() {
    iAnimation.moveLedToClipboard(iCopyLed.position());

    setText();
}

void CopyLedCommand::undo() {
   // iAnimation.moveLedToClipboard(iNewRow, iNewColumn);

    setText();
}

void CopyLedCommand::setText() {
    QUndoCommand::setText(QObject::tr("Copy led %1,%2").arg(iCopyLed.position().row()).arg(iCopyLed.position().column()));
}

//bool CopyLedCommand::mergeWith(const QUndoCommand *command) {
*/
