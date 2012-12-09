#ifndef ANIMATION_H
#define ANIMATION_H

#include <QObject>
#include <QTimer>
#include <QPoint>

#include "engine.h"

class Led;

class Animation : public QObject
{
    Q_OBJECT
public:
    explicit Animation(Engine& engine);

    void setupNew(int numRows, int numColumns, int numFrames);

    int currentFrame();
    void setCurrentFrame(int frame);

    int numFrames();
    void setNumFrames(int numFrames);

    int numRows();
    int numColumns();
    int numLeds();

    int frameFrequency();
    void setFrameFrequency(int frameFrequency);

    void play();
    void stop();

    bool isPlaying();

    Led& ledAt(int row, int column);

    QString fileName();
    void setFileName(QString fileName);

    bool saved();
    void setSaved(bool saved);

    void ledColourChanged(Led& led, int frame);
    
signals:
    void currentFrameChanged(int currentFrame);
    void numFramesChanged(int numFrames);

    void ledColourChanged(int row, int column, int frame);

    void newLed(Led& led);
    
public slots:
    void nextFrame();

private:
    Engine& iEngine;

    QList<Led*> iLeds;

    int iNumRows;
    int iNumColumns;

    int iNumFrames;
    int iCurrentFrame;
    int iFrameFrequency;

    bool iIsPlaying;

    QString iFileName;
    bool iIsSaved;

    QTimer* iPlayTimer;
};

#endif // ANIMATION_H
