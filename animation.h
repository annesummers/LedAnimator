/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef ANIMATION_H
#define ANIMATION_H

#include <QObject>
#include <QTimer>
#include <QPoint>
#include <QApplication>
#include <QHash>

#include "engine.h"
#include "constants.h"

namespace AnimatorModel {

class Led;

class Animation : public QObject {
    Q_OBJECT

public:
    explicit Animation(Engine& engine);

    void setupNew(int numRows, int numColumns, int numFrames, int frameFrequency, int numLeds, QList<int> ledPositions);
    void setupNew(int numRows, int numColumns, int numFrames, int frameFrequency);

    void addNewLed(int row, int column, int ledNum = INVALID);
    void moveLed(Led& led, int toRow, int toColumm);
    void copyLed(Led& led, int toRow, int toColumn);
    void deleteLed(Led& led);

    void renumberLed(Led& led, int newNumber);

    void play();
    void stop();

    Led* ledAt(int row, int column) const;
    Led& ledAt(int number) const;

    void getLedPosition(int number, int* const row, int* const column) const;
   // const int getLedNumber(int row, int column) const;

    inline const QString fileName() const { return iFileName; }
    inline const bool isSaved() const { return iIsSaved; }

    inline const int currentFrame() const { return iCurrentFrame; }
    inline const int numFrames() const { return iNumFrames; }
    inline const int frameFrequency() const { return iFrameFrequency; }

    inline const int numRows() const { return iNumRows; }
    inline const int numColumns() const { return iNumColumns; }
    inline const int numLeds() const { return iNumLeds; }
    inline const int numGroups() const { return iNumGroups; }
    inline const int numSubAnimations() const { return iNumSubAnimations; }

    inline const bool isPlaying() const { return iIsPlaying; }

    inline void addGroup() { iNumGroups++; emit groupAdded(iNumGroups); }
    void selectGroup(int groupNumber);
    
signals:
    void currentFrameChanged(int currentFrame);
    void numFramesChanged(int numFrames);
    void groupAdded(int groupNumber);

    void newLed(int row, int column);
    void newSocket(int row, int column);

public slots:
    inline void setFileName(QString fileName) { iFileName = fileName; }
    inline void setSaved(bool saved) { iIsSaved = saved; }

    void copyToClipboard();

    void setCurrentFrame(int frame);
    void setFrameFrequency(int frameFrequency);
    inline void setNumFrames(int numFrames) { iNumFrames = numFrames; emit numFramesChanged(iNumFrames); }
    
private slots:
    void nextFrame();

private:
    void addLed(Led &led, int row, int column);

    inline void setNumRows(int numRows) { iNumRows = numRows; }
    inline void setNumColumns(int numColumns) { iNumColumns = numColumns; }
    inline void setNumLeds(int numLeds) { iNumLeds = numLeds; }

    inline void setPlaying(bool isPlaying) { iIsPlaying = isPlaying; }

    //void getGridPosition(int index, int* row, int* column) const;

    int gridPositionNumber(int row, int column) const;
    void setGridPositionNumber(int row, int column, int number);

    const Engine& iEngine;

    QHash<int, Led*> iLeds;
    QList<int> iPositions;

    QTimer*     iPlayTimer;

    int iNumRows;
    int iNumColumns;

    int iNumLeds;
    int iNumGroups;

    int iNumFrames;
    int iCurrentFrame;
    int iFrameFrequency;

    int iNumSubAnimations;

    bool iIsPlaying;

    QString iFileName;
    bool    iIsSaved;
};
}

#endif // ANIMATION_H
