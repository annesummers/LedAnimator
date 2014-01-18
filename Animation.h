/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef ANIMATION_H
#define ANIMATION_H

#include <QObject>
#include <QApplication>
#include <QHash>
#include <QUndoStack>

#include "LedSet.h"
#include "engine.h"
//#include "Frame.h"
#include "constants.h"
#include "TimeAxis.h"
#include "ValueAxis.h"

namespace AnimatorModel {

class Frame;

class Animation : public QObject {
    Q_OBJECT

public:
    explicit Animation(Engine& engine);

    void newAnimation(int numRows,
                      int numColumns,
                      int numLeds,
                      QList<int> ledPositions);
    void newAnimation(int numRows,
                      int numColumns);

    inline void setUndoStack(QUndoStack& undoStack) { iUndoStack = &undoStack; }

    void addNewLed(Position position);
    void deleteLed(Led &led, bool deleteObject);

    void addTimeAxis(int lowValue,
                     int highValue,
                     int speed,
                   int priority,
                   bool isOpaque);

    int addValueAxis(int lowValue,
                     int highValue,
                     int zeroValue,
                       int priority,
                       bool isOpaque);

    void moveLed(Position fromPosition, Position toPosition);
    void cloneLed(Position fromPosition, Position toPosition);
    void pasteLed(Position fromPosition, Position toPosition);

   // void setFrameColour(Frame& frame, QColor oldColour, QColor newColour);

    void doAddNewLed(Position position, int ledNum = INVALID);
    void doDeleteLed(Position position, bool deleteObject);

    void doMoveLed(Position fromPosition, Position toPosition);
    Led* doCloneLed(Position fromPosition, Position toPosition);
    void doPasteLed(Position fromPosition, Position toPosition, Led **fromLed, Led **toLed);

    void copyLedTimeAxis(Position fromPosition, Position toPosition);
    void copyLedValueAxis(Position fromPosition, Position toPosition, int axisNum);
    void copyLedCurrentFrame(Position fromPosition, Position toPosition, int copyFrameNum);

    Led* doCopyLedValueAxis(Position fromPosition, Position toPosition, int axisNum);
    Led* doCopyLedCurrentFrame(Position fromPosition, Position toPosition, int copyFrameNum);
    Led* doCopyLedTimeAxis(Position fromPosition, Position toPosition);

    void moveLedToClipboard(Position position);
    void addLedToClipboard(Led* led);
    void deleteLedFromClipboard(int ledNumber);

    void renumberLed(Position position, int oldNumber, int newNumber);
    void doRenumberLed(Position position, int newNumber);

    void play(bool repeat);
    void stop();

    Led* ledAt(Position position) const;
    Led *ledAt(int number) const;

    ValueAxis& axisAt(int number) const;
    inline TimeAxis* timeAxis() const { return iTimeAxis; }

    inline int numValueAxes() const { return iAxes.count(); }

    inline bool isMissing(int ledNumber) { return iLeds->isMissing(ledNumber); }
    inline bool ledsMissing() { return iLeds->ledsMissing(); }
    inline const QList<int> missingLedNumbers() const { return iLeds->missingLedNumbers(); }

    Position ledPosition(int number) const;

    inline const QString fileName() const { return iFileName; }
    inline bool isSaved() const { return iIsSaved; }

    inline int numRows() const { return iNumRows; }
    inline int numColumns() const { return iNumColumns; }
    inline int numLeds() const { return iLeds->count(); }

    int nextLedNumber() const ;

    int addFunction(Function function);
    inline Function functionAt(int number) { return iFunctions.at(number); }
    inline int numFunctions() { return iFunctions.size(); }
    
signals:
   // void framesInserted(int numFrames, int numFramesAdded);
    //void numFramesChanged(int numFrames);

    void timeAxisAdded();
    void valueAxisAdded(int axisNumber);

    void newLed(int row, int column);
    void newSocket(int row, int column);

    void ledDeleted(int row, int column, int number);
    void ledMoved(int oldRow, int oldColumn, int newRow, int newColumn);
    void ledRenumbered(int row, int column, int oldNumber);

public slots:
    inline void setFileName(QString fileName) { iFileName = fileName; }
    inline void setSaved(bool saved) { iIsSaved = saved; }

    void copyToClipboard();

    //void addFrames(int numFrames);
    //inline void setNumFrames(int numFrames) { iNumFrames = numFrames; emit numFramesChanged(iNumFrames); }


private:
    void addLed(Led &led, Position position);
    void addLed(Led & led, int number);

    void removeLed(Led& led);

    inline void setNumRows(int numRows) { iNumRows = numRows; iLeds->setNumRows(numRows); iClipboardLeds->setNumRows(numRows);}
    inline void setNumColumns(int numColumns) { iNumColumns = numColumns; iLeds->setNumColumns(numColumns); iClipboardLeds->setNumColumns(numColumns);}

    int gridPositionNumber(Position position) const;
    void setGridPositionNumber(Position position, int number);
    void setClipboardGridPositionNumber(Position position, int number);

    const Engine& iEngine;

    QUndoStack* iUndoStack;

    LedSet* iLeds;
    LedSet* iClipboardLeds;

    int iNumRows;
    int iNumColumns;

    QList<ValueAxis*> iAxes;
    TimeAxis* iTimeAxis;

    QList<Function> iFunctions;

    QString iFileName;
    bool    iIsSaved;
};
}

#endif // ANIMATION_H
