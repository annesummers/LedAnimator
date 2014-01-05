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

#include "Function.h"

namespace AnimatorModel {
class Animation;
class Led;
class Frame;
class Axis;
class FunctionValue;
}

using namespace AnimatorModel;

namespace ImportExport {

class AnimChar {

public:
    AnimChar(const int value);
    AnimChar(const unsigned char value);
    AnimChar();

    const int intValue() const;
    const unsigned int unsignedIntValue() const;
    const unsigned char unsignedCharValue() const;
    const bool boolValue() const;

private:
    const char iValue;
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

    void writeAxisData(int iAxisNum);
    void readAxisData(int iAxisNum);

    void writeAxis(int iAxisNum);
    void readAxis(int iAxisNum);

    virtual void writeColour(Frame& frame) = 0;
    virtual const QColor readColour() const = 0;

    virtual void writeFunction(Frame& frame) = 0;
    virtual const FunctionValue& readFunction(Frame& frame) const = 0;

    virtual void writeFunctionData(Function function) = 0;
    virtual Function readFunctionData() const = 0;

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

    //void writeAxisData(int axisNum);
    //void readAxisData(int iAxisNum);

    virtual void writeColour(Frame& frame);
    virtual const QColor readColour() const;

    void writeFunction(Frame& frame);
    const FunctionValue& readFunction(Frame& frame) const;

    virtual void writeFunctionData(Function function);
    virtual Function readFunctionData() const;

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

    virtual void writeFunction(Frame& frame);
    virtual const FunctionValue& readFunction(Frame& frame) const;

    virtual void writeFunctionData(Function function);
    virtual Function readFunctionData() const;

    virtual void writeColour(Frame& frame);
    virtual const QColor readColour() const;

private:
    QByteArray  iByteArray;
    mutable int iPosition;
};

class LedAnimSimpleByteArrayCodec: public LedAnimByteArrayCodec {
    Q_OBJECT
public:
    explicit LedAnimSimpleByteArrayCodec(Animation& animation);
    LedAnimSimpleByteArrayCodec(Animation& animation, QByteArray bytes);

    virtual void writeFunction(Frame& frame);
    virtual const FunctionValue& readFunction(Frame& frame) const;

    virtual void writeFunctionData(Function function);
    virtual Function readFunctionData() const;

    virtual void writeColour(Frame& frame);
    virtual const QColor readColour() const;
};

}

#endif // LEDANIMCODEC_H
