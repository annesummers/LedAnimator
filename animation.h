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

    QString fileName();
    void setFileName(QString fileName);

    bool saved();
    void setSaved(bool saved);

    int currentFrame();

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
    
signals:
    void currentFrameChanged(int currentFrame);
    void numFramesChanged(int numFrames);

    void newLed(Led& led);
    
public slots:
    void setCurrentFrame(int frame);
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
