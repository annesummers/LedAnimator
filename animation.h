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

#include "engine.h"

class Led;

class Animation : public QObject {
    Q_OBJECT

public:
    explicit Animation(Engine& engine);

    void setupNew(int numRows, int numColumns, int numFrames);

    void play();
    void stop();

    Led& ledAt(int row, int column) const;

    inline const QString fileName() const { return iFileName; }
    inline const bool isSaved() const { return iIsSaved; }

    inline const int currentFrame() const { return iCurrentFrame; }
    inline const int numFrames() const { return iNumFrames; }
    inline const int frameFrequency() const { return iFrameFrequency; }

    inline const int numRows() const { return iNumRows; }
    inline const int numColumns() const { return iNumColumns; }
    inline const int numLeds() const { return iLeds.count(); }

    inline const bool isPlaying() const { return iIsPlaying; }
    
signals:
    void currentFrameChanged(int currentFrame);
    void numFramesChanged(int numFrames);

    void newLed(int row, int column);

public slots:
    inline void setFileName(QString fileName) { iFileName = fileName; }
    inline void setSaved(bool saved) { iIsSaved = saved; }

    inline void setCurrentFrame(int frame) { iCurrentFrame = frame; emit currentFrameChanged(iCurrentFrame); }
    inline void setNumFrames(int numFrames) { iNumFrames = numFrames; emit numFramesChanged(iNumFrames); }
    inline void setFrameFrequency(int frameFrequency) { iFrameFrequency = frameFrequency; }
    
private slots:
    void nextFrame();

private:
    inline void setNumRows(int numRows) { iNumRows = numRows; }
    inline void setNumColumns(int numColumns) { iNumColumns = numColumns; }

    inline void setPlaying(bool isPlaying) { iIsPlaying = isPlaying; }

    const Engine& iEngine;

    QList<Led*> iLeds;
    QTimer*     iPlayTimer;

    int iNumRows;
    int iNumColumns;

    int iNumFrames;
    int iCurrentFrame;
    int iFrameFrequency;

    bool iIsPlaying;

    QString iFileName;
    bool    iIsSaved;
};

#endif // ANIMATION_H
