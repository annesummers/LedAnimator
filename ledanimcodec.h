/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef LEDANIMCODEC_H
#define LEDANIMCODEC_H

#include <QObject>
#include <QList>
#include <QPoint>
#include <QColor>
#include <QSize>

#include "animatorbase.h"

class Animation;
class Led;

namespace ImportExport {

class AnimChar {

public:
    AnimChar(const int value);
    AnimChar(const unsigned char value);

    const int intValue() const;
    const unsigned char charValue() const;

private:
    const unsigned char iValue;
};

class AnimStringChar : public AnimChar {

};

class AnimByteChar : public AnimChar {

};

class LedAnimCodec : public AnimatorBase
{
    Q_OBJECT
public:
    explicit LedAnimCodec(Animation &animation);

    void writeAnimation();
    void readAnimation();

protected:
    virtual const AnimChar readCharacter() const = 0;
    virtual void writeCharacter(AnimChar character) = 0;

    virtual const QByteArray&  asByteArray() const = 0;
    virtual const QString     asString() const = 0;

private:
    void writeColour(QColor colour);
    const QColor readColour() const;
};

class LedAnimStringCodec: public LedAnimCodec {
    Q_OBJECT
public:
    explicit LedAnimStringCodec(Animation &animation);

protected:
    const AnimChar readCharacter() const;
    void writeCharacter(AnimChar character);

    const QString asString() const;
    const QByteArray &asByteArray() const;

private:
    QString iString;
};

class LedAnimByteArrayCodec: public LedAnimCodec {
    Q_OBJECT
public:
    explicit LedAnimByteArrayCodec(Animation& animation);
    LedAnimByteArrayCodec(Animation& animation, QByteArray bytes);


    const QString asString() const;
    const QByteArray& asByteArray() const;

protected:
    const AnimChar readCharacter() const;
    void writeCharacter(AnimChar character);

private:
    QByteArray  iByteArray;
    mutable int iPosition;
};
}

#endif // LEDANIMCODEC_H
