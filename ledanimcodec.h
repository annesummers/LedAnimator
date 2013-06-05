/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef LEDANIMCODEC_H
#define LEDANIMCODEC_H

#include <QObject>
#include <QList>
#include <QColor>
#include <QSize>

namespace AnimatorModel {
class Animation;
class Led;
}

using namespace AnimatorModel;

namespace ImportExport {

class AnimChar {

public:
    AnimChar(const int value);
    AnimChar(const unsigned char value);
    AnimChar();

    const int intValue() const;
    const unsigned char charValue() const;

private:
    const unsigned char iValue;
};

class AnimStringChar : public AnimChar {

};

class AnimByteChar : public AnimChar {

};

class LedAnimCodec : public QObject {
    Q_OBJECT

public:
    explicit LedAnimCodec(Animation &animation);

    void writeAnimation(bool withPositions);
    void readAnimation();

protected:
    virtual const AnimChar readCharacter() const = 0;
    virtual void writeCharacter(AnimChar character) = 0;

    virtual void writeControlCharacter(AnimChar character) = 0;
    virtual const AnimChar readControlCharacter() const = 0;

    virtual const QByteArray&  asByteArray() const = 0;
    virtual const QString     asString() const = 0;

    virtual void writePositionData() { }
    virtual QList<int> readPositionData(int* numRows, int* numColumns, int numLeds) { Q_UNUSED(numRows); Q_UNUSED(numColumns); Q_UNUSED(numLeds); QList<int> positions; return positions; }

    virtual void writeColourData() = 0;
    virtual void readColourData() = 0;

    void writeColour(QColor colour);
    const QColor readColour() const;

    Animation& iAnimation;
    bool iWriteLedNumber;
};

class LedAnimStringCodec: public LedAnimCodec {
    Q_OBJECT
public:
    explicit LedAnimStringCodec(Animation &animation);

    const QString asString() const;

protected:
    const AnimChar readCharacter() const;
    void writeCharacter(AnimChar character);

    void writeControlCharacter(AnimChar character);
    const AnimChar readControlCharacter() const { return AnimChar(); }

    const QByteArray &asByteArray() const;

    void writeColourData();
    void readColourData();

private:
    QString iString;
    int iLastReturn;
};

class LedAnimByteArrayCodec: public LedAnimCodec {
    Q_OBJECT
public:
    explicit LedAnimByteArrayCodec(Animation& animation);
    LedAnimByteArrayCodec(Animation& animation, QByteArray bytes);

    void writeAnimation(bool withPositions);
    void readAnimation();

    const QString asString() const;
    const QByteArray& asByteArray() const;

protected:
    const AnimChar readCharacter() const;
    void writeCharacter(AnimChar character);

    void writeControlCharacter(AnimChar character);
    const AnimChar readControlCharacter() const;

    void writePositionData();
    QList<int> readPositionData(int *numRows, int *numColumns, int numLeds);

    void writeColourData();
    void readColourData();

private:
    QByteArray  iByteArray;
    mutable int iPosition;
};
}

#endif // LEDANIMCODEC_H
