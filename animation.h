/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef ANIMATION_H
#define ANIMATION_H

#include <QObject>
#include <QTimer>
#include <QApplication>
#include <QHash>
#include <QUndoStack>

#include "engine.h"
#include "frame.h"
#include "constants.h"

namespace AnimatorModel {

class Led;
class LedSetIterator;

class LedSet {

public :
    LedSet();

    void addLed(Led& led);
    void removeLed(Led& led);

    void clearPositions();
    inline void clear() { iLeds.clear(); }
    void clearAll();

    Led* findLed(Position position) const;
    Led* findLed(int ledNumber) const;

    void moveLed(Led &led, Position fromPosition, Position toPosition);

    void setPositions(QList<int> positions);

    Position ledPosition (int ledNumber) const;

    inline bool ledsMissing() const { return iFreeNumbers.count() != 0; }
    inline bool isMissing(int ledNumber) const { return iFreeNumbers.contains(ledNumber); }
    inline const QList<int> missingLedNumbers() const { return iFreeNumbers; }

    void clearMissing();
    void addMissing(int ledNumber);

    inline void setHighestNumber(int highestNumber) { iHighestNumber = highestNumber; }
    inline int highestNumber() const { return iHighestNumber; }

    inline int count() const { return iLeds.count(); }

    LedSetIterator& iterator();

    inline void setNumRows(int numRows) { iNumRows = numRows; }
    inline void setNumColumns(int numColumns) { iNumColumns = numColumns; }

private:
    LedSet(const LedSet &set);
    int positionNumber(Position position) const;
    void setPositionNumber(Position position, int number);
    LedSetIterator* iIterator;

    int iHighestNumber;
    QList<int> iFreeNumbers;

    QList<int> iPositions;

    QHash<int, Led*> iLeds;

    int iNumRows;
    int iNumColumns;

    friend class LedSetIterator;
};

class LedSetIterator : public QObject {
    Q_OBJECT

public:
    LedSetIterator (const LedSet *set);
    bool findNext (const int group);
    bool findPrevious (const int group);
    bool hasNext () const;
    bool hasPrevious () const;
    Led& next ();
    const Led& peekNext () const;
    const Led& peekPrevious () const;
    Led& previous ();
    void toBack ();
    void toFront ();
private:
    LedSetIterator &	operator= (const LedSetIterator &set);
    const LedSet& iLedSet;
    int iNextCounter;
    int iPreviousCounter;
};

class Animation : public QObject {
    Q_OBJECT

public:
    explicit Animation(Engine& engine);

    void setupNew(int numRows, int numColumns, int numFrames, int frameFrequency, int numLeds, QList<int> ledPositions);
    void setupNew(int numRows, int numColumns, int numFrames, int frameFrequency);

    inline void setUndoStack(QUndoStack& undoStack) { iUndoStack = &undoStack; }

    void addNewLed(Position position);
    void deleteLed(Led &led, bool deleteObject);

    void moveLed(Position fromPosition, Position toPosition);
    void cloneLed(Position fromPosition, Position toPosition);
    void pasteLed(Position fromPosition, Position toPosition);

    void setFrameColour(Frame& frame, QColor oldColour, QColor newColour);

    void doAddNewLed(Position position, int ledNum = INVALID);
    void doDeleteLed(Position position, bool deleteObject);

    void doMoveLed(Position fromPosition, Position toPosition);
    Led* doCloneLed(Position fromPosition, Position toPosition);
    void doPasteLed(Position fromPosition, Position toPosition, Led **fromLed, Led **toLed);

    void moveLedToClipboard(Position position);
    void addLedToClipboard(Led* led);
    void deleteLedFromClipboard(int ledNumber);

    void renumberLed(Position position, int oldNumber, int newNumber);
    void doRenumberLed(Position position, int newNumber);

    void play(bool repeat);
    void stop();

    Led* ledAt(Position position) const;
    Led *ledAt(int number) const;

    inline bool isMissing(int ledNumber) { return iLeds.isMissing(ledNumber); }
    inline bool ledsMissing() { return iLeds.ledsMissing(); }
    inline const QList<int> missingLedNumbers() const { return iLeds.missingLedNumbers(); }

    Position ledPosition(int number) const;

    inline const QString fileName() const { return iFileName; }
    inline const bool isSaved() const { return iIsSaved; }

    inline const int currentFrame() const { return iCurrentFrame; }
    inline const int numFrames() const { return iNumFrames; }
    inline const int frameFrequency() const { return iFrameFrequency; }

    inline const int numRows() const { return iNumRows; }
    inline const int numColumns() const { return iNumColumns; }
    inline const int numLeds() const { return iLeds.count(); }
    inline const int numGroups() const { return iNumGroups; }
    inline const int numSubAnimations() const { return iNumSubAnimations; }

    inline const bool isPlaying() const { return iIsPlaying; }

    inline void addGroup() { iNumGroups++; emit groupAdded(iNumGroups); }
    void selectGroup(int groupNumber);

    int nextLedNumber() const ;
    
signals:
    void currentFrameChanged(int currentFrame);
    void framesInserted(int numFrames, int numFramesAdded);
    void numFramesChanged(int numFrames);

    void groupAdded(int groupNumber);

    void newLed(int row, int column);
    void newSocket(int row, int column);

    void ledDeleted(int row, int column, int number);
    void ledMoved(int oldRow, int oldColumn, int newRow, int newColumn);
    void ledRenumbered(int row, int column, int oldNumber);

    void stopped();

public slots:
    inline void setFileName(QString fileName) { iFileName = fileName; }
    inline void setSaved(bool saved) { iIsSaved = saved; }

    void copyToClipboard();

    void setCurrentFrame(int frame);
    void setFrameFrequency(int frameFrequency);

    void addFrames(int numFrames);
    inline void setNumFrames(int numFrames) { iNumFrames = numFrames; emit numFramesChanged(iNumFrames); }
    
private slots:
    void nextFrame();

private:
    void addLed(Led &led, Position position);
    void addLed(Led & led, int number);

    void removeLed(Led& led);

    inline void setNumRows(int numRows) { iNumRows = numRows; iLeds.setNumRows(numRows); iClipboardLeds.setNumRows(numRows);}
    inline void setNumColumns(int numColumns) { iNumColumns = numColumns; iLeds.setNumColumns(numColumns); iClipboardLeds.setNumColumns(numColumns);}

    inline void setPlaying(bool isPlaying) { iIsPlaying = isPlaying; }

    int gridPositionNumber(Position position) const;
    void setGridPositionNumber(Position position, int number);
    void setClipboardGridPositionNumber(Position position, int number);

    const Engine& iEngine;

    QUndoStack*     iUndoStack;

    LedSet iLeds;
    LedSet iClipboardLeds;

    QTimer*     iPlayTimer;

    int iNumRows;
    int iNumColumns;

    int iNumGroups;

    int iNumFrames;
    int iCurrentFrame;
    int iFrameFrequency;

    int iNumSubAnimations;

    bool iIsPlaying;
    bool iRepeat;

    QString iFileName;
    bool    iIsSaved;
};
}

#endif // ANIMATION_H
